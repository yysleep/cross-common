/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <mutex>
#include <string>

#include "ILogger.h"

// 文件日志实现
class FileLogger : public ILogger {
 public:
  explicit FileLogger(const std::string &filePath);

  void log(LogLevel level,
           const std::string &tag,
           const std::string &message,
           const std::string &file,
           int line,
           pid_t threadId,
           const std::string &timeStr) override;

 private:
  std::string getLevelString(LogLevel level) const;

  std::string filePath_;
  std::mutex mutex_;
};
