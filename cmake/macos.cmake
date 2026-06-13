# 设置 macOS 平台的库类型为静态库
set(LIB_TYPE STATIC)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)

# 配置 macOS 工具链
set(CMAKE_SYSTEM_NAME Darwin)

# 支持的架构
set(CMAKE_OSX_ARCHITECTURES
    "x86_64;arm64"
    CACHE STRING "Build architectures for macOS")

# 启用语言支持
enable_language(C)
enable_language(CXX)

# macOS 相关编译选项
add_compile_definitions(TARGET_OS_MAC=1)

# 设置编译器标志
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# 设置SDK（一般无需指定，除非有特殊需求） set(CMAKE_OSX_SYSROOT "macosx" CACHE STRING "System root
# for macOS")

# macOS 通用编译选项
add_compile_options(-fPIC)

# 设置构建类型相关的编译选项
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# 设置macOS通用系统框架变量（供各个库使用）
set(MACOS_SYSTEM_FRAMEWORKS "-framework Foundation" "-framework CoreFoundation")