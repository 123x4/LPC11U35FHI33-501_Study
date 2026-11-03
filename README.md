# 触觉鼠标固件（LPC11U35）

杰哥带我做的第一个项目：带线圈震动反馈的触觉鼠标。芯片是 NXP LPC11U35（Cortex-M0，48 MHz），用 Keil MDK 编译。

**当前代码**：杰哥 2026-11-03 的版本，板型 `GT_LINA_T1A_H5_PWM`（T1A 板，在 `inc/my_board.h` 里选）。旧板型 `IRPT_V4` 的代码在提交 `367ef40`。

## 从哪里开始

| 想做什么 | 去哪里 |
|---|---|
| 学这个项目 | [study_notes/README.md](study_notes/README.md)（学习资料目录） |
| 看新代码改了什么 | [杰哥最新代码总览](https://claude.ai/artifact/UKS8dvoYJWBQPnUPaS4sXA) |
| 看还有哪些问题没确认 | [study_notes/待确认问题.md](study_notes/待确认问题.md) |
| 编译 | [BUILD.md](BUILD.md) |

## 仓库里有什么

```
├── Sample.uvproj          Keil 工程（打开它）
├── src/                   驱动和系统代码：USB、GPIO、SPI、定时器、线圈、协议……
├── app/                   应用层：按键、滚轮、键位映射、传感器
├── inc/                   头文件；inc/my_board.h 选板型、定义引脚
├── Led_Effect_Gruop_Code/ 灯效和产线测试（公司老代码，部分编进了工程）
├── List/  output/         编译产物（map、hex、axf），保留下来方便查
├── study_notes/           学习资料（讲解网页的源文件、早期讲义、工具）
├── materials/             T1A 原理图、霍尔芯片 GTRY154 手册和 H5 参考代码、PAW3950 手册、MCU 手册、LPCOpen 例程
└── M300S/                 公司老项目资料
```

## T1A 板主要硬件

| 部分 | 器件 / 接法 |
|---|---|
| 左右键 | 霍尔芯片，SPI1（PIO0_21 / 22、PIO1_15），电源 PIO0_16 |
| 线圈 | 两路 200 kHz PWM（PIO0_13 / 14，定时器 CT32B1），升压使能 PIO1_19 |
| 光学传感器 | PAW3950，SPI0（PIO0_8 / 9 / 10），片选 PIO0_20 |
| 其他按键 | 前侧 PIO0_11、后侧 PIO0_12、中键 PIO0_7、DPI PIO0_23 |
| 滚轮 | PIO0_17 / 18（目前代码里关着） |
