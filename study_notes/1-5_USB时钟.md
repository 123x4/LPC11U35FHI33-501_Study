# 1-5 USB 时钟：`InitUSBOSC(USB_SEL, USB_PSEL, USB_MSEL, USB_DIV_VAL)`

> 📍 **位置**：`main()` → `Device_Init()`（`src/device.c:415`）→ ~~第 424~427 行 `InitUART()`（`ENABLE_UART = 0`，**不执行**）~~ → **第 429 行 `InitUSBOSC( USB_SEL , USB_PSEL , USB_MSEL , USB_DIV_VAL) ;`**
> 📄 **代码**：`src/OSC.c` 第 58~74 行；参数在 `inc/OSC.h`
> 📖 **手册**：UM p21~22 Table 5（寄存器总表，含 “Reset value after boot” 一列）；p25 Table 11/12（USBPLLCTRL / USBPLLSTAT）；p29 Table 19（USBPLLCLKSEL）；p30 Table 20（USBPLLCLKUEN）；p34~35 Table 28/29（USBCLKSEL / USBCLKUEN）；p35 Table 30（USBCLKDIV）；p44 Table 47（PDRUNCFG）；p54 3.10.3 节（改 PLL 参数的推荐顺序）

---

## 零、先更正前两课的一个错误

讲这一课时我查 UM p21~22 **Table 5**，发现它有两列复位值：

| 列名 | 意思 |
|---|---|
| Reset value | 芯片刚复位时的值 |
| **Reset value after boot** | **ROM Bootloader 跑完、准备跳到用户程序时的值** |

0-1 课讲过，芯片上电先跑 ROM Bootloader；0-3 课讲过，公司 Bootloader 的正常路径是 `CheckAP()` 之后直接 `boot_jump()`，**不碰时钟**（我核对了它的启动文件，没有调用 `SystemInit`）。所以 **app 进入 `main()` 时，寄存器是“after boot”那一列的值**，不是“复位值”那一列。

两列不同的寄存器里，和我们有关的是：

| 寄存器 | 复位值 | boot 之后 | 差别 |
|---|---|---|---|
| PDRUNCFG | 0xEDD0 | **0xEDF0** | bit5 SYSOSC_PD = 1：**晶振被 ROM 关掉了** |
| SYSAHBCLKCTRL | 0x3F | **0x0800 485F** | GPIO(6)、SSP0(11)、USB(14)、USBRAM(27) 已开；I2C(5) 已关 |
| USBCLKDIV | 0 | 1 | （Table 30 本身写的复位值就是 1） |

所以：

- **1-2 课第 35 行** `PDRUNCFG &= ~(1<<5)` **真正给晶振上了电**，不是我之前说的“保险写法”。
- **1-3 课第 310 行**里 GPIO 的时钟（bit6）其实已经开着了。

两份讲义里都已经加了勘误。这也是 1-2 课练习第 4 题的答案：两个值差在 bit5，说明 ROM Bootloader 跑完后把晶振关了。

> 💡 教训：看寄存器的初始值时，要想清楚“我的代码开始执行时，前面已经有谁动过它”。

---

## 一、USB 为什么要单独一路时钟

📖 UM p24，3.5.5 节：

> The USB PLL is identical to the system PLL and is used to provide a dedicated clock to the USB block…
> Remark: The USB PLL must be connected to the system oscillator for correct USB operation.

USB 全速通信是 12 Mbit/s，USB 控制器内部要用**准确的 48 MHz**（USB 规范要求全速设备的时钟误差在 ±0.25% 以内）。IRC 的精度是 ±1%（DS p49），达不到，所以 USB 时钟**必须来自晶振**。

芯片专门给 USB 配了一个**和系统 PLL 一模一样的 USB PLL**：

```
                    ① PDRUNCFG bit8 上电
外部晶振 12MHz ──▶ [选择器 USBPLLCLKSEL=1] ──▶ [ USB PLL ×4 ] ──▶ 48MHz ─┐
（1-2 课已上电）     ② UEN 0→1 生效              ③ USBPLLCTRL=0x23        │
                                                  ④ 等 USBPLLSTAT 锁定    │
                                                                          ▼
main clock 48MHz ────────────────────────────▶ [选择器 USBCLKSEL=0] ──▶ [÷ USBCLKDIV=1] ──▶ usb_clk 48MHz
                                                ⑤ 选 USB PLL                ⑥                 │
                                                                                               ▼
                                                     ⑦ PDRUNCFG bit10 上电 ──▶ [ USB 收发器 PHY ] ── D+/D- ── 电脑
```

**两条路都能得到 48 MHz**：USB PLL 输出，或者直接用主时钟（本项目主时钟也正好是 48 MHz）。本项目选的是 USB PLL 这一路。

---

## 二、参数

`device.c:429` → `inc/OSC.h`：

| 参数 | 宏 | 值 | 去向 |
|---|---|---|---|
| USBSEL | `USB_SEL` | 1 | ⚠️ **函数里根本没用这个参数**，第 64 行写死了 0x01 |
| USBPSEL | `USB_PSEL` | 1 | USBPLLCTRL bit 6:5 → P = 2 |
| USBMSEL | `USB_MSEL` | 3 | USBPLLCTRL bit 4:0 → M = 4 |
| USBDiv | `USB_DIV_VAL` | 1 | USBCLKDIV |

和系统 PLL 完全相同的配置：12 MHz × 4 = **48 MHz**，FCCO = 2 × 2 × 48 = **192 MHz**（📖 UM p55 Table 52 第一行）。

---

## 三、逐行讲解（`src/OSC.c` 第 58~74 行）

进入函数时（boot 之后的值）：

| 寄存器 | 值 | 含义 |
|---|---|---|
| PDRUNCFG bit8 USBPLL_PD | 1 | USB PLL 断电 |
| PDRUNCFG bit10 USBPAD_PD | 1 | USB 收发器断电 |
| USBPLLCLKSEL | 0 | USB PLL 输入 = IRC |
| USBPLLCTRL | 0 | M = 1，P = 1 |
| USBCLKSEL | 0 | USB 时钟 = USB PLL 输出 |
| USBCLKDIV | 1 | ÷1 |

### 第 60 行：给 USB 收发器上电

```c
LPC_SYSCON->PDRUNCFG &= ~(1 << 10) ;   // USBPAD_PD = 0
```

📖 **UM p44 Table 47**：bit10 **USBPAD_PD**，“USB transceiver power-down configuration”。0 = 上电，1 = 断电（挂起模式）。

收发器（PHY）就是芯片里直接驱动 USB_DP / USB_DM 两根线的模拟电路。

### 第 61 行：给 USB PLL 上电

```c
LPC_SYSCON->PDRUNCFG &= ~(1 <<  8) ;   // USBPLL_PD = 0
```

📖 **UM p44 Table 47**：bit8 **USBPLL_PD**。

⚠️ **顺序问题**：1-2 课的第三条规律是“改 PLL 参数：先断电改参数，再上电等 LOCK”（📖 UM p54 “Changing the divider values”）。系统 PLL 那一课是按这个顺序写的；这里却是**先上电（第 61 行），再选输入（第 64 行），最后改参数（第 69 行）**。

上电那一刻 USB PLL 的状态是：输入 = IRC 12 MHz，M = 1，P = 1 → FCCO = 2 × 1 × 12 = 24 MHz，**远低于 156 MHz 的下限**，PLL 处在一个不合法的状态。之后再在 PLL 运行中切输入、改参数——这正是手册说“不建议”的做法。

实际上能正常工作，因为最后第 70 行会等到 LOCK=1 才往下走，而 USB 控制器此时还没开始用这个时钟。但更规范的顺序应该是：

```
选输入（USBPLLCLKSEL + UEN） → 写 USBPLLCTRL → 再给 PLL 上电 → 等 LOCK
```

### 第 63~67 行：USB PLL 的输入选晶振

```c
LPC_SYSCON->USBPLLCLKSEL = 0x01 ;   // 系统振荡器（晶振）
LPC_SYSCON->USBPLLCLKUEN = 0x01 ;
LPC_SYSCON->USBPLLCLKUEN = 0x00 ;
LPC_SYSCON->USBPLLCLKUEN = 0x01 ;   // 0→1 生效
while (!(LPC_SYSCON->USBPLLCLKUEN & 0x01)) ;
```

📖 **UM p29 Table 19**（USBPLLCLKSEL，0x4004 8048）：

| SEL | 输入 | 说明 |
|---|---|---|
| 0 | IRC | ← 复位值。“The IRC is suitable for low-speed USB operation.” |
| **1** | **System oscillator** | 全速 USB 必须选这个 |

📖 **UM p30 Table 20**（USBPLLCLKUEN，0x4004 804C）：和 1-2 课一样的 **0→1 生效** 规则。注意它的复位值是 **0**（系统 PLL 的 UEN 复位值是 1），所以第一句 `= 0x01` 在这里其实就已经产生了一次 0→1，后面两句又做了一次，多做一次也没关系。

📖 UM p29 3.5.13 节 Remark：

> For USB operation, the clock source must be switched from IRC to system oscillator with both the IRC and the system oscillator running.

切换时 IRC 和晶振都在跑吗？IRC 一直开着；晶振在 1-2 课第 35 行上电，而且 1-2 课等到系统 PLL 锁定，说明晶振已经稳定。✅

这里也说明了为什么 `InitUSBOSC()` **必须排在 `InitSystemOSC()` 后面**：晶振是在 1-2 课才打开的（见第零节的勘误）。

### 第 69~70 行：设倍频，等锁定

```c
LPC_SYSCON->USBPLLCTRL = ((USBPSEL<<5) | USBMSEL) ;   // 0x23
while (!(LPC_SYSCON->USBPLLSTAT & 0x01)) ;            // 等 LOCK
```

📖 **UM p25 Table 11**（USBPLLCTRL，0x4004 8010）：位定义和 SYSPLLCTRL 完全一样。MSEL=3 → M=4，PSEL=1 → P=2。

📖 **UM p25 Table 12**（USBPLLSTAT，0x4004 8014）：bit0 LOCK。

和 1-2 课一样：晶振如果坏了，这里永远等不到 LOCK。不过晶振坏了的话，程序在 1-2 课第 46 行就已经卡住了，走不到这里。

### 第 72 行：USB 时钟来源选 USB PLL

```c
LPC_SYSCON->USBCLKSEL = 0x00 ;   // 0 = USB PLL 输出；1 = 主时钟
```

📖 **UM p34 Table 28**（USBCLKSEL，0x4004 80C0）：

| SEL | 来源 |
|---|---|
| **0** | **USB PLL out** ← 复位值 |
| 1 | Main clock |

📖 UM p34 3.5.22 节：“The USBCLKUEN register must be toggled from LOW to HIGH for the update to take effect.”

代码**只写了 SEL，没有翻转 USBCLKUEN**（📖 p34~35 Table 29），所以这一行严格说**不会触发切换**。但复位值本来就是 0（USB PLL），选择器一直就在 USB PLL 这一路上，所以结果正确。

如果以后有人想改成用主时钟（写 1），只写这一行是不够的，必须再加上 `USBCLKUEN = 0; USBCLKUEN = 1;`。

### 第 73 行：不分频

```c
LPC_SYSCON->USBCLKDIV = USBDiv ;   // = 1
```

📖 **UM p35 Table 30**（USBCLKDIV，0x4004 80C8）：0 = 关闭 USB 时钟；1 = ÷1；…；255 = ÷255。

48 MHz ÷ 1 = **48 MHz**，正好是 USB 要的频率。

---

## 四、编译器的一个小技巧（看反汇编时会遇到）

第 60、61 行的反汇编：

```
4996: ldr  r0, [pc, #0x6c]    ; r0 = 0x4004 8200（PDRUNCFG 在 r0 + 0x38）
4998: ldr  r4, [r0, #0x38]    ; 读 PDRUNCFG
499a: asrs r5, r0, #0x14      ; r5 = 0x40048200 >> 20 = 0x400 = (1<<10) !
499c: bics r4, r5             ; 清 bit10
499e: str  r4, [r0, #0x38]
49a0: ldr  r4, [r0, #0x38]
49a2: asrs r5, r0, #0x16      ; r5 = 0x40048200 >> 22 = 0x100 = (1<<8) !
49a4: bics r4, r5             ; 清 bit8
49a6: str  r4, [r0, #0x38]
```

编译器发现 **基地址右移 20 位正好是 0x400，右移 22 位正好是 0x100**，就直接拿地址算出掩码，省掉了一条装常量的指令。这不是 bug，只是编译器在省空间。以后看反汇编遇到“奇怪的移位”，可以先想想是不是这种技巧。

---

## 五、这一课没做、后面会做的事

USB 要真正工作，还需要：

| 还缺什么 | 在哪做 |
|---|---|
| SYSAHBCLKCTRL bit14 USB 寄存器时钟、bit27 USB RAM | `src/usbhw.c:129`（1-10 课）。boot 之后其实已经是 1 |
| USB_CONNECT 引脚、USB 中断、端点、描述符 | 1-10 ~ 1-13 课 |

**挂起时会关掉晶振**：`src/usbhw.c:644` 在进入 Deep-sleep 前执行 `PDRUNCFG |= (1<<7 | 1<<5 | 1<<3)`，把系统 PLL、晶振、BOD 都断电了。所以唤醒时（`usbhw.c:708`）要重新调用 `InitSystemOSC()`。这也回答了 1-2 讲义第九节的疑问：**唤醒时系统 PLL 是断电的，不存在“运行中改参数”的问题**。细节在 2-11 课。

---

## 六、小结

```
InitUSBOSC()
 ├─ USB 收发器上电（bit10）
 ├─ USB PLL 上电（bit8）          ⚠️ 比手册推荐的顺序早了
 ├─ USB PLL 输入 = 晶振，UEN 0→1    必须在 InitSystemOSC 之后（晶振那时才上电）
 ├─ M=4, P=2 → 48MHz，等 LOCK
 ├─ USBCLKSEL = 0（USB PLL）       没翻 USBCLKUEN，但复位值本来就是 0
 └─ USBCLKDIV = 1                  usb_clk = 48MHz
```

**一条新规律**：看寄存器初始值，要看 **UM Table 5 的 “Reset value after boot”** 那一列，因为 ROM Bootloader 已经改过一些寄存器。

---

## 🧪 验证（零风险）

- **USB 能枚举，就证明这一课全部正常**：`hid_tool.py list` 能看到设备。
- 对照反汇编：`llvm-objdump -d --triple=thumbv6m --start-address=0x4994 --stop-address=0x49d6 output/Sample.axf`，找到 USBPLLCLKSEL（偏移 0x48）、USBPLLCTRL（0x10）、USBCLKSEL（0xC0）、USBCLKDIV（0xC8）这几次写入。

> ⚠️ 不要改这里的参数做实验。USB 时钟一错，USB 就断了，就不能再用 USB 升级程序。

---

## ❓ 练习

1. 如果想**不用 USB PLL**、直接用主时钟给 USB，需要改哪几行？改完之后能把 USB PLL 断电吗？（📖 UM p34 3.5.22 节 Remark）
2. 如果把 `InitUSBOSC()` 挪到 `InitSystemOSC()` 前面执行，会发生什么？程序会卡在哪一行？（提示：第零节的勘误）
3. 把 SYSAHBCLKCTRL 的 boot 之后的值 0x0800 485F 写成二进制，对照 UM p31~32 Table 24，说出每个为 1 的位是什么。为什么 ROM 要把 USB 相关的时钟打开？（提示：ROM 自己也能走 USB ISP，0-1 课）
4. 第 72 行没有翻转 USBCLKUEN。为什么在这个项目里结果仍然正确？

---

## 补充问答

### Q1：USB 收发器是个什么东西？

**① 先看 USB 线里有什么**

USB 线里有 4 根线：

| 线 | 作用 |
|---|---|
| VBUS（5V） | 电脑给鼠标供电 |
| GND | 地 |
| **D+（USB_DP）** | 数据线 |
| **D-（USB_DM）** | 数据线 |

鼠标和电脑之间所有的数据（按键、移动、和 GUI 的通信），都只走 D+、D- 这两根线。

**② 芯片里的 USB 模块分三层**

📖 **UM p222，Figure 20（USB block diagram）** 和下面的文字：

```
  你的代码（usbhw.c 等）
        │ 读写 USB 寄存器、往 USB RAM 里放数据
        ▼
┌─────────────────────────────────────────────┐
│ 寄存器接口 + DMA        （数字电路）          │  管理端点、缓冲区
├─────────────────────────────────────────────┤
│ SIE 串行接口引擎         （数字电路）          │  把字节打包成 USB 数据包：
│                                              │  加同步头、加 CRC 校验、并行转串行…
├─────────────────────────────────────────────┤
│ ATX 收发器               （模拟电路）  ← 就是它 │  把 0/1 变成 D+/D- 上的真实电压，
│                                              │  也把线上的电压变回 0/1
└─────────────────────────────────────────────┘
        │                 │
     USB_DP            USB_DM      ── 线 ── 电脑
```

📖 UM p222 原文：

> The USB Device Controller has a built-in analog transceiver (ATX). The USB ATX sends/receives the bi-directional USB_DP and USB_DM signals of the USB bus.

打个比方：

- **SIE** 像写信的人：把要说的话写好、装进信封、封口（打包、加校验）。
- **收发器（ATX）** 像嘴巴和耳朵：把信“念出来”变成声音（电压）传到线上，也负责“听”电脑发过来的声音。

“收发器”这个名字就是这么来的：**收**（接收）+ **发**（发送）。英文 transceiver = transmitter（发送器）+ receiver（接收器）。

**③ 它具体做什么**

USB 用的是**差分信号**：D+ 和 D- 总是一高一低，用“谁高谁低”来表示 0 和 1。

```
D+  ‾‾‾‾\____/‾‾‾‾‾‾‾‾\____
D-  ____/‾‾‾‾\________/‾‾‾‾
```

两根线同时受到干扰时，会一起被抬高或拉低，但“谁高谁低”不变，所以抗干扰能力强。

收发器负责：

- **发送**：按 SIE 给的 0/1，用足够的驱动能力把 D+/D- 推成一高一低，12 Mbit/s 的速度。
- **接收**：比较 D+ 和 D- 谁高，还原出 0/1 交给 SIE。
- **识别总线状态**：比如两根线都被拉低一段时间，表示电脑在发“复位”；线上长时间没有任何活动，表示电脑要进入“挂起”（睡眠）。

这些都是**模拟电路**，要消耗电流，所以芯片给它单独配了一个电源开关，就是 PDRUNCFG 的 bit10。

**④ 为什么要能关掉它**

📖 UM p44 Table 47 里 bit10 写的是 “USB transceiver powered down (**suspend mode**)”。

电脑睡眠时，USB 进入挂起状态，USB 规范对挂起时设备的耗电有严格上限。这时把收发器断电，可以省下不少电流。2-11 课讲挂起时会看到代码怎么处理。

复位后（包括 boot 之后）这一位是 1，也就是收发器默认**断电**。所以第 60 行必须先把它打开，USB 才可能收发数据。

**⑤ 顺带一个预告：电脑怎么知道鼠标插上了？**

📖 UM p222~223，11.4.3 SoftConnect：

> The connection to the USB is accomplished by bringing USB_DP (for a full-speed device) HIGH through a 1.5 kOhm pull-up resistor.

全速设备要在 **D+ 上接一个 1.5 kΩ 上拉电阻到 3.3V**。电脑看到 D+ 被拉高，才知道“有个全速设备插上了”，然后开始和它通信（枚举）。

芯片可以用 **USB_CONNECT** 引脚（PIO0_6）控制一个开关，决定什么时候把这个上拉电阻接上。这样代码可以先把初始化做完，再“告诉”电脑自己插上了。这部分在 1-10 课讲 USB 初始化时细讲。

**一句话总结**：USB 收发器就是芯片里直接连着 D+/D- 两根线的那块模拟电路，负责把数据变成线上的电压发出去、把线上的电压收回来变成数据。它默认是断电的，第 60 行把它打开。
