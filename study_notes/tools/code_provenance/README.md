# 代码来源清单的生成脚本

生成网页 `study_notes/easy/代码来源清单.html` 的数据。代码改动后重新编译一次（更新 `List/Sample.map`、`output/Sample.axf`），再跑一遍就能刷新清单。

需要：Python 3、clang（只用来做预处理）、llvm-objdump / llvm-objcopy。在 Linux 上跑过；Windows 上没试过。

1. `callgraph.py <仓库> <输出目录>`：读 map 的符号表、反汇编 axf，从复位入口和中断向量表出发求出“走得到”的函数 → `funcs.json`。
2. 对工程里每个 .c 用 clang `-E` 预处理到 `<输出目录>/pp/`（include 路径 `inc`、`inc/Led_Effect_H`、`Led_Effect_Gruop_Code`，加 `-D__CC_ARM -D__ARMCC_VERSION=5060750`；缺的系统头文件用空文件代替）。
3. `inventory.py <仓库> <输出目录>`：源码函数 + 预处理结果 + funcs.json + 和 `M300S/MG380U_V1.0` 比对 → `inventory.json`。工程文件列表放在 `<输出目录>/../proj_files.txt`（从 `Sample.uvproj` 的 `<FilePath>` 提取）。
4. `gen2.py <仓库> <输出目录>`：按 `origins.py` 给每个文件定“最终来源”，写网页数据 `page_data2.json`（还要一个 `notin.txt`：仓库里有、工程里没有的 .c）。把 `origins.py` 复制到 `<输出目录>` 里再跑。
5. 网页：把 `page_data2.json` 的内容作为 `const DATA=…;` 放进 `代码来源清单.html` 的 `<script>` 开头。

`origins.py` 是人工写的：每个文件一行，写明分类（vendor 原厂原样 / vendor_mod 原厂底子 + 后人改过 / agent 方案商世平 / dev 历任开发者 / proj 本项目）和判断证据（版权头、署名、命名风格）。新增文件要在这里补一行，否则 gen2.py 会报错。
