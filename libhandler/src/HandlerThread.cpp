/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "handler/HandlerThread.h"

#include "logger/logger.h"

#define TAG "HandlerThread"

HandlerThread::HandlerThread(const std::string &name) {
  ILOGI(TAG) << "HandlerThread";
  threadName_ = name;
}

HandlerThread::~HandlerThread() {
  ILOGI(TAG) << "~HandlerThread +";
  quit();
  ILOGI(TAG) << "~HandlerThread -";
}

void HandlerThread::start() {
  ILOGI(TAG) << "HandlerThread::start()" << " threadName=" << threadName_;
  if (nullptr != looperSp_) {
    return;
  }

  looperSp_ = std::make_shared<Looper>();
  auto fun = [](std::shared_ptr<Looper> looper) noexcept { looper->loop(); };
  looperThreadUp_ = std::make_unique<std::thread>(fun, looperSp_);
}

void HandlerThread::quit() {
  ILOGI(TAG) << "HandlerThread::quit() +";
  if (looperSp_ != nullptr) {
    looperSp_->quit();

    // 等待线程结束
    if (looperThreadUp_ && looperThreadUp_->joinable()) {
      looperThreadUp_->join();
    }

    looperThreadUp_.reset();
    looperSp_.reset();
    looperSp_ = nullptr;
  }
  ILOGI(TAG) << "HandlerThread::quit() -";
}

std::shared_ptr<Looper> HandlerThread::getLooper() {
  return looperSp_;
}

bool HandlerThread::compareAndSet(H_THREAD_STATE compareValue,
                                  H_THREAD_STATE newValue) {
  ILOGI(TAG) << "compareAndSet() hThreadState_=" << hThreadState_
             << ", compareValue=" << compareValue;
  if (hThreadState_ != compareValue) {
    return false;
  }

  return true;
}