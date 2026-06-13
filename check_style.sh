#!/bin/bash

# 使用 cpplint 检查所有C++文件的代码风格，但排除third_party、output和CMakeFiles目录
echo "正在使用 cpplint 检查所有源文件的代码风格（排除third_party、output和CMakeFiles目录）..."

# 检查 cpplint 是否安装
if ! command -v cpplint &> /dev/null; then
    echo "错误: cpplint 未安装"
    echo "请安装 cpplint: pip install cpplint"
    exit 1
fi

# 创建临时文件来存储计数和错误信息
temp_file=$(mktemp)
error_summary_file=$(mktemp)

# 查找并检查所有 .h, .hpp, .cpp, .cc 文件，但排除third_party、output和CMakeFiles目录
find . \( -name "third_party" -o -name "output" -o -name "CMakeFiles" -o -name ".git" \) -prune -o \( -name "*.h" -o -name "*.hpp" -o -name "*.cpp" -o -name "*.cc" \) -print | while read file; do
    echo "检查: $file"
    
    # 使用 cpplint 检查文件，实时显示输出
    if ! cpplint "$file"; then
        echo "ERROR" >> "$temp_file"
        # 保存错误信息到汇总文件，包含具体的错误行和描述
        echo "=== $file ===" >> "$error_summary_file"
        cpplint "$file" 2>&1 | grep -E "^[^D]" >> "$error_summary_file"
        echo "" >> "$error_summary_file"
    else
        echo "OK" >> "$temp_file"
    fi
done

# 计算总数和错误数
total_files=$(wc -l < "$temp_file" 2>/dev/null || echo "0")
error_files=$(grep -c "ERROR" "$temp_file" 2>/dev/null || echo "0")

echo ""
echo "=========================================="
echo "代码风格检查完成！"
echo "=========================================="
echo "总文件数: $total_files"
echo "有问题的文件数: $error_files"

if [ "$error_files" -eq 0 ]; then
    echo "✓ 所有文件代码风格检查通过！"
    # 清理临时文件
    rm -f "$temp_file" "$error_summary_file"
    exit 0
else
    echo "✗ 发现 $error_files 个文件存在代码风格问题"
    echo ""
    echo "=========================================="
    echo "错误汇总："
    echo "=========================================="
    
    # 打印错误汇总
    if [ -s "$error_summary_file" ]; then
        cat "$error_summary_file"
    fi
    
    echo ""
    echo "=========================================="
    echo "请根据上述错误信息修复代码风格问题"
    echo "=========================================="
    
    # 清理临时文件
    rm -f "$temp_file" "$error_summary_file"
    exit 1
fi 