"""Final origin of every .c / .s file, judged from evidence inside the file itself.

cat: vendor      = 原厂原样（芯片 / 工具厂商给的，基本没动）
     vendor_mod  = 原厂底子，后来被方案商或开发者改过
     agent       = 方案商（世平 WPI）写的
     dev         = 历任开发者写的（非原厂）
     proj        = 本项目（2026，chengjie）写的
"""
CATS = {
 'vendor':     ('🟦 原厂原样', 'per'),
 'vendor_mod': ('🟦 原厂底子 + 后人改过', 'hw'),
 'agent':      ('🟨 方案商（世平 WPI）', 'gold'),
 'dev':        ('⬜ 历任开发者', 'muted'),
 'proj':       ('🟩 本项目（2026）', 'sw'),
}
ADAM = '开发者 ADAM（文件头“Writer ADAM”）。他很可能是方案商世平那边的人：他写的 wnDrv_wI2C.c 用的是世平的 wn 命名，里面还有世平 Nikita 的注释；老项目工程里也有“Adam Wang”的 Keil 配置文件。待问杰哥'
GT = '没有原厂版权头；文件 / 函数以 gtech 命名'
O = {
 # ---- CMSIS / startup
 'src/core_cm0.c': ('vendor', 'ARM CMSIS', '文件头“Copyright (C) 2009-2010 ARM Limited”。CMSIS 是 ARM 给所有 Cortex-M 芯片的标准库'),
 'src/startup_LPC11Uxx.s': ('vendor', 'ARM / NXP CMSIS 启动文件', '文件头“CMSIS Cortex-M0 Core Device Startup File for NXP LPC11Uxx”“Copyright (C) 2012 ARM Limited”。向量表、Reset_Handler 都在这里'),
 # ---- USB stack
 'src/usbhw.c': ('vendor_mod', 'NXP USB 例程 → 方案商 / 开发者改过', '文件头“NXP LPC11xx USB example”；函数被改名成 nsAPI_ / NDrv_，加了世平风格的 wnApi_Events_Suspend_Handler、wnDrv_Usbhw_Suspend；注释里有“2016_0517_Hsiang”等开发者署名'),
 'src/usbcore.c': ('vendor_mod', 'Keil USB 例程 → 开发者改过', '文件头“Copyright (c) 2009 Keil - An ARM Company”；注释“(Add)Abellee 20130102 / 20161019”；还留着 Cypress 的 USBFS_ 名字'),
 'src/usbuser.c': ('vendor_mod', 'Keil USB 例程 → 方案商 / 开发者改过', '文件头 Keil 版权；里面调用 wnDrv_Pmu 等世平函数'),
 'src/usbdesc.c': ('vendor_mod', 'Keil USB 例程 → 开发者改过', '文件头 Keil 版权；描述符内容被改成这个产品的：“HansH Modify”“Bono:1 bit padding”“(Add)Abellee 20161019”'),
 'src/hiduser.c': ('vendor_mod', 'Keil USB 例程 → 开发者改过', '文件头 Keil 版权；HID 协议处理（Get/Set Report、Set Protocol）'),
 # ---- other vendor-based
 'src/IAP_CMD.c': ('vendor_mod', 'NXP LPCOpen 库 → 本项目改过', 'Chip_EEPROM_Write、Chip_IAP_ReadUID 等函数名和写法与 NXP 官方 LPCOpen 库一致；还留着库里的原句“//command[4] = SystemCoreClock / 1000;”被注释掉、改成 MCU_OSC / 1000；注释大段照抄用户手册 20.8 节。老项目 M300S 里没有这个文件，是本项目加进来的'),
 'app/optical_3325.c': ('vendor_mod', 'Cypress 参考设计 → 开发者改过', '文件头“Copyright 2003-2006, Cypress Semiconductor”“interface to the Avago optical chip using an SPI port on the PSoC”；还留着 PSoC 的 SPIM_Radio_TX_BUFFER_REG；寄存器部分被开发者改成 PixArt 3325'),
 'Led_Effect_Gruop_Code/PWM_1.c': ('vendor', 'Cypress PSoC Creator 自动生成', '文件头“Copyright 2013-2015, Cypress Semiconductor”，全是 CY_TCPWM_ 寄存器宏：PSoC Creator 工具为 PWM 组件自动生成的代码'),
 'app/SROM_0x04.c': ('vendor', 'PixArt 传感器固件', '整个文件只有一个 4000 多字节的数组 tracking_SROM：PixArt 3360 传感器的固件，开机时由 optical.c 下载进传感器。原厂给的二进制，不是人写的代码'),
 'app/SROM_0x81.c': ('vendor', 'PixArt 传感器固件', '数组 liftCutOff_calibration_SROM：PixArt 3360 的“抬起高度校准”固件，原厂提供'),
 'app/optical_3389_SROM0x05.c': ('vendor', 'PixArt 传感器固件', '数组 OP_3389_SROM_0X05：PixArt 3389 传感器固件，原厂提供'),
 # ---- solution provider WPI
 'src/wnDrv_Pmu.c': ('agent', '世平 WPI', '文件头“Copyright (c) 2013 . World Peace Industrial Co., LTD.”；wnDrv_ 是世平的命名'),
 'app/led_driver.c': ('agent', '世平 WPI → 开发者改过', '文件头是 WPI 的免责声明，原文件名 wnApi_Key_Controller.c，署名“Nikita 20140224”；后来有“By Martin 2017.11.10”的修改'),
 'src/wnDrv_wI2C.c': ('agent', '世平命名，署名 ADAM', '文件名 wnDrv_ 是世平命名；文件头“Writer ADAM”；注释里有“Nikita I2C_SCL”（Nikita 在 led_driver.c 的世平文件里署过名）'),
 # ---- ADAM drivers
 'src/Main.c': ('dev', 'ADAM', ADAM), 'src/GPIO.c': ('dev', 'ADAM', ADAM), 'src/I2C.c': ('dev', 'ADAM', ADAM + '；里面调用 wnDrv_wI2C'),
 'src/OSC.c': ('dev', 'ADAM', ADAM), 'src/SPI.c': ('dev', 'ADAM → Martin 改过', ADAM + '；注释“Martin 2017.12.1”'),
 'src/Timer.c': ('dev', 'ADAM', ADAM + '；还留着 Cypress 风格的 USBFS_SOF_ISR_EntryCallback'),
 'src/UART.c': ('dev', 'ADAM', ADAM), 'src/ADC.c': ('dev', 'ADAM', ADAM), 'src/WWDT.c': ('dev', 'ADAM', ADAM),
 'src/APP_74LVC595A.c': ('dev', 'ADAM', ADAM + '；74LVC595 数码管驱动'),
 # ---- gtech / unsigned developer code
 'src/gtech_flash.c': ('dev', '开发者（gtech）', GT + '；文件头“Copyright YOUR COMPANY, THE YEAR”是 Cypress PSoC Creator 新建文件的模板，还留着 PSoC 的 cy_checksum_exclude：说明最早写在 Cypress 平台上，后来移植到 NXP'),
 'src/operation_flash.c': ('dev', '开发者（gtech）', GT),
 'src/pc_communicate.c': ('dev', '开发者（gtech）', GT + '；老 GUI 协议'),
 'src/eeprom.c': ('dev', '开发者（gtech）', GT + '；外接 SPI Flash（MX25L）的驱动'),
 'src/device.c': ('dev', '开发者（gtech）', GT + '；主流程；调用了 nsAPI_、wnDrv_ 等函数'),
 'src/macro.c': ('dev', '开发者', '没有版权头；宏按键'),
 'src/keydef.c': ('dev', '开发者', '没有版权头；只有键值表'),
 'src/suspend.c': ('dev', '开发者', '没有版权头；调用世平的 wnApi_Events_*'),
 'src/usb.c': ('dev', '开发者（最早写在 Cypress 平台）', '没有版权头；注释掉的 USBFS_EnableOutEP、USBFS_GetConfiguration 等是 Cypress PSoC USB 组件的函数名：最早是给 Cypress 芯片写的封装，后来移植到 NXP'),
 'app/button.c': ('dev', '开发者 → 本项目大改', '没有版权头；老项目只有约 200 行，本项目加到 900 多行（霍尔、ADC 按键），新代码 include 了 my_adc.h、my_hallkey_func.h，风格和 chengjie 一致'),
 'app/custom.c': ('dev', '开发者 → 本项目改过', '没有版权头；按键映射、DPI、配置档；本项目改了 12 个函数'),
 'app/scroll.c': ('dev', '开发者', '没有版权头；滚轮'),
 'app/optical.c': ('dev', '开发者 Martin', '署名“By Martin 2017.11.13”；PixArt 3360 驱动，负责下载 SROM'),
 'app/optical_3389.c': ('dev', '开发者', '没有版权头；PixArt 3389 驱动，结构同 optical.c'),
 'app/led.c': ('dev', '开发者 Martin', '“this file for IS31FL3731 use”，署名“By Martin 2017.11.09”：照 ISSI 芯片手册写的 LED 驱动'),
 'app/LedControl.c': ('dev', '开发者 Martin', '“Modify by Matin 2017.11.08”'),
 'app/Fingerprint.c': ('dev', '开发者', '没有版权头，整个文件包在 #if 0 里，没做完的残留'),
 # ---- this project
}
for f in ['src/my_tick.c', 'src/my_queue.c', 'src/my_debug.c', 'src/my_electromagnet.c', 'src/my_misc.c', 'src/my_protocol.c',
          'src/my_protocol_mouse.c', 'src/TI_DRV8833.c', 'src/PAW3950.c', 'src/my_adc.c', 'src/my_hallkey_func.c', 'src/my_i2c.c']:
    O[f] = ('proj', 'chengjie', '文件头“@author chengjie”，创建日期 2026 年')
O['src/TI_DRV8833.c'] = ('proj', 'chengjie', '文件头“@author chengjie”；照 TI DRV8833 电机驱动芯片手册写的驱动（IRPT_V4 没用这颗芯片）')
LED = '没有版权头；LED 灯效（呼吸、彩虹、反应……），老项目带过来的'
for f in ['test.c', 'Production_line_test.c', 'ram_malloc.c', 'breath.c', 'color_cycle.c', 'led2.c', 'reaction.c', 'static.c',
          'breathe_group.c', 'color_cycle_group.c', 'led1.c', 'led_effect.c', 'led_effect_group_code.c', 'reaction_group.c',
          'static_group.c', 'waves_designer_group.c', 'waves_group.c']:
    O['Led_Effect_Gruop_Code/' + f] = ('dev', '开发者', LED)
O['Led_Effect_Gruop_Code/test.c'] = ('dev', '开发者', '没有版权头；测试用的打印、宏测试')
O['Led_Effect_Gruop_Code/Production_line_test.c'] = ('dev', '开发者', '没有版权头；产线测试模式')
O['Led_Effect_Gruop_Code/ram_malloc.c'] = ('dev', '开发者', '没有版权头；自己写的简易内存分配')
