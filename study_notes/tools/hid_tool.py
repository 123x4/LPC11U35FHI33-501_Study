#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
HapticPOC 鼠标 USB 小工具（无仪器学习用）

通过 USB 接口 2（自定义 HID，64 字节收发，无 Report ID）和固件通信，
协议见 inc/my_protocol.h、inc/my_protocol_common.h、src/my_protocol_mouse.c。

安装:  pip install hidapi
用法:
  python hid_tool.py list                 列出鼠标的所有 HID 接口
  python hid_tool.py info  L|R            读一次 GET_INFO/ALL（raw、基线、段数、线圈电压配置）
  python hid_tool.py watch [L|R|LR]       连续读取 raw/基线/段数（相当于 GUI 的 RowData 面板）
  python hid_tool.py stats L|R [N]        采 N 次 raw 值做统计：均值、标准差、最小、最大、直方图
  python hid_tool.py getcfg L|R           读取 GET_CONFIG/ALL（AP/RT、按下和释放振感、制动）
  python hid_tool.py setcoil L|R 500      设置按下振感导通时间（µs）。注意：会写入 EEPROM
  python hid_tool.py log                  打印固件的 USB 调试日志（需要固件开启 USB 打印，见 README）

注意: 本工具和 MouseTest.exe 都使用接口 2，不要同时运行。
"""
import struct
import sys
import time

try:
    import hid
except ImportError:
    sys.exit("请先安装 hidapi:  pip install hidapi")

VID, PID = 0x1A81, 0x2071          # inc/usbdesc.h:39-40
CUSTOM_IFACE = 2                   # 接口 2 = 自定义通信（configs.xml 中的 mi_02）
PREAMBLE = 0x55                    # PACKET_PREAMBLER，同时也是 pc_communicate.c 中的 TEST_CMD
ID_HOST, ID_MOUSE = 0x01, 0x02
TYPE_REQUEST, TYPE_RESPONSE = 0x02, 0x03
CMD_GET_INFO, CMD_GET_CONFIG, CMD_SET_CONFIG = 0x01, 0x02, 0x03
INFO_ALL, INFO_HALL_RAW, INFO_HALL_SEGMENT = 0xFF, 0x01, 0x02
CFG_ALL, CFG_COIL = 0xFF, 0x02
VK = {"L": 0x01, "R": 0x02}        # VK_LBUTTON / VK_RBUTTON


# ---------------- 报文打包与解析（对应 my_protocol.h） ----------------
def checksum(dst, src, payload):
    """Protocol_CalculateChecksum(): 目的ID + 源ID + 长度低字节 + 长度高字节 + 负载各字节，取低 8 位"""
    n = len(payload)
    return (dst + src + (n & 0xFF) + (n >> 8) + sum(payload)) & 0xFF


def build_packet(cmd, sub, vk, data=b""):
    """PacketHeader(6B) + PayloadHeader(4B) + data"""
    payload = bytes([TYPE_REQUEST, cmd, sub, vk]) + data
    head = struct.pack("<BBBBH", PREAMBLE, ID_MOUSE, ID_HOST,
                       checksum(ID_MOUSE, ID_HOST, payload), len(payload))
    return head + payload


def parse_packet(buf):
    """返回 (cmd, sub, vk, data)，不是合法的响应报文时返回 None"""
    buf = bytes(buf)  # hidapi 的 read() 返回的是 list，struct 需要 bytes
    if len(buf) < 10 or buf[0] != PREAMBLE or buf[1] != ID_HOST:
        return None
    _, dst, src, csum, n = struct.unpack_from("<BBBBH", buf)
    payload = bytes(buf[6:6 + n])
    if len(payload) != n or checksum(dst, src, payload) != csum:
        return None
    ptype, cmd, sub, vk = payload[:4]
    if ptype != TYPE_RESPONSE:
        return None
    return cmd, sub, vk, payload[4:]


# ---------------- 设备访问 ----------------
def find_path():
    for d in hid.enumerate(VID, PID):
        if d.get("interface_number") == CUSTOM_IFACE:
            return d["path"]
    sys.exit("没找到鼠标的接口 2。请确认鼠标已插上，并且关闭了 MouseTest.exe")


class Mouse:
    def __init__(self):
        self.dev = hid.device()
        self.dev.open_path(find_path())

    def close(self):
        self.dev.close()

    def request(self, cmd, sub, vk, data=b"", timeout_ms=200):
        pkt = build_packet(cmd, sub, vk, data)
        # hidapi 要求第一个字节是 Report ID；本接口没有 Report ID，所以填 0
        self.dev.write([0x00] + list(pkt.ljust(64, b"\x00")))
        deadline = time.time() + timeout_ms / 1000
        while time.time() < deadline:
            r = self.dev.read(64, timeout_ms)
            if not r:
                continue
            p = parse_packet(r)
            if p and p[0] == cmd and p[1] == sub and p[2] == vk:
                return p[3]
        return None


# ---------------- 结构体解析（对应 my_protocol_common.h） ----------------
def parse_raw(d):      # HallRawInfo_ts
    cur, bottom, top = struct.unpack_from("<HHH", d)
    return dict(raw=cur, bottom=bottom, top=top)


def parse_seg(d):      # HallSegmentInfo_ts
    seg, mx = struct.unpack_from("<HH", d)
    return dict(seg=seg, max=mx)


def parse_all_info(d):  # AllInfo_ts
    r = parse_raw(d)
    r.update(parse_seg(d[6:]))
    r["coil_mv"] = struct.unpack_from("<HHH", d, 10)
    return r


def parse_all_cfg(d):   # AllConfig_ts
    rt = struct.unpack_from("<BBHHH", d, 0)
    coil = struct.unpack_from("<BBH", d, 8)
    rel = struct.unpack_from("<BBH", d, 12)
    brk = struct.unpack_from("<BBHH", d, 16)
    brel = struct.unpack_from("<BBHH", d, 22)
    return {
        "RT(使能掩码, AP, RT按下, RT松开)": (hex(rt[1]), rt[2], rt[3], rt[4]),
        "按下振感(使能掩码, Ton_us)": (hex(coil[1]), coil[2]),
        "释放振感(使能掩码, Ton_us)": (hex(rel[1]), rel[2]),
        "按下制动(使能掩码, 延时us, 时长us)": (hex(brk[1]), brk[2], brk[3]),
        "释放制动(使能掩码, 延时us, 时长us)": (hex(brel[1]), brel[2], brel[3]),
    }


def read_raw_seg(m, vk):
    a = m.request(CMD_GET_INFO, INFO_HALL_RAW, vk)
    b = m.request(CMD_GET_INFO, INFO_HALL_SEGMENT, vk)
    if a is None or b is None:
        return None
    r = parse_raw(a)
    r.update(parse_seg(b))
    return r


# ---------------- 子命令 ----------------
def cmd_list():
    for d in hid.enumerate(VID, PID):
        print(f"接口 {d.get('interface_number')}  usage_page=0x{d.get('usage_page', 0):04X}  "
              f"usage=0x{d.get('usage', 0):02X}  {d.get('product_string')}")


def cmd_info(key):
    m = Mouse()
    d = m.request(CMD_GET_INFO, INFO_ALL, VK[key])
    m.close()
    print(parse_all_info(d) if d else "无响应")


def cmd_getcfg(key):
    m = Mouse()
    d = m.request(CMD_GET_CONFIG, CFG_ALL, VK[key])
    m.close()
    if not d:
        print("无响应")
        return
    for k, v in parse_all_cfg(d).items():
        print(f"{k:28s} {v}")


def cmd_setcoil(key, us):
    data = struct.pack("<BBH", 0xAA, 0x80, int(us))   # bReserve, bmEnableMask(bit7=使能), wCoilOnTimeUs
    m = Mouse()
    d = m.request(CMD_SET_CONFIG, CFG_COIL, VK[key], data)
    m.close()
    print("成功" if d and d[0] == 0x01 else f"失败: {d}")


def cmd_watch(keys):
    m = Mouse()
    print("按 Ctrl+C 停止")
    try:
        while True:
            line = []
            for k in keys:
                r = read_raw_seg(m, VK[k])
                if r:
                    line.append(f"{k}: raw={r['raw']:5d} top={r['top']:5d} "
                                f"bottom={r['bottom']:5d} seg={r['seg']:3d}")
            print("   ".join(line))
            time.sleep(0.05)
    except KeyboardInterrupt:
        pass
    m.close()


def cmd_stats(key, n):
    m = Mouse()
    vals = []
    print(f"采样 {n} 次，期间请不要碰按键 ...")
    while len(vals) < n:
        a = m.request(CMD_GET_INFO, INFO_HALL_RAW, VK[key])
        if a:
            vals.append(parse_raw(a)["raw"])
    m.close()
    mean = sum(vals) / n
    std = (sum((v - mean) ** 2 for v in vals) / n) ** 0.5
    print(f"均值={mean:.2f}  标准差={std:.2f}  最小={min(vals)}  最大={max(vals)}  峰峰值={max(vals) - min(vals)}")
    print("直方图:")
    for v in range(min(vals), max(vals) + 1):
        c = vals.count(v)
        print(f"  {v:5d} | {'#' * max(1, c * 60 // n) if c else ''} {c}")
    print("注意: 这里的 raw 是固件滤波之后的值（Hall_SyncParam 中的 wCurrentRaw）。")


def cmd_log():
    """固件开启 MYDEBUG_USB_PRINTF_MASK 后，日志文本以 64 字节包从接口 2 发出，末尾补 0"""
    m = Mouse()
    print("按 Ctrl+C 停止")
    try:
        while True:
            r = m.dev.read(64, 500)
            if r:
                sys.stdout.write(bytes(r).rstrip(b"\x00").decode("utf-8", "replace"))
                sys.stdout.flush()
    except KeyboardInterrupt:
        pass
    m.close()


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return
    c = argv[1]
    if c == "list":
        cmd_list()
    elif c == "info":
        cmd_info(argv[2])
    elif c == "getcfg":
        cmd_getcfg(argv[2])
    elif c == "setcoil":
        cmd_setcoil(argv[2], argv[3])
    elif c == "watch":
        cmd_watch(list(argv[2]) if len(argv) > 2 else ["L", "R"])
    elif c == "stats":
        cmd_stats(argv[2], int(argv[3]) if len(argv) > 3 else 2000)
    elif c == "log":
        cmd_log()
    else:
        print(__doc__)


if __name__ == "__main__":
    main(sys.argv)
