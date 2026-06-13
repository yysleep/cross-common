/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include "ILogger.h"

#ifdef __ANDROID__
#include <android/log.h>
#endif

// Android日志实现
class AndroidLogger : public ILogger {
 public:
  void log(LogLevel level,
           const std::string &tag,
           const std::string &message,
           const std::string &file,
           int line,
           pid_t threadId,
           const std::string &timeStr) override;

 private:
#ifdef __ANDROID__
  android_LogPriority getAndroidLogPriority(LogLevel level) const;
#endif
};
