./#!/bin/bash

# 通用 macOS 构建脚本 - 支持测试应用构建选择

set -e  # 遇到错误立即退出

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

PROJECT_NAME=$(basename "$(pwd)")
echo -e "${GREEN}=== ${PROJECT_NAME} macOS构建脚本 ===${NC}"

if [[ $(uname) != "Darwin" ]]; then
    echo -e "${RED}错误: 此脚本必须在macOS环境下运行${NC}"
    exit 1
fi

if ! command -v cmake &> /dev/null; then
    echo -e "${RED}错误: 未安装CMake${NC}"
    echo -e "${YELLOW}请先安装CMake，可通过以下方式之一:${NC}"
    echo -e "${YELLOW}1. 使用Homebrew: brew install cmake${NC}"
    echo -e "${YELLOW}2. 从官网下载: https://cmake.org/download/${NC}"
    exit 1
fi

CMAKE_VERSION=$(cmake --version | head -n 1 | awk '{print $3}')
echo -e "${BLUE}检测到CMake版本: $CMAKE_VERSION${NC}"

PROJECT_ROOT=$(pwd)
BUILD_DIR="${PROJECT_ROOT}/build_macos"

echo -e "${YELLOW}项目根目录: ${PROJECT_ROOT}${NC}"
echo -e "${YELLOW}构建目录: ${BUILD_DIR}${NC}"

BUILD_TESTS=ON
BUILD_TYPE=Debug

show_help() {
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --with-tests      构建测试应用 (默认)"
    echo "  --without-tests   不构建测试应用"
    echo "  --debug          调试模式构建"
    echo "  --release        发布模式构建 (默认)"
    echo "  --help           显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0                        # 默认构建（包含测试应用，Release模式）"
    echo "  $0 --without-tests        # 只构建库，不构建测试应用"
    echo "  $0 --debug --with-tests   # 调试模式构建，包含测试应用"
    echo ""
}

while [[ $# -gt 0 ]]; do
    case $1 in
        --with-tests)
            BUILD_TESTS=ON
            shift
            ;;
        --without-tests)
            BUILD_TESTS=OFF
            shift
            ;;
        --debug)
            BUILD_TYPE=Debug
            shift
            ;;
        --release)
            BUILD_TYPE=Release
            shift
            ;;
        --help)
            show_help
            exit 0
            ;;
        *)
            echo -e "${RED}未知选项: $1${NC}"
            show_help
            exit 1
            ;;
    esac
    done

echo -e "${BLUE}=== 构建配置 ===${NC}"
echo -e "${BLUE}构建类型: ${BUILD_TYPE}${NC}"
echo -e "${BLUE}构建测试应用: ${BUILD_TESTS}${NC}"
echo -e "${BLUE}===============${NC}"

if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}清理旧的构建目录...${NC}"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo -e "${GREEN}开始CMake配置...${NC}"

cmake -G "Unix Makefiles" \
    -DBUILD_PLATFORM=macos \
    -DCMAKE_SYSTEM_NAME=Darwin \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DOPTION_WITH_${PROJECT_NAME}_TEST=${BUILD_TESTS} \
    ..

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake配置失败${NC}"
    exit 1
fi

echo -e "${GREEN}开始编译...${NC}"

cmake --build . --config ${BUILD_TYPE}

if [ $? -ne 0 ]; then
    echo -e "${RED}编译失败${NC}"
    exit 1
fi

echo -e "${GREEN}=== 构建完成 ===${NC}"

echo -e "${YELLOW}生成的文件:${NC}"
echo -e "${BLUE}静态库 (.a):${NC}"
find "$BUILD_DIR" -name "*.a" -type f | while read file; do
    echo -e "${GREEN}  $(basename "$file")${NC} - $(ls -lh "$file" | awk '{print $5}')"
done

echo -e "${BLUE}动态库 (.dylib):${NC}"
find "$BUILD_DIR" -name "*.dylib" -type f | while read file; do
    echo -e "${GREEN}  $(basename "$file")${NC} - $(ls -lh "$file" | awk '{print $5}')"
done

if [ "$BUILD_TESTS" == "ON" ]; then
    echo -e "${BLUE}测试应用:${NC}"
    find "$BUILD_DIR" -name "test_${PROJECT_NAME}" -type f | while read file; do
        echo -e "${GREEN}  $(basename "$file")${NC} - $(ls -lh "$file" | awk '{print $5}')"
    done
fi

echo -e "${GREEN}构建成功完成！${NC}"
