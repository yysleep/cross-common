#!/bin/bash

# macOS 全库批量构建脚本
PROJECT_ROOT=$(pwd)

rm -rf output_ios

LIBS=("liblogger" "libhandler" "libjson" "libbuffer")

for LIB in "${LIBS[@]}"; do
    echo "======================"
    echo "Building $LIB ..."
    echo "======================"

    cd "$PROJECT_ROOT/$LIB" || { echo "Error: Failed to enter $LIB"; exit 1; }

    # 清理旧文件
    rm -rf Makefile CMakeCache.txt cmake_install.cmake CMakeFiles build_ios

    # 执行 macOS 构建脚本
    if [ -f "build_ios.sh" ]; then
        bash build_ios.sh --release --without-tests || { echo "Error: Failed to build $LIB"; exit 1; }
    else
        echo "Warning: $LIB 没有 build_ios.sh，跳过"
    fi

    # 拷贝产物到统一输出目录
    if [ -d "build_ios/$LIB" ]; then
        mkdir -p "$PROJECT_ROOT/output_ios/$LIB"
        cp -r build_ios/$LIB/* "$PROJECT_ROOT/output_ios/$LIB/"
    fi

    echo "$LIB built successfully!"
    cd "$PROJECT_ROOT"
done

echo "======================"
echo "All libraries built!"
