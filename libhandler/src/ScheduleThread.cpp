/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "handler/ScheduleThread.h"

ScheduleThread &ScheduleThread::getInstance() {
  static ScheduleThread instance;
  return instance;
}

std::shared_ptr<Handler> ScheduleThread::getHandlerSp() {
  return handlerSp_;
}

ScheduleThread::ScheduleThread() : HandlerThread("ScheduleThread") {
  start();
  handlerSp_ = std::make_shared<Handler>(getLooper());
}

ScheduleThread::~ScheduleThread() {
  quit();
}