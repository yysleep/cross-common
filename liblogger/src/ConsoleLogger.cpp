/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "logger/ConsoleLogger.h"

void ConsoleLogger::log(LogLevel level,
                        const std::string &tag,
                        const std::string &message,
                        const std::string &file,
                        int line,
                        pid_t threadId,
                        const std::string &timeStr) {
  std::string levelStr = getLevelString(level);

  std::cout << timeStr << " C[" << threadId << "] [" << file << ":" << line
            << "] " << "[" << levelStr << "] " << tag << " " << message
            << std::endl;
}

std::string ConsoleLogger::getLevelString(LogLevel level) const {
  switch (level) {
    case LogLevel::LOG_DEBUG:
      return "DEBUG";
    case LogLevel::LOG_INFO:
      return "INFO";
    case LogLevel::LOG_WARN:
      return "WARN";
    case LogLevel::LOG_ERROR:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}