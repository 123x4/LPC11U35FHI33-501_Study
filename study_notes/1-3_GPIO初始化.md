# 1-3 引脚初始化：`InitGPIO()`

> 📍 **位置**：`main()` → `Device_Init()`（`src/device.c:415`）→ **第 421 行 `InitGPIO() ;`**
> 📄 **代码**：`src/GPIO.c` 第 305~561 行（只执行 `IRPT_V4` 分支：第 310~324 行 + 第 478~518 行）；引脚定义在 `inc/my_board.h` 第 230~279 行；操作宏在 `inc/my_board.h` 第 328~338 行
> 📖 **手册**：UM p32 Table 24（SYSAHBCLKCTRL）；UM 第 7 章 IOCON —— p81 7.3 节（引脚功能/上下拉/模拟模式）、p84 Table 75（寄存器总表和复位值）、p87 Table 77（PIO0_1）、p89 Table 80/81（PIO0_4/PIO0_5）、p94~97 Table 87~90（PIO0_11~14）；UM 第 9 章 GPIO —— p164 Table 160（DIR）、p165 Table 164（PIN）、p166 Table 168/170（SET/CLR）；DS p11 6.2 节

---

## 一、一根引脚要设置两个地方

LPC11U35 的每一根引脚，要分两个模块来配置：

```
                ┌──────────────── IOCON（0x4004 4000）─────────────────┐
                │  每根引脚一个 32 位寄存器                             │
                │  FUNC  这根脚当什么用？ GPIO / ADC / SPI / 定时器 …  │
                │  MODE  内部上拉 / 下拉 / 都不接                       │
                │  ADMODE 数字 / 模拟                                   │
                │  OD    开漏                                           │
                └──────────────────────────────────────────────────────┘
                                         │  FUNC 选成 GPIO 之后
                                         ▼
                ┌──────────────── GPIO（0x5000 0000）──────────────────┐
                │  每个端口一组 32 位寄存器，1 位对应 1 根脚            │
                │  DIR   输入还是输出                                   │
                │  PIN   读引脚电平                                     │
                │  SET   写 1 → 输出高                                  │
                │  CLR   写 1 → 输出低                                  │
                └──────────────────────────────────────────────────────┘
```

📖 **UM p81，7.3.1 Pin function**：

> If the pins are GPIO pins, the DIR registers determine whether the pin is configured as an input or output. For any peripheral function, the pin direction is controlled automatically… The DIR registers have no effect for peripheral functions.

也就是说：**IOCON 决定“这根脚是谁的”，GPIO 决定“作为普通 IO 时是输入还是输出、电平是多少”**。

两个模块各要一个时钟开关：IOCON 的时钟上一课（OSC.c:55）已经打开了，GPIO 的时钟在本课第一行打开。

---

## 二、复位后每根脚是什么状态

📖 **UM p84 Table 75**：大部分引脚的 IOCON 复位值是 **0x0000 0090**。拆开看：

```
0x90 = 1001 0000
       │  │└┴┴── bit 2:0  FUNC   = 000
       │  └┴──── bit 4:3  MODE   = 10  → 上拉
       └──────── bit 7    ADMODE = 1   → 数字（带模拟功能的脚才有这一位）
```

📖 **DS p11，6.2 节**：

> All port pins have internal pull-up resistors enabled after reset except for the true open-drain pins PIO0_4 and PIO0_5.

📖 **UM p164 Table 160**：DIR 复位值 0 → **所有脚都是输入**。

所以复位后：**所有引脚 = 输入 + 内部上拉**。上拉的意思是芯片内部有一个电阻接到 3.3V，外面什么都不接时读到的是 1。

⚠️ **FUNC = 000 不一定是 GPIO**。对大部分脚，000 是 GPIO；但有几根脚的 000 是别的功能，后面第 312~321 行就是在处理这件事。

---

## 三、逐行讲解

### 第 310 行：打开 GPIO 相关的时钟

```c
LPC_SYSCON->SYSAHBCLKCTRL |= ((1<<6)|(1<<19)|(1<<23)|(1<<24)) ;
```

📖 **UM p31~32 Table 24**：

| 位 | 名字 | 复位值 | 作用 |
|---|---|---|---|
| 6 | GPIO | 0 | GPIO 端口寄存器（DIR/PIN/SET/CLR）的时钟 |
| 19 | PINT | 0 | GPIO 引脚中断寄存器的时钟 |
| 23 | GROUP0INT | 0 | GPIO 组中断 0 的时钟 |
| 24 | GROUP1INT | 0 | GPIO 组中断 1 的时钟 |

本课真正要用的只有 bit6。另外三个是给引脚中断用的，本课没有用到；GROUP0 中断会在 USB 挂起/唤醒（2-11 课）时用到。

到这里 SYSAHBCLKCTRL 已经打开的时钟（累计）：

| 位 | 谁打开的 |
|---|---|
| 0~5 | 复位值就是 1（SYS、ROM、RAM0、FLASHREG、FLASHARRAY、I2C） |
| 15 WWDT | 1-1 课 |
| 16 IOCON | 1-2 课 |
| 6、19、23、24 | 本课 |

### 第 314~321 行：把 4 根 JTAG 脚改成 GPIO

```c
LPC_IOCON->TDI_PIO0_11 &= ~0x07 ;  LPC_IOCON->TDI_PIO0_11 |= 0x01 ;
LPC_IOCON->TMS_PIO0_12 &= ~0x07 ;  LPC_IOCON->TMS_PIO0_12 |= 0x01 ;
LPC_IOCON->TDO_PIO0_13 &= ~0x07 ;  LPC_IOCON->TDO_PIO0_13 |= 0x01 ;
LPC_IOCON->TRST_PIO0_14 &= ~0x07 ; LPC_IOCON->TRST_PIO0_14 |= 0x01 ;
```

📖 **UM p94 Table 87**（TDI_PIO0_11，0x4004 402C）：

| FUNC | 功能 |
|---|---|
| **0x0** | **TDI**（JTAG）← 复位值 |
| 0x1 | PIO0_11（GPIO） |
| 0x2 | AD0 |
| 0x3 | CT32B0_MAT3 |

你之前自己在手册里查到过：**PIO0_13 上电默认是 TDO，PIO0_14 上电默认是 TRST**。这 4 根脚（PIO0_11~14）复位后都属于 JTAG，FUNC = 0 不是 GPIO，要写 **1** 才是 GPIO。

写法是固定的两步：

```
&= ~0x07   先把 bit 2:0 清成 000
|=  0x01   再写成 001
```

不能直接 `= 0x01`，否则会把 MODE、ADMODE 等其他位也一起清掉。

在 IRPT_V4 上，这 4 根脚是：

| 引脚 | 板上用途 | 本课之后 |
|---|---|---|
| PIO0_11 | DPI 键 | GPIO 输入 |
| PIO0_12 | 中键 | GPIO 输入 |
| PIO0_13 | LB_ADC（左键光电信号） | 先当 GPIO 输入，1-20 课 `MyADC_Init()` 改成 AD2 |
| PIO0_14 | RB_ADC（右键光电信号） | 先当 GPIO 输入，1-20 课改成 AD3 |

**为什么 PIO0_10 和 PIO0_15 被注释掉了？** 这两根是 **SWCLK / SWDIO**，也就是 SWD 调试接口（下载、单步调试用的就是它）。复位值 FUNC=0 就是 SWD 功能，代码特意**不改**，这样调试器还能连上。

### 第 478~518 行：IRPT_V4 分支

`inc/my_board.h` 第 21 行 `#define IRPT_V4`，所以 `#elif (defined(IRPT_V4))` 这一段被编译；其他板型的分支全部**不执行**。

先认识代码里用到的 6 个操作（`src/GPIO.c:563` 和 `inc/my_board.h:328~338`）：

| 写法 | 展开后 | 📖 手册 | 作用 |
|---|---|---|---|
| `DrvGPIOInput(port, pin)` | `DIR[port] &= ~(1<<pin)` | p164 Table 160 | 设为输入 |
| `IOCTRL_CfgAsOutput(port, pin)` | `DIR[port] \|= (1<<pin)` | p164 Table 160 | 设为输出 |
| `IOCTRL_HighLevel(port, pin)` | `SET[port] = (1<<pin)` | p166 Table 168 | 输出高 |
| `IOCTRL_LowLevel(port, pin)` | `CLR[port] = (1<<pin)` | p166 Table 170 | 输出低 |
| `IOCTRL_NoPull(x)` | `IOCON->x &= ~((1<<3)\|(1<<4))` | p87 Table 77 MODE | MODE = 00，不接上下拉 |
| `IOCTR_PushPull(x)` | `IOCON->x &= ~(1<<10)` | p87 Table 77 OD | OD = 0，推挽（不开漏） |

**SET / CLR 为什么不用 `|=`？** 📖 UM p166：SET 写 1 的位变高，**写 0 的位不受影响**；CLR 同理。所以直接用 `=` 只改一位，其他脚不会被误改。这比“读出来改一位再写回去”更快，也不会被中断打断出错。

下面按代码的分组逐段讲。

#### ① 按键：4 个输入（第 480~483 行）

```c
DrvGPIOInput(FRONT_BUTTON_PORT, FRONT_BUTTON_PIN);    // PIO0_1   前侧键
DrvGPIOInput(BACK_BUTTON_PORT, BACK_BUTTON_PIN);      // PIO0_23  后侧键
DrvGPIOInput(MIDDLE_BUTTON_PORT, MIDDLE_BUTTON_PIN);  // PIO0_12  中键
DrvGPIOInput(DPI_BUTTON_PORT, DPI_BUTTON_PIN);        // PIO0_11  DPI 键
```

DIR 复位值本来就是 0（输入），这 4 行是保险写法。

**MODE 没有改**，所以保持复位值“上拉”。按键的接法是：

```
   3.3V
    │
  [内部上拉电阻]
    │
    ├────── PIO0_x  ──▶ 读 PIN
    │
  ─┴─ 按键
    │
   GND

  没按：读到 1      按下：引脚被拉到 GND，读到 0
```

代码里读按键的地方也证实了这一点（`inc/button.h:149`）：

```c
#define BUTTON_PORT0 (U32)((~LPC_GPIO->PIN[0]) & (MIDDLE_BUTTON_MASK | DPI_CYCLE_BUTTON_MASK | K5_BUTTON_MASK | K4_BUTTON_MASK))
```

先取反 `~`，这样“按下 = 0”就变成了“按下 = 1”。

> 💡 **前侧键接在 PIO0_1 上**。0-1 课讲过：PIO0_1 在复位时是 ROM Bootloader 检查的 **ISP 引脚**（UM p390），复位那一刻是低电平就进 ISP。前侧键按下正好把 PIO0_1 拉低，所以**按住前侧键再插 USB，理论上会进入 ROM 的 ISP 模式**。这可能就是这块板的“救砖”方法。但能不能走 USB ISP，还要看 PIO0_3（USB_VBUS）有没有接到 USB 的 5V。**先问杰哥确认，暂时不要试。**

#### ② 光学传感器 PAW3950：MOTION 和 NCS（第 485~489 行）

```c
DrvGPIOInput(MOTION_PORT, MOTION_PIN) ;      // PIO0_2  输入，保持上拉
IOCTRL_NoPull(NCS);                          // PIO0_20 不接上下拉
IOCTRL_HighLevel(NCS_PORT, NCS_PIN);         //         先把输出值设为高
IOCTR_PushPull(NCS);                         //         推挽
IOCTRL_CfgAsOutput(NCS_PORT, NCS_PIN);       //         最后才切成输出
```

- **MOTION**：传感器检测到移动时，把这根线拉低通知 MCU。它是输入，保持上拉。
- **NCS**：SPI 的片选，**低电平 = 选中传感器**。平时要保持高。

**为什么先 SET 再设 DIR？** 这是本课最重要的一个写法细节。

📖 UM p166 Table 168：“Reading from these register returns the port's output bits, **regardless of pin directions**”。也就是说，引脚还是输入的时候，也可以先往 SET/CLR 里写好“将来要输出的值”。

```
错误顺序：DIR=输出 → SET=高
          DIR 一切换，引脚立刻输出“输出寄存器里的旧值” = 0（复位值）
          → NCS 会短暂变低 → 传感器以为被选中了（一个毛刺）

正确顺序：SET=高 → DIR=输出
          切成输出的那一刻，输出的就已经是高
```

后面所有输出脚都是这个 4 步固定套路：**不接上下拉 → 设好电平 → 推挽 → 切成输出**。

#### ③ 滚轮编码器：ZA、ZB（第 491~494 行）

```c
#if (ENABLE_WHEEL_SCAN)                   // inc/my_board.h:37 = 1，执行
DrvGPIOInput(ZA_PORT, ZA_PIN);            // PIO0_17
DrvGPIOInput(ZB_PORT, ZB_PIN);            // PIO0_18
#endif
```

滚轮的两路信号，输入 + 上拉。2-6 课讲怎么解码。

#### ④ 光电按键（IRPT）：IR_EN、LB_ADC、RB_ADC（第 496~503 行）

```c
IOCTRL_NoPull(IR_EN);
IOCTRL_HighLevel(IR_EN_PORT, IR_EN_PIN);   // PIO0_19 输出高 = 打开红外发射电源
IOCTR_PushPull(IR_EN);
IOCTRL_CfgAsOutput(IR_EN_PORT, IR_EN_PIN);

IOCTRL_NoPull(LB_ADC);                     // PIO0_13 不接上下拉
DrvGPIOInput(LB_ADC_PORT, LB_ADC_PIN);     //         输入
IOCTRL_NoPull(RB_ADC);                     // PIO0_14 不接上下拉
DrvGPIOInput(RB_ADC_PORT, RB_ADC_PIN);     //         输入
```

板名 IRPT 就是红外（IR）+ 光电管（Photo Transistor）。左右键不是机械开关，而是红外对管：按下按键会改变光电管接收到的光量，输出一个**模拟电压**，MCU 用 ADC 读这个电压来判断按下的深度（2-2~2-4 课的 AP/RT 算法就是处理它）。

- **IR_EN 输出高**：给红外发射管供电。
- **LB_ADC / RB_ADC 不接上下拉**：这两根要测模拟电压，内部上拉电阻会把电压往 3.3V 拉，测出来就不准了。所以这里先把上拉关掉。

📖 UM p82，7.3.7 Analog mode：真正切成模拟输入，要把 FUNC 改成 AD、ADMODE 改成 0，那是 1-20 课 `MyADC_Init()` 的事。这里只是先把它们放成“没有上下拉的输入”，相当于准备好。

#### ⑤ 线圈驱动：HP_EN、PWM1、PWM2（第 506~517 行）

```c
// HP_EN（PIO0_16）：输出低 = 关闭升压
IOCTRL_NoPull(HP_EN);  IOCTRL_LowLevel(HP_EN_PORT, HP_EN_PIN);
IOCTR_PushPull(HP_EN); IOCTRL_CfgAsOutput(HP_EN_PORT, HP_EN_PIN);
// PWM1（PIO0_4）：输出低
IOCTRL_NoPull(PWM1);   IOCTRL_LowLevel(PWM1_PORT, PWM1_PIN);
IOCTR_PushPull(PWM1);  IOCTRL_CfgAsOutput(PWM1_PORT, PWM1_PIN);
// PWM2（PIO0_5）：输出低
IOCTRL_NoPull(PWM2);   IOCTRL_LowLevel(PWM2_PORT, PWM2_PIN);
IOCTR_PushPull(PWM2);  IOCTRL_CfgAsOutput(PWM2_PORT, PWM2_PIN);
```

- **HP_EN 低**：先把线圈的升压芯片关掉。上电时先保证线圈不会乱动，下一课 `Ele_Init()` 再按顺序打开。
- **PWM1 / PWM2 低**：线圈两端的驱动信号，先都给低（不通电）。

⚠️ **这里有一个手册细节：PIO0_4 和 PIO0_5 是“真开漏”引脚**。

📖 **UM p89 Table 80**（PIO0_4，0x4004 4010）：

| 位 | 名字 | 复位值 | 说明 |
|---|---|---|---|
| 2:0 | FUNC | 000 | 0 = PIO0_4（**open-drain pin**）；1 = I2C SCL |
| 7:3 | — | 10000 | **保留位**（没有 MODE、HYS、INV） |
| 9:8 | I2CMODE | 00 | 00 = I2C 标准/快速模式；**01 = Standard I/O functionality** |
| 31:10 | — | — | 保留（**没有 OD 位**） |

这两根脚本来是给 I2C 用的，结构和其他脚不一样：

1. **没有上下拉、没有推挽**。`IOCTRL_NoPull` 清的 bit3、bit4，和 `IOCTR_PushPull` 清的 bit10，在这张表里都是**保留位**（复位值本来就是 0）。所以这两条宏对 PIO0_4/5 **什么也没做**，写了等于没写。
2. **真开漏 = 只能拉低，不能输出高**。输出“高”时引脚只是松开（悬空），要靠**外部上拉电阻**才能变成高电平。（📖 DS p11 6.2 节也说 PIO0_4/5 复位后**没有**内部上拉。）
3. 📖 Table 80 的 I2CMODE 说明里写：引脚当 GPIO 用时，应选 I2CMODE = 01（Standard I/O）。本项目没有改，保持 00。

GEN_2_H_V1 分支（GPIO.c:469）的注释是 “External pull-up present”（外部有上拉），说明写代码的人知道这件事。IRPT_V4 板上是否也有外部上拉、I2CMODE 保持 00 是否有影响，**列入问杰哥的问题**。

---

## 四、本课结束后，IRPT_V4 每根脚的状态

| 引脚 | 名字 | IOCON FUNC | 上下拉 | 方向 | 电平 | 作用 |
|---|---|---|---|---|---|---|
| PIO0_1 | FRONT_BUTTON | GPIO（复位值） | 上拉 | 输入 | — | 前侧键（也是 ISP 引脚） |
| PIO0_2 | MOTION | GPIO（复位值） | 上拉 | 输入 | — | PAW3950 运动中断 |
| PIO0_4 | PWM1 | GPIO（复位值） | 无（真开漏） | 输出 | 低 | 线圈驱动 |
| PIO0_5 | PWM2 | GPIO（复位值） | 无（真开漏） | 输出 | 低 | 线圈驱动 |
| PIO0_11 | DPI_BUTTON | **改成 GPIO** | 上拉 | 输入 | — | DPI 键 |
| PIO0_12 | MIDDLE_BUTTON | **改成 GPIO** | 上拉 | 输入 | — | 中键 |
| PIO0_13 | LB_ADC | **改成 GPIO** | **关闭** | 输入 | — | 左键光电（1-20 课改 AD） |
| PIO0_14 | RB_ADC | **改成 GPIO** | **关闭** | 输入 | — | 右键光电（1-20 课改 AD） |
| PIO0_16 | HP_EN | GPIO（复位值） | **关闭** | 输出 | 低 | 升压使能（关） |
| PIO0_17 | ZA | GPIO（复位值） | 上拉 | 输入 | — | 滚轮 A |
| PIO0_18 | ZB | GPIO（复位值） | 上拉 | 输入 | — | 滚轮 B |
| PIO0_19 | IR_EN | GPIO（复位值） | **关闭** | 输出 | **高** | 红外发射电源（开） |
| PIO0_20 | NCS | GPIO（复位值） | **关闭** | 输出 | **高** | PAW3950 片选（不选中） |
| PIO0_23 | BACK_BUTTON | GPIO（复位值） | 上拉 | 输入 | — | 后侧键 |
| PIO0_10 / PIO0_15 | SWCLK / SWDIO | SWD（复位值，不改） | — | — | — | 调试口 |

**还没出现的引脚**：PAW3950 的 SPI 三根线（PIO1_15、PIO0_21、PIO0_22）在 1-19 课 `PAW3950_Init()` 里配置；USB 的引脚在 1-10~1-13 课配置。

---

## 五、小结

```
第 310 行      打开 GPIO 时钟（IOCON 时钟上一课已开）
第 314~321 行  PIO0_11~14 复位后是 JTAG，FUNC 改成 1 才是 GPIO
               PIO0_10 / PIO0_15 是 SWD，故意不改，保留调试口
第 478~518 行  IRPT_V4 分支：
               输入脚：DIR=0，按键和编码器保持上拉，ADC 脚关上拉
               输出脚：不接上下拉 → 先设电平 → 推挽 → 最后切输出（防毛刺）
```

**三条规律：**

1. **配一根脚分两步**：IOCON 选功能和上下拉，GPIO 设方向和电平。
2. **FUNC = 0 不一定是 GPIO**：查每根脚自己的 IOCON 表。PIO0_10~15 的 0 是调试功能。
3. **每根脚的 IOCON 位定义不完全一样**：PIO0_4/5 是 I2C 脚，没有 MODE 和 OD。写代码前要查这根脚自己的那张表。

---

## 🧪 验证（零风险）

1. **按键电平**：`hid_tool.py watch` 时，分别按前侧键、后侧键、中键、DPI 键，观察对应的按键位是否变化。这证明了“输入 + 上拉 + 按下接地”的电路。
2. **ADC 脚不接上拉**：`watch` 输出里左右键的 ADC 原始值，不按时不会贴着最大值（如果上拉没关，会被往 3.3V 方向拉）。

> ⚠️ 不要尝试“按住前侧键插 USB”进 ISP，先等杰哥确认 PIO0_3 的接法和恢复方法。

---

## ❓ 练习

1. TDI_PIO0_11 的 IOCON 复位值是 0x90。执行完第 314~315 行后变成多少？写出二进制，并说出每一段的含义（对照 UM p94 Table 87）。
2. 如果第 489 行 `IOCTRL_CfgAsOutput(NCS…)` 和第 487 行 `IOCTRL_HighLevel(NCS…)` 调换顺序，会发生什么？对 PAW3950 有什么影响？
3. 如果 PIO0_4 的 PWM1 想输出高电平，芯片内部能做到吗？需要外部电路提供什么？（UM p89 Table 80、DS p11）
4. 为什么按键脚要保持上拉，而 LB_ADC / RB_ADC 要关闭上拉？

---

## 补充问答

（上课时的追问会补在这里。）
