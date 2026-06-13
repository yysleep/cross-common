/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "logger/logger.h"

#include <iostream>

#include "logger/AndroidLogger.h"
#include "logger/ConsoleLogger.h"
#include "logger/FileLogger.h"
#include "logger/iOSLogger.h"

namespace ifly {
namespace log {
bool initLogger(const std::string &packageName, const std::string &path) {
  // 可以根据需要进行初始化设置
  // 例如：设置日志文件路径、初始化特定平台的日志系统等
  return true;
}

void destroyLogger() {
  // 清理资源
  LoggerManager::getInstance().clearAdditionalLoggers();
}

void setLogger(std::shared_ptr<ILogger> logger) {
  LoggerManager::getInstance().setLogger(logger);
}

void addLogger(std::shared_ptr<ILogger> logger) {
  LoggerManager::getInstance().addLogger(logger);
}

std::shared_ptr<ILogger> createAndroidLogger() {
#ifdef __ANDROID__
  std::cout << "logger.cpp: __ANDROID__ 已定义，创建AndroidLogger" << std::endl;
  return std::make_shared<AndroidLogger>();
#else
  std::cout << "logger.cpp: __ANDROID__ 未定义，创建ConsoleLogger作为替代"
            << std::endl;
  // 如果不是Android平台，返回控制台日志作为替代
  return std::make_shared<ConsoleLogger>();
#endif
}

std::shared_ptr<ILogger> createIOSLogger() {
#ifdef __APPLE__
  std::cout << "logger.cpp: __APPLE__ 已定义，创建iOSLogger" << std::endl;
  return std::make_shared<iOSLogger>();
#else
  std::cout << "logger.cpp: __APPLE__ 未定义，创建ConsoleLogger作为替代"
            << std::endl;
  // 如果不是iOS平台，返回控制台日志作为替代
  return std::make_shared<ConsoleLogger>();
#endif
}

std::shared_ptr<ILogger> createConsoleLogger() {
  return std::make_shared<ConsoleLogger>();
}

std::shared_ptr<ILogger> createFileLogger(const std::string &filePath) {
  return std::make_shared<FileLogger>(filePath);
}

}  // namespace log

}  // namespace ifly