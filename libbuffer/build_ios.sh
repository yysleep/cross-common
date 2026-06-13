#!/bin/bash

# 通用 iOS 构建脚本 - 支持测试应用构建选择

set -e  # 遇到错误立即退出

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 自动获取项目名称（当前目录名）
PROJECT_NAME=$(basename "$(pwd)")
echo -e "${GREEN}=== ${PROJECT_NAME} iOS构建脚本 ===${NC}"

# 检查是否在macOS环境
if [[ $(uname) != "Darwin" ]]; then
    echo -e "${RED}错误: 此脚本必须在macOS环境下运行${NC}"
    exit 1
fi

# 检查是否安装了Xcode
if ! command -v xcodebuild &> /dev/null; then
    echo -e "${RED}错误: 未安装Xcode${NC}"
    echo -e "${YELLOW}请在App Store安装Xcode，或通过命令行安装: xcode-select --install${NC}"
    exit 1
fi

# 检查是否安装了CMake
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}错误: 未安装CMake${NC}"
    echo -e "${YELLOW}请先安装CMake，可通过以下方式之一:${NC}"
    echo -e "${YELLOW}1. 使用Homebrew: brew install cmake${NC}"
    echo -e "${YELLOW}2. 从官网下载: https://cmake.org/download/${NC}"
    exit 1
fi

# 显示安装的CMake版本
CMAKE_VERSION=$(cmake --version | head -n 1 | awk '{print $3}')
echo -e "${BLUE}检测到CMake版本: $CMAKE_VERSION${NC}"

# 项目根目录
PROJECT_ROOT=$(pwd)
BUILD_DIR="${PROJECT_ROOT}/build_ios"

echo -e "${YELLOW}项目根目录: ${PROJECT_ROOT}${NC}"
echo -e "${YELLOW}构建目录: ${BUILD_DIR}${NC}"

# 默认构建选项
BUILD_TESTS=ON
BUILD_TYPE=Debug

# 解析命令行参数
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

# 解析参数
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

# 显示构建配置
echo -e "${BLUE}=== 构建配置 ===${NC}"
echo -e "${BLUE}构建类型: ${BUILD_TYPE}${NC}"
echo -e "${BLUE}构建测试应用: ${BUILD_TESTS}${NC}"
echo -e "${BLUE}===============${NC}"

# 清理旧的构建目录
if [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}清理旧的构建目录...${NC}"
    rm -rf "$BUILD_DIR"
fi

# 创建构建目录
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo -e "${GREEN}开始CMake配置...${NC}"

# CMake配置
cmake -G Xcode \
    -DBUILD_PLATFORM=ios \
    -DCMAKE_SYSTEM_NAME=iOS \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO \
    -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED=NO \
    -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED=NO \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DOPTION_WITH_${PROJECT_NAME}_TEST=${BUILD_TESTS} \
    ..

if [ $? -ne 0 ]; then
    echo -e "${RED}CMake配置失败${NC}"
    exit 1
fi

echo -e "${GREEN}开始编译...${NC}"

# 编译
xcodebuild -configuration ${BUILD_TYPE} \
           -sdk iphoneos \
           -arch arm64 \
           -destination 'generic/platform=iOS' \
           ONLY_ACTIVE_ARCH=NO \
           CODE_SIGNING_REQUIRED=NO \
           CODE_SIGNING_ALLOWED=NO \
           clean build

if [ $? -ne 0 ]; then
    echo -e "${RED}编译失败${NC}"
    exit 1
fi

echo -e "${GREEN}=== 构建完成 ===${NC}"