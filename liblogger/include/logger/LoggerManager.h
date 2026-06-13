/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <memory>
#include <vector>

#include "ILogger.h"

// 日志管理器（单例模式）
class LoggerManager {
 public:
  static LoggerManager &getInstance();

  // 设置日志实现
  void setLogger(std::shared_ptr<ILogger> logger);

  // 添加额外的日志实现（支持多个日志输出）
  void addLogger(std::shared_ptr<ILogger> logger);

  // 清除所有额外的日志实现
  void clearAdditionalLoggers();

  // 获取当前日志实现
  std::shared_ptr<ILogger> getLogger() const;

  // 日志输出
  void log(LogLevel level,
           const std::string &tag,
           const std::string &message,
           const std::string &file,
           int line);

 private:
  LoggerManager();

  std::shared_ptr<ILogger> logger_;
  std::vector<std::shared_ptr<ILogger>> additionalLoggers_;
};
