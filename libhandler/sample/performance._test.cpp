/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>

#include <atomic>
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "handler/Handler.h"
#include "handler/HandlerThread.h"
#include "logger/logger.h"

class PerformanceTest {
 private:
  std::shared_ptr<HandlerThread> testThread;
  std::shared_ptr<Handler> testHandler;

  // 线程安全的消息存储map
  std::map<int, int> messageMap;
  std::mutex mapMutex;

  // 统计信息
  std::atomic<int> totalSent{0};
  std::atomic<int> totalReceived{0};
  std::atomic<int> totalProcessed{0};

  // 测试开始时间
  std::chrono::steady_clock::time_point startTime;

 public:
  void init() {
    ILOGI("PerformanceTest") << "初始化Handler线程";
    testThread = std::make_shared<HandlerThread>("performance_test");
    testThread->start();
    testHandler = std::make_shared<Handler>(std::move(testThread->getLooper()));
    startTime = std::chrono::steady_clock::now();
  }

  void cleanup() {
    if (testThread) {
      ILOGI("PerformanceTest") << "清理资源";
      testThread->quit();
      testHandler = nullptr;
      testThread = nullptr;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  // 发送消息并存储到map
  void sendMessage(int messageId, int moduleId) {
    {
      std::lock_guard<std::mutex> lock(mapMutex);
      messageMap[messageId] = moduleId;
    }

    auto runnable = std::make_shared<Runnable>();
    auto msg = std::make_shared<Message>(runnable);
    msg->what = messageId;

    runnable->run = [this, messageId, moduleId]() {
      this->handleMessage(messageId, moduleId);
    };

    testHandler->sendMessage(msg);
    totalSent++;
  }

  // 处理消息，从map中移除并可能发送新消息
  void handleMessage(int messageId, int moduleId) {
    totalReceived++;

    bool found = false;
    {
      std::lock_guard<std::mutex> lock(mapMutex);
      auto it = messageMap.find(messageId);
      if (it != messageMap.end()) {
        messageMap.erase(it);
        found = true;
      }
    }

    if (found) {
      totalProcessed++;
      // 模块6：在收到消息时发送新消息（递归发送）
      if (moduleId == 60000 &&
          messageId < 60100) {  // 只发送100条递归消息避免无限循环
        int newMessageId = messageId + 1;
        sendMessage(newMessageId, moduleId);
      }
    }

    // 每处理100条消息输出一次统计
    if (totalProcessed % 100 == 0) {
      printStats();
    }
  }

  // 打印统计信息
  void printStats() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - startTime);

    int mapSize = 0;
    {
      std::lock_guard<std::mutex> lock(mapMutex);
      mapSize = messageMap.size();
    }

    ILOGI("PerformanceTest")
        << "统计信息 - 发送:" << totalSent.load()
        << " 接收:" << totalReceived.load() << " 处理:" << totalProcessed.load()
        << " Map剩余:" << mapSize << " 耗时:" << elapsed.count() << "ms";
  }

  // 模块1-5：普通发送线程
  void senderThread(int moduleId, int startId) {
    ILOGI("PerformanceTest")
        << "启动发送线程 模块ID:" << moduleId << " 起始ID:" << startId;

    for (int i = 0; i < 100; i++) {
      int messageId = startId + i;
      sendMessage(messageId, moduleId);

      // 添加小延迟避免过于密集
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ILOGI("PerformanceTest") << "发送线程完成 模块ID:" << moduleId;
  }

  // 模块6：特殊发送任务（会在收到消息时继续发送）
  void recursiveSenderTask(int moduleId, int startId) {
    ILOGI("PerformanceTest")
        << "启动递归发送任务 模块ID:" << moduleId << " 起始ID:" << startId;

    // 初始发送第一条消息，后续消息将在handleMessage中发送
    sendMessage(startId, moduleId);

    ILOGI("PerformanceTest") << "递归发送任务初始化完成 模块ID:" << moduleId;
  }

  void runTest() {
    ILOGI("PerformanceTest") << "=== 开始性能测试 ===";

    std::vector<std::thread> senderThreads;

    // 启动5个发送线程（模块1-5）
    for (int i = 1; i <= 5; i++) {
      int moduleId = i * 10000;
      senderThreads.emplace_back(
          [this, moduleId, i]() { this->senderThread(moduleId, moduleId); });
    }

    // 启动递归发送任务（模块6）
    std::thread recursiveThread(
        [this]() { this->recursiveSenderTask(60000, 60000); });

    // 等待所有发送线程完成
    for (auto &t : senderThreads) {
      t.join();
    }
    recursiveThread.join();

    ILOGI("PerformanceTest") << "所有发送线程已完成，等待消息处理完成...";

    // 等待所有消息处理完成
    int lastProcessed = -1;
    int stableCount = 0;
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));

      int currentProcessed = totalProcessed.load();
      int mapSize = 0;
      {
        std::lock_guard<std::mutex> lock(mapMutex);
        mapSize = messageMap.size();
      }

      printStats();

      // 如果处理数量不再变化且map为空，认为完成
      if (currentProcessed == lastProcessed && mapSize == 0) {
        stableCount++;
        if (stableCount >= 3) {  // 连续3次检查都稳定才退出
          break;
        }
      } else {
        stableCount = 0;
      }
      lastProcessed = currentProcessed;
    }

    ILOGI("PerformanceTest") << "=== 性能测试完成 ===";
    printFinalStats();
  }

  void printFinalStats() {
    auto endTime = std::chrono::steady_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime);

    int finalMapSize = 0;
    {
      std::lock_guard<std::mutex> lock(mapMutex);
      finalMapSize = messageMap.size();
    }

    ILOGI("PerformanceTest") << "=== 最终统计 ===";
    ILOGI("PerformanceTest") << "总发送消息数: " << totalSent.load();
    ILOGI("PerformanceTest") << "总接收消息数: " << totalReceived.load();
    ILOGI("PerformanceTest") << "总处理消息数: " << totalProcessed.load();
    ILOGI("PerformanceTest") << "Map中剩余消息: " << finalMapSize;
    ILOGI("PerformanceTest") << "总耗时: " << totalTime.count() << "ms";

    if (totalSent.load() > 0) {
      double throughput = static_cast<double>(totalProcessed.load()) * 1000.0 /
                          totalTime.count();
      ILOGI("PerformanceTest") << "平均吞吐量: " << throughput << " 消息/秒";
    }

    // 检查是否有未处理的消息
    if (finalMapSize > 0) {
      ILOGW("PerformanceTest")
          << "警告: 仍有 " << finalMapSize << " 条消息未被处理";
    }

    if (totalSent.load() == totalReceived.load() &&
        totalReceived.load() == totalProcessed.load()) {
      ILOGI("PerformanceTest") << "✓ 消息收发一致性检查通过";
    } else {
      ILOGE("PerformanceTest") << "✗ 消息收发不一致！";
    }
  }
};

int main(int argc, char *argv[]) {
  ILOGI("main") << "=== libhandler 性能测试开始 ===";

  PerformanceTest test;

  try {
    test.init();
    test.runTest();
    test.cleanup();
  } catch (const std::exception &e) {
    ILOGE("main") << "测试过程中发生异常: " << e.what();
    test.cleanup();
    return EXIT_FAILURE;
  }

  ILOGI("main") << "=== libhandler 性能测试结束 ===";
  return EXIT_SUCCESS;
}