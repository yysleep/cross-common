/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <memory>
#include <string>
#include <thread>

#include "handler/Looper.h"

enum H_THREAD_STATE
{
  H_STATUS_NONE,
  H_STATUS_START,
  H_STATUS_RELESE
};

class HandlerThread {
 public:
  explicit HandlerThread(const std::string &name);

  virtual ~HandlerThread();

  void start();

  void quit();

  std::shared_ptr<Looper> getLooper();

 private:
  bool compareAndSet(H_THREAD_STATE compapeValue, H_THREAD_STATE newValue);

 private:
  std::shared_ptr<Looper> looperSp_;
  std::unique_ptr<std::thread> looperThreadUp_;
  int hThreadState_;
  std::string threadName_;
};
