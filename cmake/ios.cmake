# 设置 iOS 平台的库类型为静态库
set(LIB_TYPE STATIC)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)

# 配置 iOS 工具链
set(CMAKE_SYSTEM_NAME iOS)
set(CMAKE_OSX_DEPLOYMENT_TARGET
    "12.0"
    CACHE STRING "Minimum iOS deployment version")
set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET
    ${CMAKE_OSX_DEPLOYMENT_TARGET})

# 支持的架构
set(CMAKE_OSX_ARCHITECTURES
    "arm64"
    CACHE STRING "Build architectures for iOS")

# 启用语言支持
enable_language(C) # 启用 C 语言支持
enable_language(CXX) # 启用 C++ 语言支持

# iOS 相关编译选项
add_compile_definitions(TARGET_OS_IOS=1 __IPHONE_OS_VERSION_MIN_REQUIRED=120000)

# Xcode 特定属性配置
set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
set(CMAKE_XCODE_ATTRIBUTE_VALID_ARCHS "arm64")
set(CMAKE_IOS_INSTALL_COMBINED YES)

# 设置编译器标志
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fobjc-arc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")

# 设置SDK
set(CMAKE_OSX_SYSROOT
    "iphoneos"
    CACHE STRING "System root for iOS")

# 禁用bitcode（如果需要）
set(CMAKE_XCODE_ATTRIBUTE_ENABLE_BITCODE "NO")

# iOS 通用编译选项 (调试模式 - 符号可见)
add_compile_options(
  # -fvisibility=hidden                   # 默认隐藏所有符号 (调试阶段暂时禁用)
  # -fvisibility-inlines-hidden           # 隐藏内联函数符号 (调试阶段暂时禁用)
  -fembed-bitcode=off # 禁用bitcode
  -fPIC # 位置无关代码
  # -fno-common                           # 避免公共符号 (调试阶段暂时禁用) -fdata-sections #
  # 将数据放入独立段 (调试阶段暂时禁用) -ffunction-sections                   # 将函数放入独立段
  # (调试阶段暂时禁用)
)

# 设置构建类型相关的编译选项
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# 设置iOS通用系统框架变量（供各个库使用）
set(IOS_SYSTEM_FRAMEWORKS "-framework Foundation" "-framework CoreFoundation")
