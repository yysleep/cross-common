/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include <iostream>
#include <mutex>
#include <thread>

#include "logger/logger.h"

using namespace ifly;

#define TAG "SAMPLE"

int main(int argc, char **argv) {
  std::cout << "=== 基本日志输出测试 ===" << std::endl;
  ILOGD(TAG) << "这是一条调试信息";
  ILOGI(TAG) << "这是一条信息" << " 支持流式输出: " << 123;
  ILOGW(TAG) << "这是一条警告信息";
  ILOGE(TAG) << "这是一条错误信息";

  std::cout << "\n=== 注入文件日志测试 ===" << std::endl;
  // 添加文件日志
  auto fileLogger = log::createFileLogger("test.log");
  log::addLogger(fileLogger);

  ILOGI(TAG) << "这条信息会同时输出到控制台和文件";
  ILOGW(TAG) << "文件日志测试完成";

  std::cout << "\n=== 切换到纯文件日志测试 ===" << std::endl;
  // 切换到纯文件日志
  log::setLogger(fileLogger);
  ILOGI(TAG) << "这条信息只会输出到文件";

  std::cout << "\n=== 平台特定日志测试 ===" << std::endl;
  // 根据平台创建相应的日志实现
#ifdef __ANDROID__
  std::cout << "检测到Android平台，使用Android日志" << std::endl;
  auto platformLogger = log::createAndroidLogger();
  log::setLogger(platformLogger);
  ILOGI(TAG) << "Android平台日志测试";
#elif defined(__APPLE__)
  std::cout << "检测到iOS平台，使用iOS日志" << std::endl;
  auto platformLogger = log::createIOSLogger();
  log::setLogger(platformLogger);
  ILOGI(TAG) << "iOS平台日志测试";
#else
  std::cout << "其他平台，使用控制台日志" << std::endl;
  auto platformLogger = log::createConsoleLogger();
  log::setLogger(platformLogger);
  ILOGI(TAG) << "控制台日志测试";
#endif

  std::cout << "\n=== 多线程日志测试 ===" << std::endl;

  std::vector<std::thread> threads;
  for (int i = 0; i < 3; ++i) {
    threads.emplace_back([i]() {
      for (int j = 0; j < 3; ++j) {
        ILOGI(TAG) << "线程 " << i << " 消息 " << j;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  ILOGI(TAG) << " sleep 6s";
  std::cout << "sleep 6s" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(6000));

  std::cout << "\n=== 多日志输出测试 ===" << std::endl;
  // 同时输出到控制台和文件
  log::setLogger(log::createConsoleLogger());
  log::addLogger(log::createFileLogger("multi_output.log"));

  ILOGI(TAG) << "这条消息会输出到控制台和文件";
  ILOGW(TAG) << "多输出测试完成";

  // 清理
  log::destroyLogger();
  std::cout << "\n=== 测试完成 ===" << std::endl;

  return 0;
}