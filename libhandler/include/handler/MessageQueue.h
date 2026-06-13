/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include "handler/Message.h"

class MessageQueue {
 public:
  explicit MessageQueue(std::mutex *mutexPtr);
  virtual ~MessageQueue();
  void putDelayed(const std::shared_ptr<Message> &msgSp, long delayMs);
  std::shared_ptr<Message> nextWithDelay();
  void removeCallbacksAndMessages();
  void removeMessages(int what);
  void quit();
  bool isQuit();
  int size();

 private:
  struct DelayedMessage {
    std::shared_ptr<Message> msgSp;
    std::chrono::steady_clock::time_point executeAt;
    bool operator<(const DelayedMessage &other) const {
      return executeAt > other.executeAt;  // 小顶堆
    }
  };
  std::mutex *mutexPtr_;
  std::condition_variable condition_;
  std::priority_queue<DelayedMessage> delayQueue_;
  bool isQuit_ = false;
};
