# 1-14 三个 `xxx_Pointer_Init()`：给老配置系统“记地址”

> 📍 **位置**：`main()` → `Device_Init()` → **第 451~453 行**
> ```c
> OperateFlash_Pointer_Init();      // src/operation_flash.c:50
> PC_Communicate_Pointer_Init();    // src/pc_communicate.c:35
> Custom_Pointer_Init();            // app/custom.c:69
> ```
> 🔍 **核实**：三个函数都在 `Device_Init` 反汇编里（1-9 课第五节 0x9C10~0x9C18）✅；EEPROM 布局按 `OperateFlash_Pointer_Init` 反汇编 + hex 里的常量算出 ✅

---

## 📌 必须掌握（3 句话）

1. 这三个函数**只是给几个全局变量赋值**，不碰任何硬件寄存器。
2. 它们服务的是**老项目留下的“配置系统”**（按键映射、宏、灯效、模式），它把数据存在 **EEPROM 的前半段（0x000 起）**；和 1-6 课的新参数区（**0xF00 起**）是两套东西，互不重叠。
3. `Custom_Pointer_Init()` 让 `usb_sendBuffer_p` 指向 `receive_buff`（64 字节），这是**回复 GUI 时用的发送缓冲区**。

> 这套老配置系统在 1-17 课（`Custom_Init` 等）还会出现，到时候再看它存了什么。本课知道“它在 EEPROM 哪里”就够了。

---

## 选读：三个函数各做了什么

### ① `OperateFlash_Pointer_Init()`（`src/operation_flash.c:50~68`）

> 注意：这个文件里有两个同名函数，第 6~209 行在 `#if 1` 里（**编译**），第 211~421 行在 `#if 0` 里（**不编译**）。看第 50 行那个。

```c
gloabSetting_p = (UINT8*)&gloabSetting;                       // RAM 里配置结构体的地址
modeSetting_p  = (UINT8*)&gloabSetting.settingMode;           // 其中“模式设置”部分的地址

flash_gloabSetting_p = inside_Flash;                           // 以下全是 EEPROM 里的偏移
flash_modeSetting_p  = (inside_Flash + OPERATE_GLOAB_SETTING_SIZE);
macroHeart_Address_p = (inside_Flash + MACRO_HEART_OFFSET);
macroData_Address_p  = (inside_Flash + MACRO_DATA_OFFSET);
lightEffect_addr     = (inside_Flash + LIGHT_EFFECT_OFFSET);
Code_CompileTimer_Adr= (inside_Flash + LIGHT_EFFECT_OFFSET);
```

**`inside_Flash` 是什么？** `src/gtech_flash.c:26`：`UINT32 inside_Flash = 0;`。名字叫 “Flash”，但因为 `USE_INSIDE_EEPROM = 1`（`inc/IAP_CMD.h:10`），这套系统的读写函数实际调用的是 1-6 课的 `Chip_EEPROM_Read/Write`（`src/gtech_flash.c:283~303`）。所以这些“地址”其实是 **EEPROM 地址，从 0 开始**。

**实际算出来的值**（源码注释里的 0x0000A008 等是老项目的数字，**已经过时**，以编译产物为准）：

| 变量 | 值 | 含义 |
|---|---|---|
| `gloabSetting_p` | 0x1000 1833 | RAM：`gloabSetting`（`Sample.map` 第 1445 行，132 字节） |
| `modeSetting_p` | 0x1000 1836 | RAM：`gloabSetting` + 3 |
| `flash_gloabSetting_p` | **0**（0x000） | EEPROM：全局设置，3 字节 |
| `flash_modeSetting_p` | **3**（0x003） | EEPROM：模式设置（`MODE_NUM = 2` 个模式） |
| `macroHeart_Address_p` | **261**（0x105） | EEPROM：宏的索引表 |
| `macroData_Address_p` | **901**（0x385） | EEPROM：宏的数据 |
| `lightEffect_addr`、`Code_CompileTimer_Adr` | **2949**（0xB85） | EEPROM：编译时间等 |

怎么算出来的（`OperateFlash_Pointer_Init` @ 0x562A 的反汇编）：

```
562c: ldr  r0, =0x10001833     ; &gloabSetting
562e: str  r0, [r1, #0xc]      ; → gloabSetting_p
5630: adds r0, r0, #0x3        ; +3
5632: str  r0, [r1, #0x10]     ; → modeSetting_p
5636: ldr  r0, [r0]            ; r0 = inside_Flash 的值 = 0
5638: str  r0, [r1, #0x4]      ; → flash_gloabSetting_p = 0
563a: adds r2, r0, #0x3        ; 0 + 3
563c: str  r2, [r1, #0x8]      ; → flash_modeSetting_p = 3
563e: adds r2, r0, #0x7 ; adds r2, #0xfe   ; 0 + 7 + 254 = 261
5642: str  r2, [r1, #0x14]     ; → macroHeart_Address_p = 261
5644: ldr  r2, =0x385 ; adds r2, r0, r2    ; 0 + 901
5648: str  r2, [r1, #0x18]     ; → macroData_Address_p = 901
564a: ldr  r2, =0xBB8 ; subs r2, #0x33 ; adds r0, r0, r2   ; 0 + 3000 - 51 = 2949
5650: str  r0, [r1, #0x1c]     ; → lightEffect_addr
5652: str  r0, [r1, #0x20]     ; → Code_CompileTimer_Adr
```

### ② `PC_Communicate_Pointer_Init()`（`src/pc_communicate.c:35`）

```c
flash_gloabSet_p = inside_Flash;     // = 0，和上面 flash_gloabSetting_p 指同一处
```

### ③ `Custom_Pointer_Init()`（`app/custom.c:69`）

```c
usb_sendBuffer_p = receive_buff;     // 指向 receive_buff（Sample.map 第 1437 行：0x1000 1650，64 字节）
```

`receive_buff` 是和 GUI 通信用的 64 字节缓冲区：GUI 发来的命令放这里，回复也从这里发出去（`src/pc_communicate.c:405`：`USB_LoadInEP_Data(EP_3_CUSTION_OUT, receive_buff, TRANSFER_LENGTH)`，1-13 精简版第 5 点）。

### 为什么要专门写函数来赋值？

这些赋值其实可以在定义变量时直接写初值（比如 `UINT8 *usb_sendBuffer_p = receive_buff;`）。写成 `xxx_Pointer_Init()` 是老项目的写法习惯，原因是老项目里 `inside_Flash` 是运行时才确定的（`#else` 分支里是 SPI Flash 的地址）。在本项目里没有实际意义，但也没有坏处。

---

## EEPROM 全貌（更新到 `02_芯片状态总表.md`）

```
EEPROM 地址
0x000 ┌──────────────────────┐
      │ 老配置系统（本课）       │
      │  0x000 全局设置  3 B    │
      │  0x003 模式设置          │
      │  0x105 宏索引            │
      │  0x385 宏数据            │
      │  0xB85 编译时间等         │
      │  ……                    │
0xF00 ├──────────────────────┤
      │ 新参数区（1-6 课）        │  版本号、校验和、88 B 参数
0xF5D ├──────────────────────┤
      │ 空闲                    │
0xFC0 ├──────────────────────┤
      │ 保留，不能写（1-6 课）     │
0xFFF └──────────────────────┘
```

> ⚠️ 老配置系统从 0xB85 往后到底用到哪里，要等 1-17 课看它写了多少字节才能确定。只要不超过 0xEFF，就不会和 1-6 课的参数区冲突。

---

## ❓ 练习

1. `usb_sendBuffer_p` 指向哪个变量？它有多大？在 RAM 的什么地址？
2. 老配置系统和 1-6 课的参数区，各自从 EEPROM 哪个地址开始？
3. 为什么说源码里 `//0x0000A008` 这类注释不可信？你是用什么方法确认真实值的？
