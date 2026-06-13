/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "handler/MessageQueue.h"

#include <chrono>
#include <thread>

#include "logger/logger.h"

#define TAG "MessageQueue"

MessageQueue::MessageQueue(std::mutex *mutexPtr) : mutexPtr_(mutexPtr) {
}

MessageQueue::~MessageQueue() {
}

void MessageQueue::putDelayed(const std::shared_ptr<Message> &msgSp,
                              long delayMs) {
  if (nullptr == msgSp) return;

  if (isQuit_) return;

  {
    std::lock_guard<std::mutex> lock(*mutexPtr_);
    if (!isQuit_) {
      DelayedMessage delayedMsg;
      delayedMsg.msgSp = msgSp;
      delayedMsg.executeAt =
          std::chrono::steady_clock::now() + std::chrono::milliseconds(delayMs);
      delayQueue_.push(delayedMsg);
      condition_.notify_all();
    }
  }
}

std::shared_ptr<Message> MessageQueue::nextWithDelay() {
  std::unique_lock<std::mutex> lock(*mutexPtr_);
  while (!isQuit_) {
    if (delayQueue_.empty()) {
      condition_.wait(lock);
      continue;
    }

    auto &top = delayQueue_.top();
    auto now = std::chrono::steady_clock::now();
    if (top.executeAt <= now) {
      std::shared_ptr<Message> msgSp = top.msgSp;
      delayQueue_.pop();
      return msgSp;
    }

    auto waitTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                        top.executeAt - now)
                        .count();
    if (waitTime > 0) {
      condition_.wait_for(lock, std::chrono::milliseconds(waitTime));
    }
  }
  return nullptr;
}

void MessageQueue::removeCallbacksAndMessages() {
  std::lock_guard<std::mutex> lock(*mutexPtr_);
  while (!delayQueue_.empty()) {
    delayQueue_.pop();
  }
}

void MessageQueue::removeMessages(int what) {
  std::lock_guard<std::mutex> lock(*mutexPtr_);
  std::vector<DelayedMessage> remain;
  while (!delayQueue_.empty()) {
    auto msg = delayQueue_.top();
    delayQueue_.pop();
    if (msg.msgSp && msg.msgSp->what != what) {
      remain.push_back(msg);
    }
  }

  for (auto &msg : remain) {
    delayQueue_.push(msg);
  }
}

void MessageQueue::quit() {
  ILOGI(TAG) << "quit() + isQuit=" << isQuit_;
  if (isQuit_) {
    ILOGI(TAG) << "quit() - step1";
    return;
  }

  isQuit_ = true;
  {
    std::lock_guard<std::mutex> lock(*mutexPtr_);
    while (!delayQueue_.empty()) {
      delayQueue_.pop();
    }
    condition_.notify_all();
  }
  ILOGI(TAG) << "quit() -";
}

bool MessageQueue::isQuit() {
  return isQuit_;
}

int MessageQueue::size() {
  std::lock_guard<std::mutex> lock(*mutexPtr_);
  return delayQueue_.size();
}