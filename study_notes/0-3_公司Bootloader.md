# 0-3 公司 Bootloader

> 📍 **位置**：上电 → ROM Bootloader（0-1 已学）→ **公司 Bootloader（本课）** → 应用程序启动文件（0-4）→ `main()`
> 📄 **代码**：`M300S/MG380U_BootLoader/20190228_ReduceDiscriptor_issue/`（Keil 工程 `M800.uvproj`，Flash 范围 0x0000~0x3FFF）
> 📖 **手册**：UM p23 Table 7（SYSMEMREMAP）、p27~28 Table 16（SYSRSTSTAT）、p397~398 扇区表、p444~447 向量表、p494 Table 445（AIRCR）

---

> 📂 **文件位置说明**：本讲义里写的 `Src/...`、`Inc/...`、`Build/...`，都是相对于
> `M300S/MG380U_BootLoader/20190228_ReduceDiscriptor_issue/` 这个文件夹。
> 例如 `Src/Source.c` 的完整路径是 `M300S/MG380U_BootLoader/20190228_ReduceDiscriptor_issue/Src/Source.c`。
> 最方便的看法是用 Keil 打开这个文件夹里的 `M800.uvproj`，左侧工程树里就能找到所有文件。

---

## 第 1 步：先确认读的代码就是板上跑的那份

杰哥给的源码目录名带日期，不一定和板上的版本完全一样。核对方法：反汇编板上固件 `materials/Z_M300S FW Update/merge_firmware.bin` 的前 16KB，和源码编译出的 `Build/BootLoader_2018.axf` 对比。

结果：
- USB 身份 VID/PID 都是 **17EF:6155**（`Inc/usbdesc.h:39~40`），和你烧录时 Fw Update 工具里显示的一致；
- `CheckAP()` 函数在两份二进制里的地址相同（0x23C4），**逐条指令一致**；
- 它用到的常量（0x3F01、0x10001F00、0x40048000）在常量表里的位置也一致。

结论：**这份源码可以当作板上的 Bootloader 来读。**

## 第 2 步：Bootloader 的 `main()` 第一行 —— 调用 `CheckAP()`

> 调用处：`Src/main.c` 第 141 行（`main()` 从第 135 行开始）
> 函数定义：`Src/Source.c` 第 68 行

Bootloader 一启动，先调用 `CheckAP()`，判断两件事：**Flash 里有没有可用的应用程序？应用程序有没有要求“留在 Bootloader 升级”？**

### ① 检查 Flash 里的“应用程序有效”标志（第 73~88 行）

```c
APData = (char*)FLASH_DATA_BOOT_START;   // = 0x0000FE00（Inc/Source.h:13）
if (*APData != 0xAA)                     // 读 Flash 地址 0xFE00 的一个字节
{
    EraseFlash();                        // 不是 0xAA：没有可用的应用程序，擦掉应用区
}
else
{
    APDataFg = 1;                        // 是 0xAA：有可用的应用程序
    APStartAddress = (*(0x3F01) << 8) | *(0x3F02);   // 读应用起始地址（实际没用到，见第 4 步）
}
```

- **0xFE00 处的 0xAA**：Bootloader 每次升级成功后，会在这里写一个标志页（第 5 步）。有这个标志，说明上一次升级是完整的。
- `EraseFlash()` 擦的是 **扇区 4~15**（`Inc/WFLASH.h:109~110`）。📖 翻到 **UM p397~398 的扇区表**：每个扇区 4KB，扇区 4 从 0x0000 4000 开始，扇区 15 到 0x0000 FFFF 结束。正好是整个**应用程序区**，Bootloader 自己所在的扇区 0~3 不会被擦。

### ② 判断是不是“应用程序要求升级”（第 90~104 行）

```c
APData = (char*)RAM_DATA_ADDR_START;            // = 0x10001F00（RAM 里的一个字节）
SW_Reset_Value = LPC_SYSCON->SYSRSTSTAT;         // 读复位原因
if ((SW_Reset_Value & 1<<4) != 0)                // bit4 = 1：上一次是“软件复位”
{
    SW_Reset_Flag = 1;
    LPC_SYSCON->SYSRSTSTAT |= 1<<4;              // 写 1 清除这一位
}
if ((*APData == 0xAA) && (SW_Reset_Flag == 1))
    APRamFg = 1;                                 // 应用程序要求留在 Bootloader
else
    APRamFg = 0;
```

📖 翻到 **UM p27，Table 16：SYSRSTSTAT（地址 0x4004 8030）**。这个寄存器记录**上一次复位是什么原因引起的**：

| 位 | 名称 | 含义 |
|---|---|---|
| 0 | POR | 上电复位 |
| 1 | EXTRST | 外部复位引脚 |
| 2 | WDT | 看门狗复位 |
| 3 | BOD | 掉电检测复位 |
| 4 | SYSRST | **软件复位**（程序自己要求的复位） |

表里写着 *“Writing a one clears this reset”*：要清除某一位，就往这一位**写 1**（不是写 0）。代码里的 `|= 1<<4` 就是这个意思。

**为什么要同时检查两个条件？** 刚上电时 RAM 里的内容是随机的，0x10001F00 处有可能**碰巧**就是 0xAA。如果只看 RAM，鼠标偶尔会莫名其妙地卡在 Bootloader 里。再加上“必须是软件复位”这个条件，就排除了上电时的随机值。源码第 91 行的注释 `add by Martin 2019.3.1` 说明这个条件是后来补上的，很可能就是为了修这个问题。

## 第 3 步：Bootloader 的 `main()` 第二行 —— 决定去哪（`Src/main.c:144`）

```c
if ((APDataFg == 1) && (APRamFg == 0))   // 有应用程序，并且应用没有要求升级
{
    boot_jump();                          // 跳去执行应用程序
}
// 否则继续往下执行：初始化 USB，以 17EF:6155 的身份连上电脑，等待升级
```

## 第 4 步：`boot_jump()` 逐行（`Src/Source.c:128`）

这 6 行就是上一课提到的“M0 没有 VTOR，中断怎么跳进应用程序”的答案：

```c
__disable_irq();                                                 // ① 关中断
Prepare_Copy((uint32_t *)0x00004000, (uint32_t*)0x10000000, 512); // ② 复制应用程序的向量表到 RAM 开头
LPC_SYSCON->SYSMEMREMAP = 0x01;                                  // ③ 让 RAM 顶替地址 0
__enable_irq();                                                  // ④ 开中断
__set_MSP(*(uint32_t *)0x00004000);                              // ⑤ 设置栈指针
entry = *((volatile unsigned int *)(0x00004000 + 4));            // ⑥ 取应用的 Reset_Handler 地址
((void(*)(void))entry)();                                        //    跳过去
```

| 行 | 说明 | 📖 手册 |
|---|---|---|
| ① | 搬运向量表期间如果来了中断，CPU 会去读一张“搬了一半”的表，所以先关中断（Cortex-M0 的 PRIMASK 寄存器） | UM 第 24 章 |
| ② | 把应用程序 0x4000 开头的 512 字节复制到 RAM 0x1000 0000。向量表实际只有 192 字节（48 个向量 × 4） | UM p18 Figure 6：active interrupt vectors 到 0x00C0 |
| ③ | MAP = 0x1：User RAM Mode，中断向量改从 RAM 读 | **UM p23 Table 7** |
| ⑤ | 向量表第 0 项是**初始栈指针**，把 CPU 的栈设到应用程序要求的位置 | UM p444~447 |
| ⑥ | 向量表第 1 项是**复位入口 Reset_Handler**，跳过去就进入了应用程序的启动文件（下一课） | UM p444~447 |

**用真实数据验证**：板上固件 0x4000 处的前两个字是：
- 第 0 项 = **0x1000 1BF8**：应用程序的初始栈顶，在 SRAM0 里；
- 第 1 项 = **0x0000 7F1D**：应用程序的 Reset_Handler。它是**奇数**，最低位 1 表示 Thumb 代码（IAP 那一课讲过），真正的地址是 0x7F1C。

**一个细节**：② 复制了 512 字节，可应用程序的 RAM 从 0x1000 0100 开始，前 256 字节之后的部分会被覆盖。这没有问题：跳转之后，应用程序的启动代码会重新初始化自己的 RAM，把那部分覆盖回去；而向量表只占前 192 字节，不受影响。

## 第 5 步：如果不跳转 —— 升级流程（概要，阶段 13 详讲）

Bootloader 留下来后：初始化 USB → 以 **17EF:6155** 的身份连上电脑 → Fw Update 工具一行一行发送 hex → 写入 Flash。

收到 hex 的结束记录时（`Src/Source.c:360` 起），在 **0xFE00 写入一个 256 字节的标志页**：`[0xAA, 起始地址高, 起始地址低, 0x00, 0xC1, 0xAA, 0xAA, ...]`。

工具发送“退出”命令后（`Src/main.c:374` 起）：

```c
BootRAM_Data->D0 = 0x00000000;   // 清掉 RAM 里的 0xAA 请求标志
NVIC_SystemReset();              // 软件复位
```

复位后又回到第 2 步：这次 0xFE00 是 0xAA，RAM 标志已清零，于是跳转进入**新的**应用程序。

## 第 6 步：另一边 —— 应用程序怎么要求升级

你环境搭建时点击 Fw Update 的“更新”，应用程序收到命令后执行的是：

```c
// src/pc_communicate.c:951
case PC_CMD_BOOTLOADER:
    Ele_DisableVibrate();          // 先关掉线圈，防止复位过程中线圈一直通电
    Bootloadable_Load();
```

```c
// app/custom.c:26、29、76
uint8_t *VecDst = (uint8_t*)0x10001F00;
uint8_t FlashData = 0xAA;
void Bootloadable_Load(void)
{
    memcpy((uint8_t *)VecDst, &FlashData, 1);   // 往 RAM 0x10001F00 写 0xAA
    NVIC_SystemReset();                          // 软件复位
}
```

📖 `NVIC_SystemReset()` 是 CMSIS 提供的函数，它往 Cortex-M0 的 AIRCR 寄存器写 **SYSRESETREQ** 位，请求一次系统复位（**UM p494 Table 445**）。这种复位会让 SYSRSTSTAT 的 bit4 置 1，**RAM 内容保留**。所以 Bootloader 启动后能读到那个 0xAA。

## 全流程图

```
          ┌────────────────────── 上电 / 复位 ──────────────────────┐
          ▼                                                          │
   ROM Bootloader（0-1）── PIO0_1 为高 ──▶ 公司 Bootloader：CheckAP() │
                                               │                     │
                  0xFE00 不是 0xAA ◀───────────┼──▶ 0xFE00 是 0xAA     │
                  （没有可用应用）              │     │                │
                        │                      │     ▼                │
                  EraseFlash() 擦应用区         │  RAM 0x10001F00 = 0xAA│
                        │                      │  且上次是软件复位？     │
                        ▼                      │     │是        │否    │
               留在 Bootloader ◀───────────────┼─────┘          ▼     │
               USB 17EF:6155，等待升级          │          boot_jump() │
                        │                      │          进入应用程序  │
                  写入新程序 + 0xFE00 标志页     │               │      │
                  清 RAM 标志，软件复位 ─────────┼───────────────┼──────┘
                                               │               │
                                               │   应用收到 PC_CMD_BOOTLOADER：
                                               │   RAM 0x10001F00 写 0xAA，软件复位 ──┘
```

## 第 7 步：这对我们意味着什么

**① 救砖问题终于能回答准确了**

| 情况 | 结果 | 能不能用 Fw Update 救回来 |
|---|---|---|
| 升级到一半断电，0xFE00 标志还没写 | 下次上电，Bootloader 发现没有标志 → 擦掉应用区 → 留在 Bootloader | ✅ 能，重新升级即可 |
| 新程序有 bug，但**还能响应**升级命令 | 正常升级 | ✅ 能 |
| 新程序有 bug，**USB 起不来**或一启动就卡死 | 0xFE00 标志是完整的，Bootloader 每次都跳进这个坏程序 | ❌ 不能，只能走 ROM ISP（PIO0_1）或 SWD |

所以真正危险的修改，是**会让应用程序在 USB 初始化完成之前卡死**的修改。这和之前定的安全规则一致：不碰 `Device_Init()` 前半部分。

**② 应用程序的 Flash 空间快用完了 ⚠️**

0xFE00 开始的那一页是 Bootloader 的标志页，**应用程序不能超过 0xFE00**。但 Keil 里 IROM1 设置的是 0x4000 + 0xC000 = 0x10000，所以即使代码越界，**链接时也不会报错**。

以仓库里的 `Sample.hex` 为例，应用程序结束于 **0xF28B**，离 0xFE00 只剩 **0xB74 = 2932 字节（约 2.9KB）**。T1A 新板还要加 Hall（H6）、光学编码器、BQ21128 充电芯片的驱动，空间很可能不够。**【待确认：问杰哥】**

**③ 两个还没解开的小问题【待确认】**
- 应用程序在 Flash 0x3F00 处放了 `0xAA 0xAA`（`src/gtech_flash.c:24`），这是 Bootloader 的扇区。Bootloader 读 0x3F01、0x3F02 作为应用起始地址，但 `boot_jump()` 里用的是写死的 0x4000。这两个字节到底给谁用？
- `merge_firmware.bin` 只到 0xF26F，**不包含 0xFE00 的标志页**。如果量产时直接烧这个文件，第一次上电 Bootloader 会把应用区擦掉。量产流程是怎样的？

## 练习

1. 📖 UM p27 Table 16：**POR** 位的复位值是多少？刚上电时 SYSRSTSTAT 的 bit4 是 0 还是 1？据此说明，为什么上电时即使 RAM 里碰巧是 0xAA，鼠标也不会卡在 Bootloader。
2. 应用程序向量表第 1 项是 0x0000 7F1D，Reset_Handler 实际在哪个地址？下一课我们会在启动文件里找到它。
3. 如果有人把 `Bootloadable_Load()` 里的 `NVIC_SystemReset()` 换成“拔插 USB 重新上电”，升级还能进入 Bootloader 吗？为什么？

---

## 补充问答

**Q1：ROM 怎么判断 0x0000 处有没有有效程序？**
📖 UM p392 “Criterion for Valid User Code”：Flash 0x0000 开头的前 8 个字（向量表前 8 项）加起来必须等于 0。板上 Bootloader 的这 8 个字之和正好是 0，其中第 7 项 0xEFFFF095 就是专门用来凑这个和的。

**Q2：UM p18 Figure 6 最下面几格 Flash 怎么读？**
各型号的 Flash **都从 0x0000 0000 开始**，图上每一格只表示“比下一档型号多出来的部分”。标着 “64 kB on-chip flash (LPC11U35)” 的那一格（0xC000~0x10000）的意思是：LPC11U35 的 Flash 一直到这一格顶端 0x0001 0000 为止，所以范围是 0x0000 0000 ~ 0x0000 FFFF。
我们的分配：0x0000~0x3FFF Bootloader（16KB），0x4000~0xFFFF app（48KB，其中 0xFE00 起的一页是 Bootloader 的标志页）。

**Q3：Bootloader 占前 16KB，手册里有规定吗？**
没有，这是公司自己定的，写在 `M800.uvproj`（Flash 从 0x0 开始，大小 0x3FFF）、`Sample.uvproj`（Flash 从 0x4000 开始）和 `Inc/Source.h:25`（`application_address 0x00004000`）里。
但分界线的位置受手册约束：📖 UM p397~398，Flash 按 **4KB 扇区**擦除，分界线必须落在扇区边界上，否则升级 app 时擦扇区会顺带擦掉 Bootloader。0x4000 是扇区 4 的起点，Bootloader 占扇区 0~3，app 占扇区 4~15（`EraseFlash()` 擦的正是 4~15）。

**注意区分**：Figure 6 里的 “16 kB boot ROM”（0x1FFF 0000）是 NXP 固化在芯片 ROM 里的程序，和放在 Flash 0x0000 的公司 Bootloader 是两回事，大小一样只是巧合。
