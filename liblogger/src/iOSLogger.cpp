/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "logger/iOSLogger.h"

#ifdef __APPLE__
#include <os/log.h>
#include <pthread.h>

#include <iostream>

static inline uint32_t currentThreadId() {
  return pthread_mach_thread_np(pthread_self());
}

os_log_type_t iOSLogger::getOSLogType(LogLevel level) const {
  switch (level) {
    case LogLevel::LOG_DEBUG:
      return OS_LOG_TYPE_DEBUG;
    case LogLevel::LOG_INFO:
      return OS_LOG_TYPE_INFO;
    case LogLevel::LOG_WARN:
      return OS_LOG_TYPE_DEFAULT;
    case LogLevel::LOG_ERROR:
      return OS_LOG_TYPE_ERROR;
    default:
      return OS_LOG_TYPE_INFO;
  }
}

void iOSLogger::log(LogLevel level,
                    const std::string &tag,
                    const std::string &message,
                    const std::string &file,
                    int line,
                    pid_t /*threadId_param_unused*/,
                    const std::string &timeStr) {
  os_log_type_t logType = getOSLogType(level);

  os_log_t category =
      tag.empty() ? OS_LOG_DEFAULT : os_log_create("", tag.c_str());

  // 仅打印必要信息，并标注 public 避免被隐私折叠
  std::string realMsg = "iOS: " + message;
  os_log_with_type(category,
                   logType,
                   "%{public}s [%{public}s:%d] [tid:%u] %{public}s",
                   timeStr.c_str(),
                   file.c_str(),
                   line,
                   currentThreadId(),
                   realMsg.c_str());

#if DEBUG
// 调试期可同时输出到控制台
// std::cout << "[" << file << ":" << line << "] [tid:" << currentThreadId()
// << "] " << message << std::endl;
#endif
}

#endif  // __APPLE__