#!/bin/bash

# 格式化所有C++文件为Google风格，但排除third_party、output和CMakeFiles目录
echo "正在使用Google C++风格格式化所有源文件（排除third_party、output和CMakeFiles目录）..."

# 查找并格式化所有 .h, .hpp, .c, .cpp 文件，但排除third_party、output和CMakeFiles目录
find . \( -name "third_party" -o -name "output" -o -name "CMakeFiles" \) -prune -o \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" \) -print | while read file; do
    echo "格式化: $file"
    clang-format -i "$file"
    
    # 确保文件末尾有换行符
    if [ -f "$file" ]; then
        # 检查文件是否以换行符结尾
        if [ "$(tail -c 1 "$file" | wc -l)" -eq 0 ]; then
            echo "  添加文件末尾换行符: $file"
            echo "" >> "$file"
        fi
    fi
done

echo "代码格式化完成！" 