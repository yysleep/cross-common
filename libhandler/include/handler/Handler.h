/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <memory>

#include "handler/Looper.h"
#include "handler/Message.h"
#include "handler/Runnable.h"

class Handler {
 public:
  Handler();
  explicit Handler(const std::shared_ptr<Looper> &looperSp);
  virtual ~Handler();

  void post(const std::shared_ptr<Runnable> &runSp);
  void postDelayed(const std::shared_ptr<Runnable> &runSp, long delayMs);

  void sendMessage(const std::shared_ptr<Message> &msgSp);
  void sendMessageDelayed(const std::shared_ptr<Message> &msgSp, long delayMs);

  void removeCallbacksAndMessages();
  void removeMessages(int what);

 private:
  std::shared_ptr<Looper> looperSp_;
  std::shared_ptr<MessageQueue> queueSp_;
};
