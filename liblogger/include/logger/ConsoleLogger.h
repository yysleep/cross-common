/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <iostream>
#include <sstream>

#include "ILogger.h"

// 默认控制台日志实现
class ConsoleLogger : public ILogger {
 public:
  void log(LogLevel level,
           const std::string &tag,
           const std::string &message,
           const std::string &file,
           int line,
           pid_t threadId,
           const std::string &timeStr) override;

 private:
  std::string getLevelString(LogLevel level) const;
};
