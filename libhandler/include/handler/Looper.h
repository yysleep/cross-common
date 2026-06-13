/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <memory>
#include <mutex>

#include "handler/MessageQueue.h"

enum LOOPER_STATE
{
  LOOP_STATE_NONE,
  LOOP_STATE_LOOPING,
  LOOP_STATE_END,
};

class Handler;
class Looper {
  friend class Handler;

 public:
  Looper();
  virtual ~Looper();
  void loop() noexcept;
  void quit();
  bool isQuit();

 private:
  std::shared_ptr<MessageQueue> queueSp_;
  std::mutex looperMutex_;
  LOOPER_STATE hThreadState_ = LOOP_STATE_NONE;
};
