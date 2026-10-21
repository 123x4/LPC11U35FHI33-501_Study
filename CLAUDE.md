# 给 Claude 的说明：这是一个“带新人学项目”的仓库

新会话开始时先读这份文件，再读 `study_notes/01_逐行精读路线图.md` 的进度表，然后从下一课继续。

## 学员情况
- 刚入职，有一些 STM32 基础，第一次接触这个项目和 LPC11U3x 芯片。
- 手上只有 **IRPT_V4 板**（`inc/my_board.h` 当前选的板型），**没有任何仪器**（没有示波器、逻辑分析仪、调试器）。能用的：Keil 编译、`materials/Z_M300S FW Update/Fw Update.exe` USB 烧录、`MouseTest.exe`、`study_notes/tools/hid_tool.py`。
- **不做任何可能让板子变砖的操作**。暂时不要建议修改 `Device_Init()` 前半部分（时钟、看门狗、USB 初始化）。

## 讲课方式（学员明确要求过）
- **从 `main()` 开始，按代码真正执行的顺序逐行讲**，进入函数就讲完再出来，不要跳着讲。路线和进度在 `study_notes/01_逐行精读路线图.md`。
- 每一行都要结合**芯片用户手册**（`materials/mcu_reference/LPC11U3x2x1x User Manual.pdf`，简称 UM，写 PDF 页码和表号）、数据手册（`LPC11U3X.pdf`，简称 DS）、原理图讲清楚：改了哪个寄存器的哪一位、手册哪一页、为什么这样写。
- 结论尽量用**编译产物里的真实数据**核对：`List/Sample.map`、`output/Sample.axf`（用 `llvm-objdump --triple=thumbv6m` 反汇编）、`output/Sample.hex`、`output/Sample.sct`。
- 用中文，说得通俗，一次只讲一个主题；先讲“这是干什么的”，再讲细节。学员说不懂时，从更基础的概念重讲。
- **不要和 STM32 对比。**
- 学员喜欢图：需要时可以画图（已有 `study_notes/0-3_启动与升级全景图.html`）。
- 每课讲义写成 `study_notes/<课号>_<主题>.md`，末尾放练习；学员追问的内容补进讲义的“补充问答”。讲完在路线图进度表里打勾。
- 所有改动提交并推送到分支 `claude/focused-ptolemy-tmjyrp`。

## 进度
- 已讲：0-1 ROM Bootloader、0-2 存储器映射、0-3 公司 Bootloader、0-4 启动文件（含 main）、1-1 看门狗。
- **下一课：1-2 `InitSystemOSC()`**（`src/device.c:420` 调用，定义在 `src/OSC.c:29`），对应 UM 第 3 章 p19~55（PLL 计算 p53~55）。

## 已经核实过的关键事实（不用重新查）
- 芯片 LPC11U35FHI33/501：64KB Flash、4KB EEPROM、SRAM0 8KB + SRAM1 2KB + USB RAM 2KB。Keil 里选的是 LPC11U37/501。
- Flash：0x0000~0x3FFF 公司 Bootloader（源码 `M300S/MG380U_BootLoader/20190228_ReduceDiscriptor_issue/`，USB 17EF:6155，已反汇编核对与板上一致）；0x4000 起 app（USB 1A81:2071）；0xFE00 是 Bootloader 的“app 有效”标志页（0xAA）。app 用到 0xF28B，**只剩约 2.9KB**。
- RAM（SRAM0）：0x1000 0000~00FF 是 Bootloader 复制过来的 app 向量表（SYSMEMREMAP=1）；app 用 0x1000 0100 起，栈 0x1000 19D8~1BD7（栈顶 0x1000 1BD8）；**只剩 904 字节**；0x1000 1F00 是“请求升级”标志；顶部 32 字节给 IAP。
- 进入 `main()` 时主时钟是 IRC 12MHz（没人改过时钟）。
- 看门狗：WDOSC 1.05MHz÷20÷4 = 13125Hz，TC=851955，超时约 65 秒（±40%）；WARNINT=0，所以中断里喂狗不会让看门狗失效。
- IRPT_V4 板：PAW3950 在 SSP1（PIO0_21/22、PIO1_15）；T1A 新板上 PAW3950 改到 SSP0、Hall 芯片改到 SSP1。

## 待问杰哥的问题
1. 看门狗超时约 65 秒是否有意为之；`WWDT.c:61` 注释是否写错。
2. app 放在 0x3F00 的两个 0xAA 字节有什么用；量产时 0xFE00 标志页怎么写入。
3. app Flash/RAM 余量很小，T1A 新板加功能怎么办。
4. 按住前侧键（PIO0_1）上电能否进入 ROM USB ISP（IRPT_V4 没有原理图，PIO0_3 是否接 VBUS 未确认）。

## 其他资料位置
- 项目需求：`materials/经理安排的项目(也是这个项目的项目需求).png`（适配 T1A 新板：H6=GTRY164 Hall 芯片、R-DIP 光学编码器、BQ21128 充电、PAW3950）。
- T1A 原理图：`materials/GT_Lina T1A Hall Haptic POC_SCH_V0.1.pdf`；GTRY164 手册在 `M300S/`。
- 按键算法 PC 仿真：`study_notes/sim/`。
