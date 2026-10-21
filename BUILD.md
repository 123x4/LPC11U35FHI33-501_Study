# 编译说明

本项目为 NXP LPC11U37/501（Cortex-M0）固件，工程文件为 `Sample.uvproj`，目标为 `Target 1`。

## 本机环境

- Keil MDK 5.37：`D:\Keil5\Core`
- uVision：`D:\Keil5\Core\UV4\UV4.exe`
- ARM Compiler 5.06 update 7 (build 960)：`D:\Keil5\Core\ARM\ARMCC\bin`
- 工程已选择该 ARM Compiler 5，头文件路径为 `inc` 和 `inc\Led_Effect_H`。

本机已具备匹配的工具链，不需要另行安装或修改系统 PATH。更换电脑时，需准备 Keil MDK 及相同版本的 ARM Compiler 5，并在工程 Options for Target 的 Target 页选择该编译器。原工程使用旧式设备配置，需要 Keil 支持 LPC11U37/501。

## PowerShell 编译

在项目目录执行：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\build.ps1
```

默认完整重编译；增量编译添加 `-Incremental`。Keil 安装路径不同时：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\build.ps1 -KeilPath 'C:\Keil_v5\UV4\UV4.exe'
```

脚本等待编译结束，输出日志并检查错误。ExecutionPolicy Bypass 仅作用于这次 PowerShell 进程。

## Keil 界面编译

1. 使用 uVision 打开 `Sample.uvproj`。
2. 选择 `Target 1`，确认 ARM Compiler 为 V5.06 update 7 (build 960)。
3. 点击 Project → Rebuild all target files（完整重编译），或按 F7 增量编译。
4. Build Output 中出现 `0 Error(s)` 表示成功。

## VS Code 代码诊断

`.vscode/c_cpp_properties.json` 为 C/C++ 扩展配置 ARMCC5 的头文件路径和版本宏；`.vscode/armcc_intellisense.h` 仅供编辑器强制包含，用于解析 ARMCC 属性、寄存器绑定和内建函数，并以声明替代 CMSIS 中的 ARM 汇编函数。该兼容层只在 `__INTELLISENSE__` 下生效，不参与 Keil 固件编译，也不关闭代码错误检查。

更新配置后若仍显示旧诊断，可执行命令 `C/C++: Reset IntelliSense Database`，然后重新打开源文件。换电脑时需同步修改配置中的 Keil 头文件路径。实际编译结果仍以 `build.ps1` 的输出为准。

## 产物与验证

- `output\Sample.hex`：本次生成的 Intel HEX 固件。
- `output\Sample.axf`：本次生成的调试 ELF 文件。
- `output\build.log`：命令行编译日志。
- `List`：编译列表及映射等文件。

2026-10-08 完整重编译通过：0 Error(s)、0 Warning(s)。
Program Size: Code=32084、RO-data=13236、RW-data=832、ZI-data=6016（字节）。

工程的 After Build 两个命令均未启用，因此当前编译不生成新的 BIN。已有 `output\Sample.bin` 是目录，其中为历史文件，不应当作本次产物。本次只验证编译与链接，未进行硬件烧录或运行验证。
