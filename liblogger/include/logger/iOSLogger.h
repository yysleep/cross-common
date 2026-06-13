/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include "ILogger.h"

#ifdef __APPLE__
#include <os/log.h>

// iOS日志实现
class iOSLogger : public ILogger {
 public:
  void log(LogLevel level,
           const std::string &tag,
           const std::string &message,
           const std::string &file,
           int line,
           pid_t threadId,
           const std::string &timeStr) override;

 private:
  os_log_type_t getOSLogType(LogLevel level) const;
};

#endif  // __APPLE__
