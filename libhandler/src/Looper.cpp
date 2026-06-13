/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "handler/Looper.h"

#include <thread>

#include "handler/Message.h"
#include "logger/logger.h"

#define TAG "Looper"

Looper::Looper() {
  queueSp_ = std::make_shared<MessageQueue>(&looperMutex_);
  ILOGI(TAG) << "Looper() this=" << this;
}

Looper::~Looper() {
  ILOGI(TAG) << "~Looper() this=" << this;
}

void Looper::loop() noexcept {
  // std::cout << "Looper::loop() +" << std::endl;
  ILOGI(TAG) << "loop() + this=" << this;

  bool isQuitTemp = isQuit();
  if (!isQuitTemp) {
    hThreadState_ = LOOP_STATE_LOOPING;
  }

  if (isQuitTemp) {
    ILOGI(TAG) << "Looper::loop() - step one, this=" << this;
    return;
  }

  for (;;) {
    if (isQuit()) {
      hThreadState_ = LOOP_STATE_END;
      break;
    }

    std::shared_ptr<Message> msg = queueSp_->nextWithDelay();
    if (msg != nullptr) {
      std::shared_ptr<Runnable> task = msg->getTask();
      if (task != nullptr && !isQuit()) {
        task->run();
      }
    }
  }

  ILOGI(TAG) << "Looper::loop() - this=" << this;
}

void Looper::quit() {
  ILOGI(TAG) << "Looper::quit() + this=" << this;

  // 增加安全检查
  if (!queueSp_) {
    ILOGI(TAG) << "Looper::quit() ERROR: queueSp_ is null!";
    return;
  }

  try {
    queueSp_->quit();
    ILOGI(TAG) << "Looper::quit() queueSp_->quit() done";

    {
      std::lock_guard<std::mutex> lock(looperMutex_);
      hThreadState_ = LOOP_STATE_END;
      ILOGI(TAG) << "Looper::quit() state set to LOOP_STATE_END";
    }

    ILOGI(TAG) << "Looper::quit() - this=" << this;
  } catch (const std::exception &e) {
    ILOGI(TAG) << "Looper::quit() exception: " << e.what();
  } catch (...) {
    ILOGI(TAG) << "Looper::quit() unknown exception";
  }
}

bool Looper::isQuit() {
  if (!queueSp_) {
    return true;
  }
  return queueSp_->isQuit();
}