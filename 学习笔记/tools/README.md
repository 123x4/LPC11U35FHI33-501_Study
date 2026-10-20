# hid_tool.py 使用说明

没有串口模块和调试器的时候，USB 就是我们和板子之间唯一的“窗口”。这个工具通过 USB 接口 2（自定义 HID 接口，MouseTest.exe 用的也是这个接口）和固件通信。

## 安装

```
pip install hidapi
python hid_tool.py list
```

正常情况下会列出 3 个接口（0=鼠标，1=键盘，2=自定义，其 usage_page=0xFF01）。

> **不要和 MouseTest.exe 同时运行**，两者用的是同一个接口。

## 功能

| 命令 | 作用 | 用于哪个学习阶段 |
|---|---|---|
| `info L` | 读一次左键的 raw、top/bottom 基线、段数 | 10 |
| `watch LR` | 连续显示左右键的数据，相当于 GUI 的 RowData 面板 | 6、10 |
| `stats L 5000` | 采集 5000 次 raw 值，输出均值、标准差、直方图（**手不要碰按键**） | 6 |
| `getcfg L` | 读出全部配置（AP/RT、振感、制动） | 12 |
| `setcoil L 500` | 把左键按下振感设为 500µs（**会写入 EEPROM**） | 9、12 |
| `log` | 显示固件的 USB 调试日志 | 3~14 |

## 打开固件的 USB 日志

修改 `inc/my_debug.h`：

```c
#define MYDEBUG_ENABLE          1
...
#define MYDEBUG_PRINTF_ENABLE   (MYDEBUG_USB_PRINTF_MASK)   // 原来是 UART_PRINTF_MASK
```

重新编译、烧录，然后运行 `python hid_tool.py log`。代码里的 `InfoLog(...)` 就会显示在电脑上。

注意事项：
- 日志和协议响应共用同一个端点，日志模式下 GUI 可能工作不正常。
- 打印本身要消耗 CPU 时间，**测时间相关的东西时，不要在被测代码段里打印**。应该先把测量结果存进变量，再在别的地方打印。
- 用完记得改回原来的设置。

## 验证情况

- 报文格式和校验和已经和固件原版 `src/my_protocol.c` 交叉验证过（用 PC 编译固件代码，对比两边计算结果，一致）。
- **还没有在真实板子上运行过**。第一次使用时如果遇到问题，把错误信息发给我。
