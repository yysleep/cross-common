/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>

#include <memory>

#include "handler/Handler.h"
#include "handler/HandlerThread.h"
#include "logger/logger.h"

auto testHThread = std::make_shared<HandlerThread>("test");
std::shared_ptr<Handler> testHandler;

void init() {
  testHThread->start();
  testHandler = std::make_shared<Handler>(std::move(testHThread->getLooper()));
}

void unInit() {
  if (nullptr == testHThread) return;

  testHThread->quit();
  testHandler = nullptr;
  testHThread = nullptr;
}

void testSendMessage(int what,
                     unsigned long delayMs,
                     const std::string &content) {
  ILOGI("testSendMessage") << " + what=" << what << ", delayMs=" << delayMs
                           << ", content=" << content;
  auto runnable = std::make_shared<Runnable>();
  auto msg = std::make_shared<Message>(runnable);
  msg->what = what;

  runnable->run = [what, delayMs, content]() {
    ILOGI("testSendMessage") << " - what=" << what << ", delayMs=" << delayMs
                             << ", content=" << content;
  };

  testHandler->sendMessageDelayed(msg, delayMs);
}

void testRemoveMessage(int what) {
  ILOGI("testRmoveMessage") << " what=" << what;

  testHandler->removeMessages(what);
}

void testMsgThreadSendMsg(int what,
                          unsigned long delayMs,
                          const std::string &content) {
  ILOGI("testMsgThreadSendMsg") << " + what=" << what << ", delayMs=" << delayMs
                                << ", content=" << content;
  auto runnable = std::make_shared<Runnable>();
  auto msg = std::make_shared<Message>(runnable);
  msg->what = what;

  runnable->run = [what, delayMs, content]() {
    ILOGI("testMsgThreadSendMsg")
        << " - what=" << what << ", delayMs=" << delayMs
        << ", content=" << content;
    testSendMessage(what, delayMs, content);
  };

  testHandler->sendMessageDelayed(msg, delayMs);
}

void test() {
  testSendMessage(100, 8000, "testSendMessage 8s");

  ILOGI("test") << " yysleep lefts 10s";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 9s";
  testSendMessage(100, 3000, "testSendMessage 3s");
  testSendMessage(100, 500, "testSendMessage 0.5s");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 8s";
  testMsgThreadSendMsg(100, 2500, "testMsgThreadSendMsg 2.5s");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 7s";
  testSendMessage(100, 1000, "testSendMessage 1s");
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 6s";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 5s";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 4s";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 3s";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 2s";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("test") << " yysleep lefts 1s";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  testSendMessage(100, 0, "testSendMessage 0s");
  testSendMessage(100, 100, "testSendMessage 0.1s");
  ILOGI("test") << " yysleep lefts 0s";
}

int main(int argc, char *argv[]) {
  ILOGI("main") << " +++ yysleep";

  init();
  test();
  unInit();

  // ILOGI("main") << " yysleep lefts 1s";
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  ILOGI("main") << " --- yysleep";
  return EXIT_SUCCESS;
}