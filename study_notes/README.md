# 学习资料目录

所有讲解都是网页（点链接在浏览器里看，可以拖、点、做题）。网页的源文件就在下面各个文件夹里。

**现在在学：T1A 霍尔板（杰哥 2026-11-03 的代码 + 原理图 `materials/GT_Lina T1A Hall Haptic POC_SCH_V0.1.pdf`），完全按这块板子学，不对比旧板型。** 路线：全景地图 → 一次左键点击全程 → 霍尔芯片和 SPI1 → 按键算法 → PWM → 波形 → 线圈状态机 → USB 上报 → GUI 协议 → 其余。

另外两份文档：

- [待确认问题.md](待确认问题.md)：读代码时发现、要问杰哥或上板验证的问题，按代码版本分开列。
- [讲解规则与接续说明.md](讲解规则与接续说明.md)：讲法、约束、进度；换新会话时先读这份。

可搜索、能勾选已读的网页版目录：[学习目录](https://claude.ai/artifact/9nXLjFSsDezEn5UDfeTqe8)（源文件 `4_速查/学习目录.html`）。

```
study_notes/
├── 1_T1A新代码/        ← 现在学的：杰哥最新代码（板型 GT_LINA_T1A_H5_PWM）
├── 2_基础课/           ← 第 0~13 课：电、单片机、各个外设（和代码版本无关）
├── 3_旧版IRPT_V4代码/  ← 在旧板型上读代码的讲解；多数代码 T1A 没改，仍然适用
├── 4_速查/             ← 引脚、地址、端点、代码来源等随时查的表
├── 5_早期讲义/         ← 改版前的 Markdown 逐行精读，当字典查
├── sim/                ← 按键算法的电脑端模拟（C）
└── tools/              ← HID 调试脚本、代码来源清单生成脚本
```

## 1. T1A 新代码（现在学）

| 课 | 页面 | 讲什么 |
|---|---|---|
| 第 1 课 | [T1A 全景地图](https://claude.ai/artifact/UKS8dvoYJWBQPnUPaS4sXA) | 板子框图、引脚表（原理图对照代码）、固件骨架、学习路线、引脚练习题 |
| 第 2 课 | [一次左键点击全程](https://claude.ai/artifact/3pr9UvqxTKk3jFx1QUzg71) | 读 → 判 → 震 → 报，12 步逐步走、数据接力表、随机练习 |
| 第 3 课（上） | [读一次霍尔芯片](https://claude.ai/artifact/RSTQPaAEsAcNwHQ3aaUgSa) | 下命令（SCK 拉高 5 µs）+ SPI1 搬 32 位；逐行逐变量；菊花链模拟器、校验位工具、练习 |
| 第 3 课（下）① | [开机准备 SPI1](https://claude.ai/artifact/K8jtZ8wuzFac7dAVtqgwkR) | InitSPI1Master 12 行：每行寄存器位变化 + 准备情况；位运算工具、时钟链、速度计算器、练习 |

## 2. 基础课（第 0~13 课，按顺序读）

| 课 | 页面 |
|---|---|
| 第 0 课 | [这只鼠标的地图](https://claude.ai/artifact/M2RnYCfavarnoiBEjJ9Eky) |
| 第 1 课 | [电怎么传信息](https://claude.ai/artifact/QAeSDfXisuYV89dGuqVqX2) |
| 第 2 课 | [0 和 1：一排开关](https://claude.ai/artifact/HwMr9Ng8ocHS9MBvvJD1bX) |
| 第 3 课 | [单片机是什么（外设七步法）](https://claude.ai/artifact/Df8iYXGpcqsQs45cVTXCQf) |
| 第 4 课 | [程序怎么跑](https://claude.ai/artifact/8avBhNLVJRzRPehvi6xtsN) |
| 第 5 课 | [代码仓库怎么读](https://claude.ai/artifact/DMHaAKpxifpv3aa1bAJoeM) |
| 第 6 课 | [GPIO 与引脚配置](https://claude.ai/artifact/NHfRyw19iDqMJRYwhLUfTw) |
| 第 7 课 | [ADC](https://claude.ai/artifact/3CLVMetraYoANnCKhpJuC6) |
| 第 8 课 | [定时器](https://claude.ai/artifact/RjnBzgLW6mYK5WRAY7Xviy) |
| 第 9 课 | [SPI 与 PAW3950](https://claude.ai/artifact/7HACF2oXVpWjSqS19giSyB) |
| 第 10 课 | [USB](https://claude.ai/artifact/C5mHnxa8bSv88j9P2V7jvN) |
| 第 11 课 | [EEPROM 与 IAP](https://claude.ai/artifact/Q5ub7uZ9fE8dAmVVgaJvoy) |
| 第 12 课 | [看门狗、复位与掉电](https://claude.ai/artifact/NhatmnbFU8ZA6ZF1UzLvws) |
| 第 13 课 | [时钟、电源与低功耗](https://claude.ai/artifact/2Kw9ZtbCBu6cax3WLFieuv) |
| 专题 | [C 语言里的面向对象（用本项目代码举例）](https://claude.ai/artifact/1pZrKGuEwLFjCwAWh7nK8H) |

## 3. 旧版 IRPT_V4 代码

这些是在旧板型 `IRPT_V4` 上做的讲解（旧代码在提交 `367ef40`）。最后一列说明 T1A 上还适不适用。

| 页面 | 讲什么 | T1A 上 |
|---|---|---|
| [一次左键点击全程](https://claude.ai/artifact/QoPZuz6jMfgBqK7WrEJL29) | 17 步全程、26 个全局变量、24 个函数 | 大部分适用；读数（ADC → 霍尔）和线圈两段变了 |
| [ADC 串讲（10 关 + 练习题）](https://claude.ai/artifact/KM5wmiR61CJWKNEk68Xnfs) | ADC 驱动 + 按键算法 | 第 1~4 关不用了；第 5~10 关算法照用（霍尔分支参数不同） |
| [ADC 全串讲参考版](https://claude.ai/artifact/HCgkuzBx81QJ9H9jpG2Xr5) | 逐行寄存器面板、全程模拟器、坑表 | 同上 |
| [按下 DPI 键的完整流程](https://claude.ai/artifact/7yhiDgMLCiZYLqHzKXnRVy) | 从按下到 GUI 收到通知，跨 9 个文件 | 适用 |
| [custom.c 完全讲透](https://claude.ai/artifact/VZVKafNhxDA6watP5e6RMT) | 键位映射层，26 个函数 | 适用 |
| [Custom_Paramter_Init()](https://claude.ai/artifact/C2Pkz5eGpiTkeNkUvThqJJ) | 早期版，已并入上一页 | 适用 |
| [Button_GetState()](https://claude.ai/artifact/BMsC1ca4aV6Wh5QRUoTySZ) | 6 个键的快照 | 侧键 / 中键 / DPI 适用；左右键改成霍尔 |
| [app 文件夹四份代码](https://claude.ai/artifact/DQeECLZbqjbizU3wtk2zHv) | button / scroll / custom / optical_3325 | 大部分适用；button.c 有改动 |
| [从内存看一次 IO](https://claude.ai/artifact/6Z4TjTfoJmmkLUsBX27KDm) | GUI 收发在 USB RAM 和 SRAM 之间怎么复制 | 适用 |

## 4. 速查

| 页面 | 讲什么 |
|---|---|
| [学习目录](https://claude.ai/artifact/9nXLjFSsDezEn5UDfeTqe8) | 所有页面，可搜索、可勾选已读 |
| [引脚总表](https://claude.ai/artifact/5PUa6UQrGCyzkQPpPrzByB) | 33 个脚的功能和配置（目前按 IRPT_V4 写，T1A 引脚见“杰哥最新代码总览”第 2 节） |
| [代码来源清单](https://claude.ai/artifact/VPsyav56Zh3cacuF4xg6us) | 每个文件是谁写的、每个函数在不在用（按 IRPT_V4 生成） |
| [芯片存储地址分布](https://claude.ai/artifact/JoG6GLLsaj4RPKbny4Ea4i) | ROM / Flash / RAM / USB RAM / EEPROM |
| [USB 接口与端点总表](https://claude.ai/artifact/Axd33DZ2jkDBAo9C5vSKac) | 3 个接口、各端点谁发谁收 |
| [学习目标与覆盖](https://claude.ai/artifact/Fe6KbvbH6bcSansEMkZds1) | “熟悉代码 / 手册”的标准和进度 |

## 5. 早期讲义（GitHub 里直接看）

在 `5_早期讲义/`：

- **总览**：`00_项目全景与新学习路线`、`01_逐行精读路线图`、`02_芯片状态总表`、`03_T1A适配清单`、`04_回顾题`
- **启动**：`0-3_公司Bootloader`、`0-3_启动与升级全景图.html`、`0-4_启动文件`
- **逐行精读**：`1-1` 看门狗 ~ `1-14` 三个 Pointer_Init
- **专题**：`L1_USB是怎么工作的`、[L1 USB 报文解剖](https://claude.ai/artifact/Qvima6yiNCJCzXs62oqyEC)、`L2_一次点击`、`L2_一次点击图解.html`

---

**新增讲解页时**：放进对应文件夹，同时更新本文件的表格和 `4_速查/学习目录.html` 里的 `G` 数组。
