# 轻松版课程

从零开始的图解课，每课一个 HTML 页面（用浏览器打开），10~15 分钟，可以拖、点、做小测验。
之前的 0-x、1-x、L1、L2 讲义保留，作为以后查细节的“字典”。

## 📚 从这里开始：学习目录

**https://claude.ai/artifact/9nXLjFSsDezEn5UDfeTqe8**（`学习目录.html`：分组、可搜索、可勾选已读、给出下一篇建议）

下面是同样内容的分组表格。

### 一、入门（第 0~5 课，按顺序读）

| 课 | 在线页面 |
|---|---|
| 第 0 课 这只鼠标的地图（含 24 课完整路线） | https://claude.ai/artifact/M2RnYCfavarnoiBEjJ9Eky |
| 第 1 课 电怎么传信息 | https://claude.ai/artifact/QAeSDfXisuYV89dGuqVqX2 |
| 第 2 课 0 和 1：一排开关 | https://claude.ai/artifact/HwMr9Ng8ocHS9MBvvJD1bX |
| 第 3 课 单片机是什么（外设七步法） | https://claude.ai/artifact/Df8iYXGpcqsQs45cVTXCQf |
| 第 4 课 程序怎么跑 | https://claude.ai/artifact/8avBhNLVJRzRPehvi6xtsN |
| 第 5 课 代码仓库怎么读 | https://claude.ai/artifact/DMHaAKpxifpv3aa1bAJoeM |

### 二、芯片外设（第 6~13 课）

| 课 | 在线页面 |
|---|---|
| 第 6 课 GPIO 与引脚配置 | https://claude.ai/artifact/NHfRyw19iDqMJRYwhLUfTw |
| 第 7 课 ADC | https://claude.ai/artifact/3CLVMetraYoANnCKhpJuC6 |
| 第 8 课 定时器 | https://claude.ai/artifact/RjnBzgLW6mYK5WRAY7Xviy |
| 第 9 课 SPI 与 PAW3950 | https://claude.ai/artifact/7HACF2oXVpWjSqS19giSyB |
| 第 10 课 USB | https://claude.ai/artifact/C5mHnxa8bSv88j9P2V7jvN |
| 　└ 配套：USB 接口与端点总表 | https://claude.ai/artifact/Axd33DZ2jkDBAo9C5vSKac |
| 第 11 课 EEPROM 与 IAP | https://claude.ai/artifact/Q5ub7uZ9fE8dAmVVgaJvoy |
| 　└ 配套：芯片存储地址分布 | https://claude.ai/artifact/JoG6GLLsaj4RPKbny4Ea4i |
| 第 12 课 看门狗、复位与掉电 | https://claude.ai/artifact/NhatmnbFU8ZA6ZF1UzLvws |
| 第 13 课 时钟、电源与低功耗 | https://claude.ai/artifact/2Kw9ZtbCBu6cax3WLFieuv |

### 三、读代码（先看“全程”，再看文件）

| 页面 | 在线页面 |
|---|---|
| **主线：一次左键点击（全程 + 26 个全局变量 + 24 个函数）** | https://claude.ai/artifact/QoPZuz6jMfgBqK7WrEJL29 |
| 模块：ADC 全串讲（每一行代码 + 寄存器变化） | https://claude.ai/artifact/KM5wmiR61CJWKNEk68Xnfs |
| 全程：按下 DPI 键的完整流程（17 步） | https://claude.ai/artifact/7yhiDgMLCiZYLqHzKXnRVy |
| 文件夹：app 文件夹四份代码 | https://claude.ai/artifact/DQeECLZbqjbizU3wtk2zHv |
| 文件：custom.c 完全讲透 | https://claude.ai/artifact/VZVKafNhxDA6watP5e6RMT |
| 函数：Button_GetState()（6 个键的快照） | https://claude.ai/artifact/BMsC1ca4aV6Wh5QRUoTySZ |
| 专题：从内存看一次 IO | https://claude.ai/artifact/6Z4TjTfoJmmkLUsBX27KDm |
| 思想：C 语言里的面向对象（用本项目代码举例） | https://claude.ai/artifact/1pZrKGuEwLFjCwAWh7nK8H |
| 早期版：函数讲解 Custom_Paramter_Init()（已并入 custom.c 完全讲透） | https://claude.ai/artifact/C2Pkz5eGpiTkeNkUvThqJJ |

### 四、速查与清单

| 页面 | 在线页面 |
|---|---|
| 引脚总表（每学完一课都要更新：`PINS` 里的“学过”和用法、`UPDATED`、`LOG`） | https://claude.ai/artifact/5PUa6UQrGCyzkQPpPrzByB |
| 代码来源清单（可筛选） | https://claude.ai/artifact/VPsyav56Zh3cacuF4xg6us |
| 学习目标与覆盖（标准、进度） | https://claude.ai/artifact/Fe6KbvbH6bcSansEMkZds1 |

### 五、早期讲义（GitHub 里直接看）

`L1_USB报文解剖`（https://claude.ai/artifact/Qvima6yiNCJCzXs62oqyEC）、`study_notes/` 下的 `L1_*.md`、`L2_*.md`、`0-3`、`0-4`、`1-1`~`1-14` 逐行精读、`03_T1A适配清单.md`、`02_芯片状态总表.md`。

**新增讲解页时**：同时更新 `学习目录.html`（页面脚本里的 `G` 数组）和上面对应分组的表格。

## 讲代码时的固定规则

讲到任何一段功能，都按三层分开讲：

| 层 | 是什么 | 你要做到 |
|---|---|---|
| 🟦 芯片自动做的 | 硬件本身的行为，设置好寄存器后自己运行（例：ADC 转换、USB 收发包、定时器计数） | 知道它会自动发生，不用写代码 |
| ⬜ 现成代码 | 分两种：① 芯片厂商 / 方案商例程（文件头有 NXP、Keil、ARM、World Peace：USB 协议栈、启动文件、LPC11Uxx.h、wnDrv_Pmu.c）；② 公司老项目代码（文件头 Writer ADAM 或在 M300S 老项目里也有：GPIO.c、Timer.c、SPI.c、OSC.c、device.c…）。注意老文件里也夹着本项目新加的函数和 #if 分支 | 知道怎么调用它、它给你什么结果 |
| 🟩 你要负责的代码 | 这个产品自己的逻辑，以后你会改（例：按键算法、线圈时序、组包、GUI 协议、引脚定义） | 每一行都要懂，能自己改 |

## 我对你学习状态的判断，以及据此定的讲法

- 你会认真追问每个没定义的词（“最低点”“学”“反转”“设置外设”）→ **每个新词第一次出现就用一句大白话定义（虚线框 + 绿色定义卡），课末列出“本课新词”**。
- 一页同时出现寄存器、文件行号、算法细节时你会卡住 → **一课只讲一件事；细节和边角情况不放进正课**。
- 你说地图那张图最有帮助，因为能看清“谁干了什么” → **每课开头标出“在地图上的位置”，讲代码时分 🟦 芯片自动 / ⬜ 现成代码 / 🟩 你负责**。
- 你没写过完整项目，学校的知识忘了 → **打地基阶段补电、0/1、单片机、程序结构、代码仓库怎么读，再进入外设**。
- 判断你懂没懂，靠小测验，不靠问“懂了吗”。

## 模块课的固定结构（成体系讲解）

每个外设 / 模块课都完整覆盖以下内容，不只讲片段：

1. 它是干什么的（原理 + 动手演示）
2. **生命周期图**：横轴是时间（开机准备 → 平时工作 → 中断 → 停止/出错），纵轴是谁在做（🟦 芯片 / ⬜ 例程 / 🟩 项目）；每个方块标真实执行顺序、所属七步、代码原文、文件行号、调用链
3. **七步核对**：① 通电 → ② 时钟 → ③ 解锁 → ④ 引脚 → ⑤ 配置 → ⑥ 中断 → ⑦ 使用，哪些有、哪些不需要、为什么；前提依赖（如 IOCON 时钟）也列出
4. **标志和残留**：哪些标志要清、谁清、在哪清；例程已经清了的明确标出
5. 项目里怎么用：谁调用、多久一次、结果交给谁
6. 容易出错的地方
7. 你能改什么
8. T1A 上的变化

**手册对照**：讲到每个寄存器 / 每个开关，都附上用户手册 UM10462（`materials/mcu_reference/LPC11U3x2x1x User Manual.pdf`）的章、表号、页码、英文原文和中文意思；电气参数、引脚表引用数据手册 `LPC11U3X.pdf`。

## 手册能力练习阶梯

- 第 3~5 课：页面直接给手册原文和页码
- 第 6~9 课：只给章、表、页，自己打开 PDF 看
- 第 10~13 课：只给寄存器名，先自己找到表，再看答案
- 第 14 课起：先看手册猜代码怎么写，再对照项目代码
- 外设课（第 6~13 课）都包含“手册整章导读”：按章节结构带着读完整一章，而不只是节选

## 接续说明（换新会话时先读这里）

- 学习者：实习生，没写过完整项目，学校的基础知识大多忘了；目标是独立接管本项目，熟悉项目代码和芯片手册，最终完成 T1A 适配。
- 讲法：中文、大白话、多图、可动手；一课只讲一件事；新词第一次出现就解释；每课有小测验和“本课新词”；外设课按“生命周期图 + 七步 + 标志由谁清 + 手册整章导读”成体系讲；区分 🟦 芯片自动 / ⬜ 现成例程 / 🟩 项目代码；手册引用写清章、表、页和原文。
- 约束：不和 STM32 比较；不做有变砖风险的操作；不单独讲 C 语言基础；不创建 CLAUDE.md。
- 进度：第 0~13 课已完成（芯片外设部分全部讲完）。**下一课：第 14 课 一次点击（把外设串起来）。手册练习方式从这一课起改为“先看手册猜代码，再看代码”。**
- 第 13 课发现的问题：进深度睡眠没按手册 3.9.4.2 第 4 步先切到 IRC（反而关了 IRC）；现成的 clockinit()（usbhw.c:628）做这件事但没人调用。晶振起振只等 256 次空循环（有 PLL 锁定兜底）。挂起时整机电流是否 < 500 µA 需实测。
- 第 12 课发现的问题：WDT_IRQHandler 里喂狗无效（WARNINT=0，中断与复位同时）；挂起时 BOD 被断电（wnDrv_Pmu.c:61、usbhw.c:644），醒来没人重新打开 → 睡过一次后 BOD 复位失效；看门狗约 65 s（振荡器 ±40%）；STARTERP1 注释与实际相反；复位原因没人记录；bootloader 的 SYSRSTSTAT |= 会清掉其他位。
- 更正：Flash 余量约 13.6 KB，不是 2.9 KB（代码到 0xC513，0xC514~0xEFFF 是链接器 PAD）。已改第 5 课、03_T1A适配清单.md、0-3 笔记和全景图。另证实：公司 bootloader 把 0x4000 起 0x200 字节拷到 RAM 0x10000000 并设 SYSMEMREMAP=1，所以工程 RAM 从 0x10000100 开始。
- 交付形式：讲解和说明一律做成 HTML 页面发链接（用户只在 GitHub / 链接里看），不另写给 VS Code 看的 Markdown。
- 不只讲技术：看到学习者在思路、判断、做事方法上有值得改的地方，直接指出（单独标成“💡 想法上”，说清理由，允许反驳），目标是提升判断力和思考方式，不只是知识。
- 讲本质：每个讲解页开头加“🧭 本质”一段，说清这件事在抽象层次里处于哪一层、本质上在做什么，再讲细节。
- 讲法调整（第 10 课答疑后）：先讲**场景**（什么时候发生、谁发谁收、谁解析），再讲芯片和代码；手册里的图表要先说清“它为什么出现、和课上哪个比喻对应”，不要突然甩出来。
- 第 11 课发现的问题：EEPROM 写入返回值没人检查；旧协议设置开机读回的代码被注释（app/custom.c:220~272）；Flash 擦写只关了 7 个中断（gtech_flash.c:135~141）；Chip_IAP_ReadUID 调试时越界。EEPROM 布局：旧代码 0x000~0xB84，新协议 0xF00~0xF5D，无重叠。
- 第 10 课发现的问题（待问杰哥 / 以后改）：`EP_3_CUSTION_OUT` / `EP_4_CUSTION_IN` 名字和 USB 方向相反；`Device_Report()` 等 600 µs 后柜子不空也照样写；`EPInOutReq` 的 EP4_OUT 标志没人读；`usb.c` 里 `Usb_Init()`、`check_usb_and_endpoint()` 没人调用；DEVCMDSTAT 的 _C 位靠 `|=` 顺手清。
- 页面贴代码一律用源代码原文，需要时另写“宏展开”和“调用链”。
- 页面源文件就是本目录下的 .html；旧讲义（0-x、1-x、L1、L2）在上一级目录，作为字典。

**删除了 21 个没用的 .c 和 11 个 .h**（10-28 删除，10-29 曾恢复，同日按要求再次删除）：都不在 Keil 工程里，也没有被任何文件 `#include`。删后 Keil 编译 0 错误 0 警告，程序大小和 hex 与删除前完全一致。清单见“代码来源清单”页面的“已删除的文件”一节，需要时可从 git 历史找回。`src/my_hallkey_func.c`、`src/my_i2c.c` 没删（T1A 可能用得上）。

**读 app 文件夹时发现的问题**（详见“app 文件夹四份代码”页面）：① DPI 键不改 PAW3950（`optical_set_dpi()` 被 `#if` 关掉），但每按一次写一次 EEPROM、给 GUI 发一包；② `Main_Mouse_SCROLL_UP/DN()` 写的 `g_Main_Setting.MouseInReport` 在 IRPT_V4 下没人发送；③ scroll.c 第 15 行注释的 up / down 和代码相反；④ optical_3325.c 只剩 `TimerDelayMsec()` 在用，却让整段约 720 字节留在 Flash 里；⑤ button.c 的 `Hall_*` 函数（霍尔芯片菊花链）对 T1A 的 GTRY164 有参考价值，先别删。

**读 custom.c 时新发现的问题**（详见“custom.c 完全讲透”页面）：① 老 GUI 切配置（0x05）、整体同步（0x01）后 RAM 不更新（`Custom_Load_ModeSetting()` 的读取被注释）；② 多媒体 / 热键 id 低 4 位直接当表下标，`consumer_a_table` 7 项、`consumer_b_table` 11 项，会越界；③ `inc/gloab.h:20` 的 `#pragma pack(1)` 没恢复，影响所有 include 它的文件；④ 开机组合键（右键 + 后侧键）在 IRPT_V4 下可能因 ADC 未初始化而失效，待上板验证。

**ADC 全串讲时新发现**：`MyADC_GetValue()` 超时返回 −2 的那条路（my_adc.c:113）既没停止转换、也没清 SEL 位，下一次读另一个通道时 SEL 会同时选中两个通道；主循环每圈调用的 `MyADC_Poll_Test()` 函数体是 `#if 0`，是空调用。
