#!/bin/bash

# 清理旧文件
rm -rf Makefile CMakeCache.txt cmake_install.cmake CMakeFiles

# 运行 CMake 配置
cmake -DBUILD_PLATFORM=android

# 运行 make 编译
make -j$(nproc)
