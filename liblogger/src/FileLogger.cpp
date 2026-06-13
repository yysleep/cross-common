/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "logger/FileLogger.h"

#include <fstream>

FileLogger::FileLogger(const std::string &filePath) : filePath_(filePath) {
}

void FileLogger::log(LogLevel level,
                     const std::string &tag,
                     const std::string &message,
                     const std::string &file,
                     int line,
                     pid_t threadId,
                     const std::string &timeStr) {
  std::lock_guard<std::mutex> lock(mutex_);

  std::string levelStr = getLevelString(level);

  std::ofstream logFile(filePath_, std::ios::app);
  if (logFile.is_open()) {
    logFile << timeStr << " F[" << threadId << "] [" << file << ":" << line
            << "] " << "[" << levelStr << "] " << tag << " " << message
            << std::endl;
    logFile.close();
  }
}

std::string FileLogger::getLevelString(LogLevel level) const {
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