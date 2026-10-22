# 1-6 参数读取：`Param_Init()` 与 EEPROM / IAP

> 📍 **位置**：`main()` → `Device_Init()`（`src/device.c:415`）→ **第 433 行 `Param_Init();`**
> 📄 **代码**：`src/my_protocol_mouse.c` 第 204~268 行（`Param_Init`）、第 19~31 行（地址和结构体）、第 52~60 行（`CalculateChecksum`）、第 80~107 行（`Init_CfgParamToRun`）；`src/IAP_CMD.c`（EEPROM 读写函数）；结构体定义在 `inc/my_protocol_common.h` 第 63~164 行
> 📖 **手册**：UM 第 20 章 —— p394 20.8.7（IAP 期间的中断）、20.8.9（IAP 用的 RAM）；p410~411 20.14 节（IAP 调用方法、Table 382 命令表）；p416 Table 393/394（EEPROM 写/读命令）；DS p48 Table 10（EEPROM 寿命和写入时间）

---

## 一、这一课要解决什么问题

鼠标的触发点（AP）、快速触发（RT）、线圈通电时间这些参数，用户可以在 GUI 里改。改完之后**拔掉 USB 再插上，参数还得在**。

RAM 断电就清空，所以参数必须存在**断电不丢**的地方。这颗芯片有一块专门的 **4 KB EEPROM**：

📖 DS p1：“4 kB on-chip EEPROM data memory; byte erasable and byte programmable; on-chip API support.”

`Param_Init()` 做的事：**开机时从 EEPROM 把参数读到 RAM 里；如果 EEPROM 里的数据不对，就写一份默认值进去**。

---

## 二、EEPROM 不能像 RAM 那样直接读写

0-2 课讲存储器映射时说过：EEPROM **不在地址空间里**，不能用指针 `*(uint8_t *)0x...` 直接访问。

📖 DS p1 说的 “on-chip API support” 就是唯一的访问方法：调用 **ROM 里的 IAP 程序**（In-Application Programming，在应用中编程）。

### IAP 怎么调用

📖 **UM p410，20.14 节**：

> The IAP routine resides at 0x1FFF 1FF0 location and it is thumb code.
> `#define IAP_LOCATION 0x1fff1ff1`

IAP 是固化在芯片 ROM 里的一段程序（和 0-1 课的 ROM Bootloader 在同一块 ROM 里）。调用方法：

```
准备一个数组 command[5]：  [0] = 命令号  [1..4] = 参数
准备一个数组 result[5]：   [0] = 返回的状态码
跳到 0x1FFF1FF1（末位 1 = Thumb），r0 = command 地址，r1 = result 地址
```

`src/IAP_CMD.c` 第 34~45 行就是这么写的：

```c
#define LPC_IAP_ENTRY_LOCATION        0X1FFF1FF1
typedef void (*LPC_IAP_ENTRY_T)(unsigned int[], unsigned int[]);

static __inline void lpc_iap_entry(unsigned int cmd_param[], unsigned int status_result[])
{
	__disable_irq();                                                     // 关中断
	((LPC_IAP_ENTRY_T) LPC_IAP_ENTRY_LOCATION)(cmd_param, status_result); // 把地址当函数调用
	__enable_irq ();                                                     // 开中断
}
```

第二行的写法看起来吓人，拆开看：

```
LPC_IAP_ENTRY_LOCATION              一个数字 0x1FFF1FF1
(LPC_IAP_ENTRY_T) 0x1FFF1FF1        把这个数字“当成”一个函数的地址
( … )(cmd_param, status_result)     调用这个函数，传两个参数（编译器会放进 r0、r1）
```

### 为什么调用前要关中断

📖 **UM p394，20.8.7 Interrupts during IAP**：

> The on-chip flash memory and EEPROM are not accessible during erase/write operations. … Before making any IAP call, either disable the interrupts or ensure that the user interrupt vectors are active in RAM and that the interrupt handlers reside in RAM. The IAP code does not use or disable interrupts.

IAP 写 EEPROM / Flash 时，Flash 暂时**不能读**。如果这时来了一个中断，CPU 要去 Flash 里取中断函数的指令，就会出错。所以调用前关中断，调用完再开。

（0-3 课讲过，向量表已经被复制到 RAM 0x1000 0000 了，但**中断函数本身还在 Flash 里**，所以仍然要关中断。）

### IAP 占用的 RAM

📖 **UM p394，20.8.9**：

> Flash programming commands use the top 32 bytes of on-chip local RAM. The maximum stack usage in the user allocated stack space is 128 bytes.

| 区域 | 地址 | 和 IAP 冲突吗 |
|---|---|---|
| IAP 用的顶部 32 字节 | 0x1000 1FE0 ~ 0x1000 1FFF | — |
| app 的 RAM（0-4 课） | 0x1000 0100 ~ 0x1000 1F5F | 不冲突 ✅ |
| 升级标志（0-3 课） | 0x1000 1F00 | 不冲突 ✅ |
| IAP 用的栈 | 在 app 的栈里，最多 128 字节 | app 栈有 0x200 = 512 字节 ✅ |

### EEPROM 读写命令

📖 **UM p411 Table 382**：61 = EEPROM Write，62 = EEPROM Read，58 = Read UID……

📖 **UM p416 Table 393 / 394**：

| | Write EEPROM | Read EEPROM |
|---|---|---|
| 命令号 command[0] | **61** | **62** |
| command[1] | EEPROM 地址 | EEPROM 地址 |
| command[2] | RAM 地址（数据从哪来） | RAM 地址（数据放到哪） |
| command[3] | 字节数 | 字节数 |
| command[4] | **系统时钟，单位 kHz** | **系统时钟，单位 kHz** |
| 说明 | **顶部 64 字节保留，不能写** | |

`src/IAP_CMD.c` 第 57~86 行的 `Chip_EEPROM_Write()` / `Chip_EEPROM_Read()` 就是按这张表填的数组：

```c
command[0] = LPC_IAP_EEPROM_WRITE;   // 61
command[1] = dstAdd;                 // EEPROM 地址
command[2] = (uint32_t) ptr;         // RAM 地址
command[3] = byteswrt;               // 字节数
command[4] = MCU_OSC / 1000;         // 48000000 / 1000 = 48000 kHz
```

**command[4] 用到了 1-2 课的 `MCU_OSC`**。IAP 需要知道当前 CPU 有多快，才能算出写 EEPROM 时要等多久。这就是为什么 `Param_Init()` 必须排在 `InitSystemOSC()` 之后：时钟切到 48 MHz 之后，48000 这个数才是对的。

**“顶部 64 字节保留”**：4 KB = 0x000~0xFFF，顶部 64 字节是 0xFC0~0xFFF，所以能用的是 **0x000~0xFBF**。`IAP_CMD.c` 第 48~50 行的注释也写了 “Range: 0x0 ~ 0xFBF”。

---

## 三、参数在 EEPROM 里怎么放

`src/my_protocol_mouse.c` 第 19~22 行：

```c
#define EEPROM_ADDR_START     0xF00 /*0xF00 ~ 0xFBF, 192Bytes*/
#define EEPROM_ADDR_VERSION   (EEPROM_ADDR_START + 0x00) //4bytes
#define EEPROM_ADDR_CHECKSUM  (EEPROM_ADDR_START + 0x04) //1bytes
#define EEPROM_ADDR_PARAM     (EEPROM_ADDR_START + 0x06)
```

参数结构体（`inc/my_protocol_common.h`，全部 `packed`，不留空隙）：

```
AllParam_ts xgAllParam（88 字节）
├─ xInfo[2]    左、右键各 16 字节：只读信息（原始值、基线、段位、线圈电压）
│   ├─ xRawInfo       6B   wCurrentRaw / wBaselineBottom / wBaselineTop
│   ├─ xSegmentInfo   4B   wCurrentSegmentValue / wMaxSegmentValue
│   └─ xCoilInfo      6B   wCoilVoltageMv0/1/2
└─ xConfig[2]  左、右键各 28 字节：可配置参数
    ├─ xRtCfg            8B   AP、RT 按下、RT 释放
    ├─ xCoilCfg          4B   按下时线圈通电时间
    ├─ xCoilReleaseCfg   4B   释放时线圈通电时间
    ├─ xCoilBrakeCfg     6B   按下制动：延时、时长
    └─ xCoilBrakeRelCfg  6B   释放制动：延时、时长
```

2 × 16 + 2 × 28 = **88 字节**。用编译产物核对：`List/Sample.map` 第 1441 行：

```
xgAllParam    0x100016ba   Data   88   my_protocol_mouse.o(.bss)
```

（注意 `CoilBrakeConfig_ts` 的注释写的是 “4bytes”，实际是 1+1+2+2 = 6 字节，注释错了，以 map 为准。）

EEPROM 布局：

```
EEPROM 地址
0x000 ┌──────────────────────────┐
      │ （gtech_flash.c 老配置系统用，1-17 课）│
      │                                       │
0xF00 ├──────────────────────────┤ EEPROM_ADDR_VERSION   4 字节  版本号
0xF04 │ 校验和                    │ EEPROM_ADDR_CHECKSUM  1 字节
0xF05 │ （空 1 字节）              │
0xF06 │ xgAllParam               │ EEPROM_ADDR_PARAM     88 字节
0xF5D ├──────────────────────────┤ 0xF06 + 88 - 1 = 0xF5D
      │ 空闲                      │
0xFBF ├──────────────────────────┤ 能写的最后一个字节
0xFC0 │ 保留 64 字节，不能写       │ 📖 UM p416 Table 393
0xFFF └──────────────────────────┘
```

---

## 四、`Param_Init()` 逐行（第 204~268 行）

### 第 209~211 行：算出“代码版本号”

```c
codeVer = (uint16_t)(MY_MAIN_VERSION);   // inc/my_debug.h:15  0x0011
codeVer <<= 16;
codeVer |= ((uint16_t)(DEBUG_VER));      // inc/my_debug.h:16  1121 = 0x0461
```

```
codeVer = 0x0011 << 16 | 0x0461 = 0x0011 0461
```

### 第 214~217 行：从 EEPROM 读出三样东西，并算校验和

```c
Chip_EEPROM_Read(EEPROM_ADDR_VERSION,  (uint8_t *)(&eepromVer), 4);          // 0xF00 读 4 字节
Chip_EEPROM_Read(EEPROM_ADDR_CHECKSUM, &eepromSum, 1);                        // 0xF04 读 1 字节
Chip_EEPROM_Read(EEPROM_ADDR_PARAM,    (uint8_t *)(&xgAllParam), sizeof(xgAllParam)); // 0xF06 读 88 字节
calSum = CalculateChecksum((uint8_t *)(&xgAllParam), sizeof(xgAllParam));
```

`CalculateChecksum()`（第 52~60 行）就是**把 88 个字节加起来，只留低 8 位**：

```c
uint8_t sum = 0;
for (i = 0; i < len; i++) sum += pData[i];   // uint8_t 加法，超过 255 自动绕回
```

### 第 219 行：判断 EEPROM 里的数据能不能用

```c
if(eepromVer != codeVer || (eepromSum != calSum))
```

两个条件，**任何一个不满足就重写默认值**：

| 条件 | 什么时候不满足 | 意义 |
|---|---|---|
| 版本号相同 | **新芯片**（EEPROM 是空的）；或者**换了新版本固件** | 结构体可能变了，旧数据的格式不对 |
| 校验和相同 | 数据被破坏（比如写到一半断电） | 数据不可信 |

> ⚠️ **注意**：只要 `MY_MAIN_VERSION` 或 `DEBUG_VER` 改了（哪怕结构体没变），升级固件后**用户在 GUI 里设的参数全部恢复默认**。改版本号前要想清楚这一点。

### 第 222~256 行：填默认值（只在上面的条件不满足时执行）

左右键（`i = 0, 1`）各填一遍，主要的默认值：

| 参数 | 默认值 | 宏定义位置 |
|---|---|---|
| 触发点 AP | 10 | `inc/my_misc.h:42` `DEFAULT_AP` |
| 快速触发按下 RTP | 30 | `inc/my_misc.h:43` |
| 快速触发释放 RTR | 20 | `inc/my_misc.h:44` |
| 按下 / 释放线圈通电时间 | 500 µs | `inc/my_electromagnet.h:26` |
| 制动 | 关闭；延时 10 µs、时长 10 µs | `inc/my_electromagnet.h:27~29` |
| 最大段位 | 100 | 第 228 行 |
| 线圈电压档 | 12000 / 15000 / 18000 mV | 第 229~231 行 |

`bmEnableMask` 的 bit7 是“这组参数有效”的开关（注释在 `my_protocol_common.h:99~106`）。

**`Protocol_ConvertWord()` 是干什么的？**（`src/my_protocol.c:11`）：

```c
uint32_t i = 1;
if(*(uint8_t*)&i)   // 看 i 在内存里的第一个字节是不是 1
    tmp = value;    // 小端机器：原样返回
else
    tmp = 高低字节交换;  // 大端机器：交换
```

它判断 CPU 是**小端**（低字节放在低地址）还是**大端**。Cortex-M0 是小端，所以这个函数**原样返回**，写不写效果都一样。它是为了让协议（规定用小端）在大端机器上也能用。

⚠️ 一个遗漏：**`xInfo` 里的 `xRawInfo`、`wCurrentSegmentValue` 没有填默认值**（第 224~227 行被注释掉了），它们保持着刚才从 EEPROM 读出来的内容（新芯片上可能是 0xFF 或 0），然后被一起写回 EEPROM。这些是运行时会被 `Hall_SyncParam()` 覆盖的“信息”字段，影响不大，但要知道它们开机时的值不可信。

### 第 260~263 行：写回 EEPROM —— 注意顺序

```c
calSum = CalculateChecksum((uint8_t *)(&xgAllParam), sizeof(xgAllParam));
Chip_EEPROM_Write(EEPROM_ADDR_PARAM,    (uint8_t *)(&xgAllParam), sizeof(xgAllParam)); // ① 先写参数
Chip_EEPROM_Write(EEPROM_ADDR_CHECKSUM, &calSum, 1);                                  // ② 再写校验和
Chip_EEPROM_Write(EEPROM_ADDR_VERSION,  (uint8_t *)(&codeVer), 4);                    // ③ 最后写版本号
```

**为什么版本号最后写？** 假设写到一半断电：

| 断在哪 | 下次开机看到的 | 结果 |
|---|---|---|
| ① 写参数时 | 版本号是旧的 | 判断失败 → 重新写默认值 ✅ |
| ② 写校验和时 | 版本号是旧的 | 同上 ✅ |
| ③ 写版本号时 | 版本号不完整 | 同上 ✅ |
| 全部写完 | 版本号、校验和都对 | 正常使用 ✅ |

版本号相当于“这份数据已经完整写好”的**完成标记**，必须最后写。这和 0-3 课 Bootloader 最后才写 0xFE00 那个 0xAA 标志是同一个思路。

**写 EEPROM 要多久？** 📖 **DS p48 Table 10**：

| 参数 | 值 |
|---|---|
| 写入时间 tprog | **2.9 ms / 64 字节**（典型值） |
| 寿命 | 至少 **10 万次** |
| 保存时间 | 至少 100 年（通电） |

参数 0xF06~0xF5D 跨了两个 64 字节页（0xF00~0xF3F、0xF40~0xF7F），校验和、版本号各在 0xF00 那一页再写一次，加起来大约 **4 次 × 2.9 ms ≈ 12 ms**，期间**中断全关**。只在第一次开机或换版本时发生一次，没问题。但在主循环里频繁写 EEPROM 就要小心了（2-13 课 `Task_ParamGuard()` 会看到怎么控制写入次数）。

### 第 266 行：把参数交给各个模块

```c
Init_CfgParamToRun();
```

`xgAllParam` 只是一份“存档”。真正干活的模块用的是自己的变量。`Init_CfgParamToRun()`（第 80~107 行）把存档分发出去：

```
xgAllParam.xConfig[0/1].xRtCfg            → Hall_UpdateParam()        → ap_value[]、rt_press_value[]、rt_release_value[]（my_misc.c，按键算法）
xgAllParam.xConfig[0/1].xCoilCfg          → Ele_UpdateParam()         → leftEle / rightEle.pressVibrateDurationUs
xgAllParam.xConfig[0/1].xCoilReleaseCfg   → Ele_UpdateReleaseParam()  → …releaseVibrateDurationUs
xgAllParam.xConfig[0/1].xCoilBrakeCfg     → Ele_UpdateBrakeParam()    → …brakeEnable / brakeDelayUs / brakeDurationUs
xgAllParam.xConfig[0/1].xCoilBrakeRelCfg  → Ele_UpdateRelBrakeParam() → …relBrakeEnable / …
```

1-4 课里 `Ele_LeftKeyPressProcess()` 用的 `leftEle.pressVibrateDurationUs`、`leftEle.brakeEnable`，就是在这里被赋值的。

例如 `Ele_UpdateParam()`（`src/my_electromagnet.c:427`）：bit7 有效时，通电时间超过 `MAX_VIBRATE_DURATION_US`（2000 µs）就截成 2000，起到**保护线圈**的作用；bit7 无效则通电时间为 0（不振）。

---

## 五、小结

```
Param_Init()
 ├─ codeVer = 0x00110461
 ├─ IAP 读 EEPROM：版本号(0xF00) 校验和(0xF04) 参数(0xF06, 88B)
 ├─ 版本号或校验和不对？
 │    └─ 是：填默认值 → 写参数 → 写校验和 → 最后写版本号（完成标记）
 └─ Init_CfgParamToRun()：把参数分发到按键算法和线圈模块
```

**三条要点：**

1. EEPROM 不在地址空间里，只能通过 **ROM 里的 IAP（0x1FFF1FF1）** 读写；调用前要**关中断**。
2. IAP 要知道系统时钟（kHz），所以必须在时钟初始化之后调用。
3. “版本号 + 校验和 + 版本号最后写”是一种简单可靠的**掉电保护**写法。

---

## 🧪 验证（零风险）

1. 用 `python hid_tool.py getcfg L` 和 `getcfg R` 读出左右键的当前配置。如果你从没在 GUI 里改过参数，看到的应该是本课第四节的默认值（AP=10、RTP=30、RTR=20、通电 500 µs）。
2. 在 `List/Sample.map` 里找到 `xgAllParam`、`Chip_EEPROM_Read`、`Chip_EEPROM_Write` 的地址和大小。

> ⚠️ 不要用 `setcoil` 或 GUI 把通电时间改得很大做实验。虽然代码会截到 2000 µs，但频繁大电流通电会让线圈发热。另外每次保存都会写 EEPROM，寿命虽然有 10 万次，也没必要反复测试。

---

## ❓ 练习

1. 如果 `Param_Init()` 被挪到 `InitSystemOSC()` 前面执行，`command[4]` 传进去的值是 48000，但实际 CPU 只有 12 MHz（IRC）。你觉得会有什么后果？（提示：IAP 用这个值算写入时要等多久）
2. 如果把第 260~263 行的顺序改成“先写版本号，再写参数”，写参数时断电，下次开机会发生什么？
3. EEPROM 能用的范围是 0x000~0xFBF。参数区从 0xF06 开始，最多还能再加多少字节的参数？
4. `codeVer` 在 EEPROM 0xF00~0xF03 里是怎么排的？写出这 4 个字节（提示：小端）。

---

## 补充问答

### Q1：r0、r1 是什么意思？

**① CPU 里面有一小组“寄存器”**

这里的“寄存器”和前面讲的外设寄存器（PDRUNCFG、TCR 这些，有地址、在 0x4000 0000 附近）**不是一回事**。这里说的是 **CPU 内核自己身上的寄存器**，没有地址，数量很少，是 CPU 做运算时直接用的“手边的草稿格子”。

📖 **UM p434~435，24.3.1.3 Core registers，Figure 75 / Table 413**：

```
R0  ┐
R1  │
R2  │  低寄存器（Low registers）
R3  │  ← 大部分指令只能用这 8 个
R4  │
R5  │                                    通用寄存器
R6  │                                    R0~R12 都是 32 位
R7  ┘                                    “R0-R12 are 32-bit general-purpose
R8  ┐                                     registers for data operations.”
 …  │  高寄存器（High registers）
R12 ┘
R13 = SP  栈指针（0-4 课讲的 __initial_sp 就是装进它）
R14 = LR  返回地址（函数调用完回到哪里）
R15 = PC  程序计数器（当前执行到哪条指令）
```

CPU 要算 `a + b`，必须先把 a、b 从内存读进 R0~R12 里的某两个，在寄存器里相加，再把结果写回内存。r0、r1 就是 R0、R1，反汇编里习惯写小写。

**② 函数调用时，参数放在 r0~r3**

C 语言调用函数时，参数怎么交给被调用的函数？ARM 有一个所有编译器都遵守的约定（叫 AAPCS，ARM 过程调用标准）：

| 用途 | 放在哪 |
|---|---|
| 第 1 个参数 | **r0** |
| 第 2 个参数 | **r1** |
| 第 3 个参数 | r2 |
| 第 4 个参数 | r3 |
| 第 5 个及以后 | 栈上 |
| 返回值 | **r0** |

📖 UM p411 也提到了这一点：“Up to 4 parameters can be passed in the r0, r1, r2 and r3 registers respectively (see the ARM Thumb Procedure Call Standard…)”。

所以 UM p410 说的 “IAP routine should be called with a word pointer in register r0 pointing to … command code and parameters … result … pointed to by register r1”，翻译成 C 就是：

```c
iap_entry(command, result);   // 第 1 个参数 command → r0，第 2 个参数 result → r1
```

写 C 代码时**不用自己管 r0、r1**，编译器会自动按约定把参数放进去。手册之所以提 r0、r1，是因为 IAP 是 ROM 里的程序，它只认这个约定。

**③ 在编译产物里亲眼看一次**

`Chip_EEPROM_Read(srcAdd, ptr, bytesrd)` 的反汇编（`output/Sample.axf`，0x6BE6 起）：

```
                                    ; 进来时：r0 = srcAdd, r1 = ptr, r2 = bytesrd（3 个参数）
6be6: push {lr}                     ; 保存返回地址
6be8: sub  sp, #0x2c                ; 在栈上开出空间放 command[5] 和 result[5]
6bea: movs r3, #0x3e                ; r3 = 62（EEPROM Read 命令号）
6bec: str  r3, [sp, #0x18]          ; command[0] = 62
6bee: str  r0, [sp, #0x1c]          ; command[1] = srcAdd   ← 第 1 个参数从 r0 来
6bf0: str  r1, [sp, #0x20]          ; command[2] = ptr      ← 第 2 个参数从 r1 来
6bf2: str  r2, [sp, #0x24]          ; command[3] = bytesrd  ← 第 3 个参数从 r2 来
6bf4: ldr  r0, [pc, #0x48]          ; r0 = 48000（0x6C40 处的常量 0xBB80）
6bf6: str  r0, [sp, #0x28]          ; command[4] = 48000
6bf8: add  r1, sp, #0x4             ; r1 = result 数组的地址      ┐ 准备调用 lpc_iap_entry
6bfa: add  r0, sp, #0x18            ; r0 = command 数组的地址     ┘ 的两个参数
6bfc: bl   lpc_iap_entry            ; 调用
6c00: ldr  r0, [sp, #0x4]           ; r0 = result[0]（状态码）
6c02: uxtb r0, r0                   ; 截成 uint8_t → 作为返回值放在 r0
```

`lpc_iap_entry`（0x6C32 起）：

```
6c34: cpsid i                       ; __disable_irq()，关中断（PRIMASK=1）
6c36: ldr   r2, [pc, #0xc]          ; r2 = 0x1FFF1FF1（0x6C44 处的常量）
6c38: blx   r2                      ; 跳到 ROM 里的 IAP；此时 r0 = command，r1 = result
6c3a: cpsie i                       ; __enable_irq()，开中断
```

`blx r2` 跳进 IAP 的那一刻，**r0 正好指向 command 数组，r1 正好指向 result 数组**，这就是手册要求的样子。IAP 从 r0 指的地方读命令和参数，把状态码写到 r1 指的地方。

（0x6C40 处的常量 = 0x0000 BB80 = 48000，0x6C44 处的常量 = 0x1FFF 1FF1，都已经从 `output/Sample.hex` 读出来核对过。）

**一句话总结**：r0、r1 是 CPU 内核里的第 0、1 号通用寄存器。按 ARM 的约定，函数的第 1、2 个参数放在 r0、r1 里。所以手册说“r0 指向命令表、r1 指向结果表”，在 C 里就是 `iap_entry(command, result)`，编译器会自动放好。
