#!/bin/sh
# 在 PC 上编译仓库原版 src/my_misc.c + 仿真主程序
# 依赖: gcc。Windows 下可用 MSYS2 / WSL。
set -e
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT="$HERE/../.."
BUILD="$HERE/build"
rm -rf "$BUILD" && mkdir -p "$BUILD"

# 原版头文件拷进 build 目录，让它们 #include 的芯片相关头文件落到 stubs 替身上
cp "$ROOT/inc/my_misc.h" "$ROOT/inc/my_hallkey_func.h" \
   "$ROOT/inc/my_protocol_common.h" "$ROOT/inc/my_protocol.h" "$BUILD/"
cp "$HERE"/stubs/*.h "$BUILD/"
cp "$ROOT/src/my_misc.c" "$BUILD/"

gcc -std=gnu99 -O0 -Wall -Wno-unused-function -I"$BUILD" \
    "$BUILD/my_misc.c" "$HERE/sim_main.c" -o "$HERE/key_sim"
echo "编译完成: $HERE/key_sim"
