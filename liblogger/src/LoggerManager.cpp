/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "logger/LoggerManager.h"

#include <chrono>
#include <iomanip>
#include <sstream>

#include "logger/ConsoleLogger.h"
#include "logger/logger.h"

#define TAG "LoggerManager"

// 获取当前时间字符串（精确到毫秒）
std::string getCurrentTime() {
  using namespace std::chrono;
  auto now = system_clock::now();
  auto time_t_now = system_clock::to_time_t(now);
  auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

  std::ostringstream oss;
  oss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S") << '.'
      << std::setfill('0') << std::setw(3) << ms.count();
  return oss.str();
}

LoggerManager &LoggerManager::getInstance() {
  static LoggerManager instance;
  return instance;
}

void LoggerManager::setLogger(std::shared_ptr<ILogger> logger) {
  logger_ = logger;
}

void LoggerManager::addLogger(std::shared_ptr<ILogger> logger) {
  additionalLoggers_.push_back(logger);
}

void LoggerManager::clearAdditionalLoggers() {
  additionalLoggers_.clear();
}

std::shared_ptr<ILogger> LoggerManager::getLogger() const {
  return logger_;
}

void LoggerManager::log(LogLevel level,
                        const std::string &tag,
                        const std::string &message,
                        const std::string &file,
                        int line) {
  pid_t threadId = getThreadId();
  std::string timeStr = getCurrentTime();

  // 主日志输出
  if (logger_) {
    logger_->log(level, tag, message, file, line, threadId, timeStr);
  }

  // 额外日志输出
  for (auto &additionalLogger : additionalLoggers_) {
    if (additionalLogger) {
      additionalLogger->log(level, tag, message, file, line, threadId, timeStr);
    }
  }
}

LoggerManager::LoggerManager() {
  std::cout << "LoggerManager()" << std::endl;

#ifdef __ANDROID__
  std::cout << "LoggerManager() 检测到Android平台，使用Android日志"
            << std::endl;
  auto platformLogger = ifly::log::createAndroidLogger();
  setLogger(platformLogger);

#elif defined(__APPLE__)
  std::cout << "LoggerManager() 检测到iOS平台，使用iOS日志" << std::endl;
  auto platformLogger = ifly::log::createIOSLogger();
  setLogger(platformLogger);

#else
  std::cout << "LoggerManager() 其他平台，使用控制台日志" << std::endl;
  auto platformLogger = ifly::log::createConsoleLogger();
  setLogger(platformLogger);
#endif
}