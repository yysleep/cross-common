#!/bin/bash

# Linux x86 (32位) 全量构建脚本

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Linux x86 (32位) 全量构建脚本 ===${NC}"

# 检查是否为Linux环境
if [[ $(uname) != "Linux" ]]; then
    echo -e "${RED}错误: 此脚本必须在Linux环境下运行${NC}"
    exit 1
fi

# 检查是否安装了必要的32位开发工具
if ! dpkg --print-architecture | grep -q "amd64\|i386"; then
    echo -e "${YELLOW}警告: 建议安装32位开发工具包${NC}"
    echo -e "${YELLOW}可运行: sudo apt-get install gcc-multilib g++-multilib${NC}"
fi

# 检查CMake是否安装
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}错误: 未安装CMake${NC}"
    echo -e "${YELLOW}请先安装CMake: sudo apt-get install cmake${NC}"
    exit 1
fi

# 显示CMake版本
CMAKE_VERSION=$(cmake --version | head -n 1 | awk '{print $3}')
echo -e "${BLUE}检测到CMake版本: $CMAKE_VERSION${NC}"

# 定义项目根目录
PROJECT_ROOT=$(pwd)
echo -e "${YELLOW}项目根目录: ${PROJECT_ROOT}${NC}"

# 清理旧的输出目录
echo -e "${YELLOW}清理旧的输出目录...${NC}"
rm -rf output

# 定义编译顺序
LIBS=("liblogger" "libhandler" "libjson" "libbuffer")

echo -e "${BLUE}=== 构建配置 ===${NC}"
echo -e "${BLUE}目标架构: x86 (32位)${NC}"
echo -e "${BLUE}构建类型: Release${NC}"
echo -e "${BLUE}并发数: $(nproc)${NC}"
echo -e "${BLUE}===============${NC}"

# 遍历编译每个库
for LIB in "${LIBS[@]}"; do
    echo -e "${GREEN}======================${NC}"
    echo -e "${GREEN}正在构建 $LIB ...${NC}"
    echo -e "${GREEN}======================${NC}"

    # 进入目录
    cd "$PROJECT_ROOT/$LIB" || { echo -e "${RED}错误: 无法进入目录 $LIB${NC}"; exit 1; }

    # 清理旧文件
    echo -e "${YELLOW}清理旧的构建文件...${NC}"
    rm -rf Makefile CMakeCache.txt cmake_install.cmake CMakeFiles

    # 运行 CMake 配置 (针对32位)
    echo -e "${BLUE}开始CMake配置...${NC}"
    cmake -DBUILD_PLATFORM=linux \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_C_FLAGS="-m32" \
          -DCMAKE_CXX_FLAGS="-m32" \
          -DCMAKE_SYSTEM_PROCESSOR=i386 \
          -DCMAKE_SIZEOF_VOID_P=4

    if [ $? -ne 0 ]; then
        echo -e "${RED}错误: $LIB CMake配置失败${NC}"
        exit 1
    fi

    # 运行 make 编译
    echo -e "${BLUE}开始编译...${NC}"
    make -j$(nproc)
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}错误: $LIB 编译失败${NC}"
        exit 1
    fi

    echo -e "${GREEN}$LIB 构建成功！${NC}"
done

echo -e "${GREEN}======================${NC}"
echo -e "${GREEN}所有库构建完成！${NC}"
echo -e "${GREEN}======================${NC}"

# 显示构建结果
echo -e "${YELLOW}=== 构建结果 ===${NC}"
if [ -d "$PROJECT_ROOT/output" ]; then
    echo -e "${BLUE}输出目录: $PROJECT_ROOT/output${NC}"
    
    if [ -d "$PROJECT_ROOT/output/lib" ]; then
        echo -e "${BLUE}静态库文件:${NC}"
        find "$PROJECT_ROOT/output/lib" -name "*.a" | while read file; do
            echo -e "${GREEN}  $(basename "$file")${NC} - $(ls -lh "$file" | awk '{print $5}')"
        done
    fi
    
    if [ -d "$PROJECT_ROOT/output/bin" ]; then
        echo -e "${BLUE}可执行文件:${NC}"
        find "$PROJECT_ROOT/output/bin" -type f -executable | while read file; do
            echo -e "${GREEN}  $(basename "$file")${NC} - $(ls -lh "$file" | awk '{print $5}')"
        done
    fi
else
    echo -e "${YELLOW}输出目录不存在，请检查构建配置${NC}"
fi

echo -e "${GREEN}Linux x86 (32位) 构建完成！${NC}" 