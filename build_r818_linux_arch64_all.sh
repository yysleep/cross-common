#!/bin/bash

# 设置R818工具链路径
export R818_TOOLCHAIN_PATH=/mnt/d/work/tool/gcc/prebuilt/gcc/linux-x86/aarch64/toolchain-sunxi-glibc/toolchain

# 检查工具链路径是否存在
if [ ! -d "$R818_TOOLCHAIN_PATH" ]; then
    echo "Error: R818_TOOLCHAIN_PATH directory does not exist: $R818_TOOLCHAIN_PATH"
    exit 1
fi

# 定义项目根目录
PROJECT_ROOT=$(pwd)

rm -rf output

# 定义编译顺序
LIBS=("liblogger" "libhandler" "libjson" "libbuffer")

# 遍历编译每个库
for LIB in "${LIBS[@]}"; do
    echo "======================"
    echo "Building $LIB ..."
    echo "======================"

    # 进入目录
    cd "$PROJECT_ROOT/$LIB" || { echo "Error: Failed to enter $LIB"; exit 1; }

    # 清理旧文件
    rm -rf Makefile CMakeCache.txt cmake_install.cmake CMakeFiles

    # 运行 CMake 配置
    cmake -DBUILD_PLATFORM=r818 \
      -DIBUILD_GCC_PREBUILD_TOOL_PATH=$R818_TOOLCHAIN_PATH

    # 运行 make 编译
    make -j$(nproc) || { echo "Error: Failed to build $LIB"; exit 1; }

    echo "$LIB built successfully!"
done

echo "======================"
echo "All libraries built!"
echo "======================"