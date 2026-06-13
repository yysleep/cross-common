#!/bin/bash
# 批量格式化所有 CMake 文件

echo "正在查找所有 CMake 文件..."

# 查找所有 CMakeLists.txt 和 .cmake 文件
# 排除构建目录: CMakeFiles, build, output 等
find . \( -path "*/CMakeFiles/*" -o -path "*/build/*" -o -path "*/output/*" -o -path "*/.cmake/*" \) -prune -o \
    -type f \( -name "CMakeLists.txt" -o -name "*.cmake" \) -print | while read -r file; do
    echo "格式化: $file"
    cmake-format -i "$file"
    if [ $? -eq 0 ]; then
        echo "  ✓ 完成"
    else
        echo "  ✗ 失败"
    fi
done

echo ""
echo "所有 CMake 文件格式化完成！"

