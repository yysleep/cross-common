include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)

# ==== 交叉编译环境配置
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_SYSTEM_PROCESSOR aarch64)
if(NOT DEFINED IBUILD_GCC_PREBUILD_TOOL_PATH)
  message("not define IBUILD_GCC_PREBUILD_TOOL_PATH")
  set(TOOL_CHAIN_PATH
      $ENV{HOME}/toolchain/prebuilt/gcc/linux-x86/aarch64/toolchain-sunxi-glibc/toolchain
  )
else()
  message("define IBUILD_GCC_PREBUILD_TOOL_PATH value is "
          ${IBUILD_GCC_PREBUILD_TOOL_PATH})
  set(TOOL_CHAIN_PATH ${IBUILD_GCC_PREBUILD_TOOL_PATH})
endif()

set(CMAKE_TOOLCHAIN_PREFIX aarch64-openwrt-linux-gnu-)

set(ENV{STAGING_DIR} ${TOOL_CHAIN_PATH}/staging_dir)

set(arch armv8-a)

set(CMAKE_C_COMPILER_TARGET ${arch})
set(CMAKE_CXX_COMPILER_TARGET ${arch})

set(CMAKE_C_COMPILER ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}g++)
set(CMAKE_C_FLAGS "-ldl -fopenmp -fPIC")
set(CMAKE_CXX_FLAGS "-ldl -fopenmp -fPIC")
set(CMAKE_AR ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}ar)

set(CMAKE_LINKER ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}ld)
set(CMAKE_NM ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}nm)
set(CMAKE_OBJCOPY ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}objdump)
set(CMAKE_RANLIB ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}ranlib)
set(CMAKE_STRIP ${TOOL_CHAIN_PATH}/bin/${CMAKE_TOOLCHAIN_PREFIX}strip)

set(CMAKE_SYSROOT ${TOOL_CHAIN_PATH})
set(CMAKE_FIND_ROOT_PATH ${TOOL_CHAIN_PATH})
set(CROSS_COMPILE_ARCH "aarch64")

# 对FIND_PROGRAM()起作用，有三种取值，NEVER,ONLY,BOTH,第一个表示不在你CMAKE_FIND_ROOT_PATH下进行查找，第二个表示只在这个路径下查找，第三个表示先查找这个路径，再查找全局路径，对于这个变量来说，一般都是调用宿主机的程序，所以一般都设置成NEVER
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# 下面的三个选项表示只在交叉环境中查找库和头文件
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set_property(GLOBAL PROPERTY product_name_property "R818")

enable_language(C) # 启用 C 语言支持
enable_language(CXX) # 启用 C++ 语言支持

# ====
