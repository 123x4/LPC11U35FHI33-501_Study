# 本项目配套 GUI：Test HAPTIC MOUSE（V1.0，区分按压和释放的振感强度 - 静音）

**来源**：同事写的上位机测试软件，2026-11-04 拿到（原压缩包 `V1.0_HAPTIC_MOUSE_区分按压和释放的振感强度 - 静音.rar`）。可能还不完整。

**用途**：通过 USB（VID 1A81、PID 2071，自定义 HID 接口 mi_02）和鼠标通信：

- 读左右键的霍尔数据（currentRaw、baselineBtm、baselineTop、crtSegment、maxSegment）
- 读 / 设快速触发参数（AP、按下 RT、松开 RT，生效 / 失效）
- 读 / 设按下、释放振感强度（线圈导通时间）、驱动方式（Level / Waveform）、调试配置
- 测按键速度（每秒点击次数，用 Windows 鼠标钩子统计）

和固件的对应关系见第 4 课（续）“用 GUI 看按键算法”。

## 目录

| 路径 | 内容 |
|---|---|
| `可直接运行的程序_V1.0.05/MouseTest.exe` | 编译好的程序（2026-10-30，和源码同一版），Windows 上直接运行，需要 .NET Framework 4.8 |
| `MouseTest.sln`、`MouseTest/` | C# WPF 源码 |
| `MouseTest/CommandClass.cs` | 所有发给鼠标的命令（HID 包怎么拼） |
| `MouseTest/MainWindow.xaml(.cs)` | 界面和收包解析（`CheckData()`） |
| `MouseTest/HID/` | Windows HID 读写 |

## 解压时没有放进仓库的东西

- 旧版本编译结果：`bin/Haptic TEST 通用软件 V1.0 ~ V1.0.04` 及各自的 `.rar`、`bin/Debug`（和 V1.0.05 重复）
- 编译中间文件 `obj/`、NuGet 包 `packages/`（Newtonsoft.Json 13.0.3，可用 NuGet 还原）
- IDE 本地文件 `.vs/`、`.idea/`、`.lingma/`、`.vscode/`

**自己编译时注意**：`MouseTest.csproj` 引用的 `LineTable.dll`、`ReportingRate.dll` 写的是 `bin\Debug\`，`MouseKeyModel.dll` 写的是同事电脑上的绝对路径（`..\..\..\..\..\DLL\鼠标测试软件模块\...`）。这三个 DLL 在 `可直接运行的程序_V1.0.05/` 里都有，编译前把它们拷到 `MouseTest/bin/Debug/` 并改 HintPath。
