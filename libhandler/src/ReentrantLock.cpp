/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "handler/ReentrantLock.h"

#include <iostream>

ReentrantLock::ReentrantLock() {
  // std::cout << "ReentrantLock" << std::endl;
}

ReentrantLock::~ReentrantLock() {
  // std::cout << "~ReentrantLock" << std::endl;
}

void ReentrantLock::lock() {
  lockMutex_.lock();
}

void ReentrantLock::unlock() {
  lockMutex_.unlock();
}

void ReentrantLock::wait() {
  conditionAny_.wait(lockMutex_);
}

void ReentrantLock::wait_for(unsigned long _timeMs) {
  conditionAny_.wait_for(lockMutex_, std::chrono::milliseconds(_timeMs));
}

void ReentrantLock::notify() {
  conditionAny_.notify_one();
}

void ReentrantLock::notify_all() {
  conditionAny_.notify_all();
}