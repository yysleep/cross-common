/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "ILogger.h"
#include "logger/LoggerManager.h"

// 处理 `__FILE__`，提取文件名（去除路径）
inline std::string extractFileName(const std::string &filePath) {
  size_t pos = filePath.find_last_of("/\\");
  return (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
}

// 日志流类，用于支持流式输出
class LogStream : public std::ostringstream {
 public:
  LogStream(LogLevel level,
            const std::string &tag,
            const std::string &file,
            int line)
      : level_(level), tag_(tag), file_(file), line_(line) {
  }

  ~LogStream() {
    LoggerManager::getInstance().log(level_, tag_, this->str(), file_, line_);
  }

 private:
  LogLevel level_;
  std::string tag_;
  std::string file_;
  int line_;
};

// 定义日志宏，便于流式日志输出
#define ILOGI(tag) \
  LogStream(LogLevel::LOG_INFO, tag, extractFileName(__FILE__), __LINE__)
#define ILOGW(tag) \
  LogStream(LogLevel::LOG_WARN, tag, extractFileName(__FILE__), __LINE__)
#define ILOGE(tag) \
  LogStream(LogLevel::LOG_ERROR, tag, extractFileName(__FILE__), __LINE__)
#define ILOGD(tag) \
  LogStream(LogLevel::LOG_DEBUG, tag, extractFileName(__FILE__), __LINE__)

namespace ifly {
namespace log {
// 初始化日志系统
bool initLogger(const std::string &packageName, const std::string &path);

// 销毁日志系统
void destroyLogger();

// 设置日志实现
void setLogger(std::shared_ptr<ILogger> logger);

// 添加额外的日志实现
void addLogger(std::shared_ptr<ILogger> logger);

// 创建Android日志实现
std::shared_ptr<ILogger> createAndroidLogger();

// 创建iOS日志实现
std::shared_ptr<ILogger> createIOSLogger();

// 创建控制台日志实现
std::shared_ptr<ILogger> createConsoleLogger();

// 创建文件日志实现
std::shared_ptr<ILogger> createFileLogger(const std::string &filePath);

}  // namespace log
}  // namespace ifly
