/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <sys/syscall.h>
#include <unistd.h>

#include <string>

// 日志级别枚举
enum class LogLevel
{
  LOG_DEBUG = 0,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
};

// 获取线程号
inline pid_t getThreadId() {
  return static_cast<pid_t>(syscall(SYS_gettid));
}

// 抽象日志接口
class ILogger {
 public:
  virtual ~ILogger() = default;
  virtual void log(LogLevel level,
                   const std::string &tag,
                   const std::string &message,
                   const std::string &file,
                   int line,
                   pid_t threadId,
                   const std::string &timeStr) = 0;
};
