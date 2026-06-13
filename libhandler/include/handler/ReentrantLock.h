/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <condition_variable>
#include <mutex>

class ReentrantLock {
 public:
  ReentrantLock();
  virtual ~ReentrantLock();
  void lock();
  void unlock();
  void wait();
  void wait_for(unsigned long _timeMs);
  void notify();
  void notify_all();

 private:
  std::recursive_mutex lockMutex_;
  std::condition_variable_any conditionAny_;
};
