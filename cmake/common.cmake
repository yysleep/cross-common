# 设定 C++ 版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置库类型，检查是否已设置
if(NOT DEFINED LIB_TYPE)
  set(LIB_TYPE SHARED)
  message(STATUS "LIB_TYPE not defined, setting to default: ${LIB_TYPE}")
else()
  message(STATUS "LIB_TYPE is already set to: ${LIB_TYPE}")
endif()

# 引入符合 GNU 规范的 include/lib 文件夹布局作为包含文件
include(GNUInstallDirs)

# 自动生成 compile_commands.json 文件，用于 IDE 代码提示和跳转
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 定义输出库目录，供库路径解析使用
set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../output)
# 根据构建类型与文件存在性解析库实际路径（动态优先或静态优先由 BUILD_SHARED_LIBS 控制）
function(resolve_lib_path OUT_VAR BASE_NAME)
  set(SHARED_PATH ${OUTPUT_DIR}/${BASE_NAME}/lib/lib${BASE_NAME}.so)
  set(STATIC_PATH ${OUTPUT_DIR}/${BASE_NAME}/lib/lib${BASE_NAME}.a)

  if(${LIB_TYPE} STREQUAL "STATIC")
    set(${OUT_VAR}
        ${STATIC_PATH}
        PARENT_SCOPE)
  else()
    set(${OUT_VAR}
        ${SHARED_PATH}
        PARENT_SCOPE)
  endif()

endfunction()

# baseengine库
set(THIRD_PARTY_ENGINE_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libbaseengine/include)
resolve_lib_path(THIRD_PARTY_ENGINE_LIB "baseengine")
message(STATUS "THIRD_PARTY_ENGINE_LIB=${THIRD_PARTY_ENGINE_LIB}")

# buffer库
set(THIRD_PARTY_BUFFER_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../libbuffer/include)
resolve_lib_path(THIRD_PARTY_BUFFER_LIB "buffer")
message(STATUS "THIRD_PARTY_BUFFER_LIB=${THIRD_PARTY_BUFFER_LIB}")

# core基础库
set(THIRD_PARTY_CORE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../libcore/include)
resolve_lib_path(THIRD_PARTY_CORE_LIB "core")
message(STATUS "THIRD_PARTY_CORE_LIB=${THIRD_PARTY_CORE_LIB}")

# crashcollect库
set(THIRD_PARTY_CRASHCOLLECT_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libcrashcollect/include)
resolve_lib_path(THIRD_PARTY_CRASHCOLLECT_LIB "crashcollect")
message(STATUS "THIRD_PARTY_CRASHCOLLECT_LIB=${THIRD_PARTY_CRASHCOLLECT_LIB}")

# esrEngine库
set(THIRD_PARTY_ESR_ENGINE_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libesrengine/include)
resolve_lib_path(THIRD_PARTY_ESR_ENGINE_LIB "esrengine")
message(STATUS "THIRD_PARTY_ESR_ENGINE_LIB=${THIRD_PARTY_ESR_ENGINE_LIB}")

# handler库
set(THIRD_PARTY_HANDLER_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libhandler/include)
resolve_lib_path(THIRD_PARTY_HANDLER_LIB "handler")
message(STATUS "THIRD_PARTY_HANDLER_LIB=${THIRD_PARTY_HANDLER_LIB}")

# json库
set(THIRD_PARTY_JSON_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libjson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../libjson/third_party/include)
resolve_lib_path(THIRD_PARTY_JSON_LIB "json")
message(STATUS "THIRD_PARTY_JSON_LIB=${THIRD_PARTY_JSON_LIB}")

# lidEngine库
set(THIRD_PARTY_LID_ENGINE_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../liblidengine/include)
resolve_lib_path(THIRD_PARTY_LID_ENGINE_LIB "lidengine")
message(STATUS "THIRD_PARTY_LID_ENGINE_LIB=${THIRD_PARTY_LID_ENGINE_LIB}")

# logger 日志库
set(THIRD_PARTY_LOGGER_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../liblogger/include)
resolve_lib_path(THIRD_PARTY_LOGGER_LIB "logger")
message(STATUS "THIRD_PARTY_LOGGER_LIB=${THIRD_PARTY_LOGGER_LIB}")

# security库
set(THIRD_PARTY_SECURITY_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libsecurity/include)
resolve_lib_path(THIRD_PARTY_SECURITY_LIB "security")
message(STATUS "THIRD_PARTY_SECURITY_LIB=${THIRD_PARTY_SECURITY_LIB}")

# trsEngine库
set(THIRD_PARTY_TRS_ENGINE_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libtrsengine/include)
resolve_lib_path(THIRD_PARTY_TRS_ENGINE_LIB "trsengine")
message(STATUS "THIRD_PARTY_TRS_ENGINE_LIB=${THIRD_PARTY_TRS_ENGINE_LIB}")

# ttsEngine库
set(THIRD_PARTY_TTS_ENGINE_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libttsengine/include)
resolve_lib_path(THIRD_PARTY_TTS_ENGINE_LIB "ttsengine")
message(STATUS "THIRD_PARTY_TTS_ENGINE_LIB=${THIRD_PARTY_TTS_ENGINE_LIB}")

#
# 编译产物到output目录下
#
set(CMAKE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY}/lib)
