# 本项目配套 GUI：Test HAPTIC MOUSE（V1.0，区分按压和释放的振感强度 - 静音）

**来源**：同事写的上位机测试软件。第一次 2026-11-04 拿到；**2026-11-09 更新为最新源码**（源码最后修改 2026-11-04 08:53，编译版 V1.0.09）。原压缩包名 `V1.0_HAPTIC_MOUSE_区分按压和释放的振感强度 - 静音.rar`。

**用途**：通过 USB（VID 1A81、PID 2071，自定义 HID 接口 mi_02）和鼠标通信：

- 读左右键的霍尔数据（currentRaw、baselineBtm、baselineTop、crtSegment、maxSegment）
- 读 / 设快速触发参数（AP、按下 RT、松开 RT，生效 / 失效）
- 读 / 设按下、释放振感强度（线圈导通时间，滑块 0~1200 µs）
- 设全局驱动方式：**正常模式**（= 0 = level，全程 100%）/ **静音模式**（= 1 = pwm，用 100 格力度表）；静音模式下用**静音挡位 0~3** 把左右键按下 / 释放的导通时间一起设成 0 / 1500 / 2000 / 2500 µs
- 读 / 设 PWM 力度表、调试配置（动态导通时间、自动振动）：代码还在，但这两个面板在界面上隐藏了
- 测按键速度（每秒点击次数，用 Windows 鼠标钩子统计）

和固件的对应关系见第 4 课（续）“用 GUI 看按键算法”。

## 目录

| 路径 | 内容 |
|---|---|
| `可直接运行的程序_V1.0.09/MouseTest.exe` | 编译好的程序（2026-11-04 08:27），Windows 上直接运行，需要 .NET Framework 4.8。源码在这之后又改过一次（08:53），同事电脑上的 `bin/Debug`（09:09）可能更新一点，压缩包里有但没放进仓库 |
| `MouseTest.sln`、`MouseTest/` | C# WPF 源码 |
| `MouseTest/CommandClass.cs` | 所有发给鼠标的命令（HID 包怎么拼） |
| `MouseTest/MainWindow.xaml(.cs)` | 界面和收包解析（`CheckData()`） |
| `MouseTest/HID/` | Windows HID 读写 |

## 解压时没有放进仓库的东西

- 旧版本编译结果：`bin/Haptic TEST 通用软件 V1.0 ~ V1.0.08` 及各自的 `.rar`、`bin/Debug`、`bin/Release`
- 编译中间文件 `obj/`、NuGet 包 `packages/`（Newtonsoft.Json 13.0.3，可用 NuGet 还原）
- IDE 本地文件 `.vs/`、`.idea/`、`.lingma/`、`.vscode/`

**自己编译时注意**：`MouseTest.csproj` 引用的 `LineTable.dll`、`ReportingRate.dll` 写的是 `bin\Debug\`，`MouseKeyModel.dll` 写的是同事电脑上的绝对路径（`..\..\..\..\..\DLL\鼠标测试软件模块\...`）。这三个 DLL 在 `可直接运行的程序_V1.0.09/` 里都有，编译前把它们拷到 `MouseTest/bin/Debug/` 并改 HintPath。

## 2026-11-09 这一版改了什么（和 11-04 版比）

| 文件 | 改动 |
|---|---|
| `CommandClass.cs` | `SetDriveMode(bDriveMode)` 改成全局一个值（0x03 / 0x06 / 0xFF），和新固件一致；新增 `GetDriveMode()`、`GetReleaseCoil()`、`SetPwm()` / `GetPwm()`（力度表分两包，起始下标 0x00 / 0x32，每包 50 字节） |
| `MainWindow.xaml` | 按下 / 释放框里的 Level / Waveform 单选去掉；新增“驱动方式”框（正常 / 静音 + 挡位 0~3）；强度滑块上限 5000 → 1200；新增“PWM 配置”框但隐藏；“调试配置”框隐藏；窗口加宽 |
| `MainWindow.xaml.cs` | 静音挡位逐条下发（左按下 → 左释放 → 右按下 → 右释放，每条等回复，300 ms 没回重发一次）；从静音切回正常时先把四个导通时间设 0 再切模式；读配置的链路加了驱动方式、释放线圈、PWM 表 |

和固件对不上或值得问的地方，记在 `study_notes/待确认问题.md` 第三节 9~13。
