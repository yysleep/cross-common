#!/bin/bash

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
    cmake -DBUILD_PLATFORM=linux \
          -DCMAKE_BUILD_TYPE=Release

    # 运行 make 编译
    make -j$(nproc) || { echo "Error: Failed to build $LIB"; exit 1; }

    echo "$LIB built successfully!"
done

echo "======================"
echo "All libraries built!"