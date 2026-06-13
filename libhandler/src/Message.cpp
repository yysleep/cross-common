/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "handler/Message.h"

#include <iostream>

Message::Message() : runTaskSp_(nullptr) {
}

Message::Message(const std::shared_ptr<Runnable> &p_runnable)
    : runTaskSp_(p_runnable) {
}

Message::~Message() {
  runTaskSp_ = nullptr;
}

std::shared_ptr<Runnable> Message::getTask() {
  return runTaskSp_;
}