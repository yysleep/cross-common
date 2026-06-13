# Linux platform specific settings

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)

# 设置编译器
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# 全局追加 pthread 编译+链接选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")