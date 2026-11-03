# H5 霍尔芯片参考代码（2026-02-10）

**来源**：经理给的，2026-11-03 杰哥转发。是霍尔芯片厂商（G.TECH）给磁轴键盘写的参考驱动，配套芯片手册在 `materials/GTRY154_Magnetic_Keyboard_Sensor_and_LED_Driver_V1.1_20240719.pdf`。

**H5 是什么**：第 5 代霍尔检测技术，对应芯片 **GTRY154**（手册第 1 节：“With the 5rd generation Hall detecting technology”）。本项目 `inc/button.h:16` 的 `USE_HALL_SENSOR 5`、板型宏 `GT_LINA_T1A_H5` 都是指它。

文件按原样保存（GBK 编码、CRLF 换行，未改动）：

| 文件 | 内容 | 和本项目的关系 |
|---|---|---|
| `ana_sw_phy.c / .h` | 最底层：用 SCK 高电平时间发指令、SPI 收发、校验（`ana_phy_bit_error_checker2`）、频率校准、睡眠 / 唤醒；时序常数 TD1~TD13 | 本项目 `app/button.c` 的 `Hall_CLK_Command_SET()`、`hall_get_sample()`、`Hall_osc_calibrate()` 和 `src/my_misc.c` 的 `ana_phy_bit_error_checker2()` 由此改写 |
| `ana_hall2_phy.c / .h` | 写寄存器：初始化寄存器表 `sensor_reg_init_array`、增益、LED 电流、省电模式 | 本项目 `app/button.c:226` 的同名寄存器表、`ana_hall2_phy_set_mode()` 由此而来 |
| `ana_sw_hal.c` | 上层：上电复位、ESD 保护、基线学习和校准、段位计算、快速触发、产测 | 和本项目 `src/my_misc.c` 的按键算法思路相同（基线、段位、快速触发），可对照 |
| `main.c` | 键盘工程的主循环、USB、和上位机通信、产线校准 | 参考结构，键盘专用部分（矩阵、灯效）本项目用不到 |

注意：这套代码依赖键盘工程里的其他头文件（`config_common.h`、`ram_dma.h`、`kb_matrix.h` 等），不能单独编译，只做参考。
