/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "logger/AndroidLogger.h"

#include <iostream>
#include <sstream>

void AndroidLogger::log(LogLevel level,
                        const std::string &tag,
                        const std::string &message,
                        const std::string &file,
                        int line,
                        pid_t threadId,
                        const std::string &timeStr) {
#ifdef __ANDROID__
  // 在Android环境下输出到Android日志系统
  std::ostringstream androidOss;
  androidOss << "A [" << file << ":" << line << "] " << message;
  android_LogPriority priority = getAndroidLogPriority(level);
  __android_log_print(priority, tag.c_str(), "%s", androidOss.str().c_str());
#endif

#ifdef ANDROID_LOGGER_FALLBACK_TO_STDOUT
  // 当定义了ANDROID_LOGGER_FALLBACK_TO_STDOUT时，输出到stdout（用于测试/调试）
  std::ostringstream fallbackOss;
  fallbackOss << " CA [" << file << ":" << line << "]" << message;
  std::cout << timeStr << " [" << threadId << "] " << tag << fallbackOss.str()
            << std::endl;
#endif
}

#ifdef __ANDROID__
android_LogPriority AndroidLogger::getAndroidLogPriority(LogLevel level) const {
  switch (level) {
    case LogLevel::LOG_DEBUG:
      return ANDROID_LOG_DEBUG;
    case LogLevel::LOG_INFO:
      return ANDROID_LOG_INFO;
    case LogLevel::LOG_WARN:
      return ANDROID_LOG_WARN;
    case LogLevel::LOG_ERROR:
      return ANDROID_LOG_ERROR;
    default:
      return ANDROID_LOG_INFO;
  }
}
#endif  // __ANDROID__