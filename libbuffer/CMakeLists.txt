cmake_minimum_required(VERSION 3.10)

# 根据 BUILD_PLATFORM 选择合适的配置文件
if(NOT DEFINED BUILD_PLATFORM)
  message(
    FATAL_ERROR
      "BUILD_PLATFORM not defined. Please specify the platform (android/linux/r818/ios/macos)."
  )
endif()

message("BUILD_PLATFORM is " ${BUILD_PLATFORM})

if(${BUILD_PLATFORM} STREQUAL "android")
  include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/android.cmake)
elseif(${BUILD_PLATFORM} STREQUAL "linux")
  include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/linux.cmake)
elseif(${BUILD_PLATFORM} STREQUAL "r818")
  include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/r818.cmake)
elseif(${BUILD_PLATFORM} STREQUAL "ios")
  include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ios.cmake)
elseif(${BUILD_PLATFORM} STREQUAL "macos")
  include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/macos.cmake)
else()
  message(FATAL_ERROR "Unknown BUILD_PLATFORM: ${BUILD_PLATFORM}")
  include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)
endif()

project(buffer)

message("libbuffer build begin")

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_BINARY_DIR})

# logger库
include_directories(${THIRD_PARTY_LOGGER_INCLUDE})

# 代码路径
aux_source_directory(${PROJECT_SOURCE_DIR} DIR_SRC)

# 生成库
add_library(${PROJECT_NAME} ${LIB_TYPE} ${DIR_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC ${THIRD_PARTY_LOGGER_LIB})

option(OPTION_WITH_${PROJECT_NAME}_TEST "with sample apps" OFF)

if(OPTION_WITH_${PROJECT_NAME}_TEST)
  message("build with ${PROJECT_NAME} sample apps")

  # 生成可执行的文件sample
  add_executable(test_${PROJECT_NAME} sample/sample.cpp)
  target_link_libraries(test_${PROJECT_NAME} ${PROJECT_NAME})
  target_link_libraries(test_${PROJECT_NAME} ${THIRD_PARTY_LOGGER_LIB})

endif()

# 设置输出目录
set(CMAKE_PRODUCT_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}/${PROJECT_NAME})

# 添加后构建步骤，复制头文件和库文件到输出目录
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  # 删除目标目录
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/include
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/lib
  COMMAND ${CMAKE_COMMAND} -E remove_directory
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/bin
  # 创建目标目录
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/include
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/lib
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/bin
  # 创建全局 include 目录
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_OUTPUT_DIRECTORY}/include
  # 拷贝头文件
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/include
  # 额外拷贝到全局 include 目录
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include
          ${CMAKE_OUTPUT_DIRECTORY}/include
  # 拷贝库文件（仅拷贝当前项目的库文件，自动兼容动态库和静态库）
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}>
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/lib/
  # 拷贝依赖的第三方库文件
  COMMAND ${CMAKE_COMMAND} -E copy ${THIRD_PARTY_LOGGER_LIB}
          ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/lib/
  COMMENT "Copying header files and library files to output directory")

# 如果构建了样例应用，也拷贝可执行文件到bin目录
if(OPTION_WITH_${PROJECT_NAME}_TEST)
  add_custom_command(
    TARGET test_${PROJECT_NAME}
    POST_BUILD
    # 拷贝可执行文件
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test_${PROJECT_NAME}>
            ${CMAKE_PRODUCT_OUTPUT_DIRECTORY}/bin/
    COMMENT "Copying executable files to bin directory")
endif()
