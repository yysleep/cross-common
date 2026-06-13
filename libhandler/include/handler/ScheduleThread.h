/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include "handler/Handler.h"
#include "handler/HandlerThread.h"

class ScheduleThread : public HandlerThread {
 public:
  // 获取单例实例
  static ScheduleThread &getInstance();

  std::shared_ptr<Handler> getHandlerSp();

 public:
  // 禁用拷贝构造函数和赋值运算符
  ScheduleThread(const ScheduleThread &) = delete;
  ScheduleThread &operator=(const ScheduleThread &) = delete;

 private:
  ScheduleThread();
  virtual ~ScheduleThread();

 private:
  std::shared_ptr<Handler> handlerSp_ = nullptr;
};
