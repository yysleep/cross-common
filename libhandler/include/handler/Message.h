/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <memory>

#include "handler/Runnable.h"

class Message {
 public:
  int what = 0;
  Message();
  explicit Message(const std::shared_ptr<Runnable> &p_runnable);
  virtual ~Message();
  std::shared_ptr<Runnable> getTask();

 private:
  std::shared_ptr<Runnable> runTaskSp_;
};
