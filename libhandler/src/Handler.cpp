/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "handler/Handler.h"

#include <iostream>
#include <memory>
#include <thread>

#include "handler/Message.h"

Handler::Handler(const std::shared_ptr<Looper> &looperSp) {
  // std::cout << "Handler()" << std::endl;
  looperSp_ = looperSp;
  if (looperSp_ != nullptr) {
    queueSp_ = looperSp_->queueSp_;
  }
}

Handler::~Handler() {
  // std::cout << "~Handler()" << std::endl;
}

void Handler::sendMessage(const std::shared_ptr<Message> &msgSp) {
  if (msgSp != nullptr && queueSp_ != nullptr) {
    queueSp_->putDelayed(msgSp, 0);
  }
}

void Handler::post(const std::shared_ptr<Runnable> &runSp) {
  sendMessage(std::make_shared<Message>(runSp));
}

void Handler::postDelayed(const std::shared_ptr<Runnable> &runSp,
                          long delayMs) {
  std::shared_ptr<Message> msgSp = std::make_shared<Message>(runSp);
  sendMessageDelayed(msgSp, delayMs);
}

void Handler::sendMessageDelayed(const std::shared_ptr<Message> &msgSp,
                                 long delayMs) {
  if (msgSp != nullptr && queueSp_ != nullptr) {
    queueSp_->putDelayed(msgSp, delayMs);
  }
}

void Handler::removeCallbacksAndMessages() {
  if (queueSp_ != nullptr) {
    queueSp_->removeCallbacksAndMessages();
  }
}

void Handler::removeMessages(int what) {
  if (queueSp_ != nullptr) {
    queueSp_->removeMessages(what);
  }
}