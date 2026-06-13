# 临时设置 android 平台的库类型为静态库 set(LIB_TYPE STATIC) set(LIB_TYPE STATIC)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)

# 配置 Android NDK
set(ANDROID_NDK /home/zhengyang4/ndk/android-ndk-r28c)
set(CMAKE_TOOLCHAIN_FILE ${ANDROID_NDK}/build/cmake/android.toolchain.cmake)
set(ANDROID_ABI "arm64-v8a")
set(ANDROID_PLATFORM "android-26")

# 选择 Android C++ STL 运行时（优先使用 CMAKE_ANDROID_STL_TYPE；并保留 ANDROID_STL 兼容）
# 注意：仅在外部未显式指定时设置默认值，避免覆盖调用方参数（例如命令行 -D...）。
if(NOT DEFINED CMAKE_ANDROID_STL_TYPE)
  set(CMAKE_ANDROID_STL_TYPE
      c++_shared
      CACHE STRING "")
endif()

if(NOT DEFINED ANDROID_STL)
  set(ANDROID_STL
      c++_shared
      CACHE STRING "")
endif()

# ============================================================================
# 16KB 页面对齐配置: Android 15+ 要求，主要针对 arm64-v8a
# ============================================================================
# 注意: Android 15+ 要求 native 库必须支持 16KB 页面对齐，否则无法加载 参考:
# https://developer.android.com/guide/practices/page-sizes
# ============================================================================
if(ANDROID_ABI STREQUAL "arm64-v8a")
  # 强制 16KB 页面对齐（Google 官方标准）
  add_link_options("-Wl,-z,max-page-size=16384")
  add_link_options("-Wl,-z,common-page-size=16384")

  message(STATUS "[android.cmake][✓] 16KB page alignment enabled for arm64-v8a")
else()
  message(STATUS "[android.cmake] 16KB page alignment skipped (not arm64-v8a)")
endif()

# 配置阶段打印关键 Android 参数，便于排查环境/参数是否生效
message(STATUS "[android.cmake] ANDROID_NDK=${ANDROID_NDK}")
message(STATUS "[android.cmake] CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "[android.cmake] ANDROID_ABI=${ANDROID_ABI}")
message(STATUS "[android.cmake] ANDROID_PLATFORM=${ANDROID_PLATFORM}")
message(
  STATUS "[android.cmake] CMAKE_ANDROID_STL_TYPE=${CMAKE_ANDROID_STL_TYPE}")
message(STATUS "[android.cmake] ANDROID_STL=${ANDROID_STL}")

# 保持动态链接C++标准库（默认为c++_shared）
enable_language(C) # 启用 C 语言支持
enable_language(CXX) # 启用 C++ 语言支持

# Android 相关编译选项 决定是否把日志输出到 stdout 上，用于调试(正式版本关闭)
add_compile_definitions(ANDROID_LOGGER_FALLBACK_TO_STDOUT)

# 自动拷贝NDK的C++动态库到output目录
function(copy_ndk_libs)
  # 查找libc++_shared.so的路径
  set(POSSIBLE_LIBCXX_PATHS
      "${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so"
      "${ANDROID_NDK}/sources/cxx-stl/llvm-libc++/libs/${ANDROID_ABI}/libc++_shared.so"
      "${ANDROID_NDK}/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so")

  # 查找libomp.so的路径 注意：NDK r28c 中 libomp.so 位于 lib/clang 目录下（不是 lib64/clang）
  set(LIBOMP_BASE_DIR
      "${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/lib/clang")

  # 使用 file(GLOB) 查找所有版本目录
  file(GLOB CLANG_VERSIONS "${LIBOMP_BASE_DIR}/*")
  set(POSSIBLE_LIBOMP_PATHS "")
  foreach(version_dir ${CLANG_VERSIONS})
    if(IS_DIRECTORY ${version_dir})
      list(APPEND POSSIBLE_LIBOMP_PATHS
           "${version_dir}/lib/linux/aarch64/libomp.so")
    endif()
  endforeach()

  set(LIBCXX_FOUND FALSE)
  foreach(path ${POSSIBLE_LIBCXX_PATHS})
    if(EXISTS ${path})
      set(LIBCXX_PATH ${path})
      set(LIBCXX_FOUND TRUE)
      message("找到 libc++_shared.so: ${path}")
      break()
    endif()
  endforeach()

  set(LIBOMP_FOUND FALSE)
  foreach(path ${POSSIBLE_LIBOMP_PATHS})
    if(EXISTS ${path})
      set(LIBOMP_PATH ${path})
      set(LIBOMP_FOUND TRUE)
      message("找到 libomp.so: ${path}")
      break()
    endif()
  endforeach()

  if(LIBCXX_FOUND)
    # 添加自定义命令，在编译时拷贝libc++_shared.so
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../output/lib/libc++_shared.so
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBCXX_PATH}
              ${CMAKE_CURRENT_SOURCE_DIR}/../output/lib/libc++_shared.so
      COMMENT "拷贝 libc++_shared.so 到 output/lib")

    # 创建一个自定义目标来触发拷贝
    add_custom_target(
      copy_libcxx ALL
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../output/lib/libc++_shared.so)

    message("✅ 将在编译时自动拷贝 libc++_shared.so 到 output/lib")
  else()
    message(WARNING "❌ 未找到 libc++_shared.so，可能需要手动拷贝")
  endif()

  if(LIBOMP_FOUND)
    # 添加自定义命令，在编译时拷贝libomp.so
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../output/lib/libomp.so
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBOMP_PATH}
              ${CMAKE_CURRENT_SOURCE_DIR}/../output/lib/libomp.so
      COMMENT "拷贝 libomp.so 到 output/lib")

    # 创建一个自定义目标来触发拷贝
    add_custom_target(
      copy_libomp ALL
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../output/lib/libomp.so)

    message("✅ 将在编译时自动拷贝 libomp.so 到 output/lib")
  else()
    message(WARNING "❌ 未找到 libomp.so，可能需要手动拷贝")
  endif()
endfunction()

# 调用拷贝函数
copy_ndk_libs()
