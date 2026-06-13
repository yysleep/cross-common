/*
 * CharBuffer prototypes
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 *
 * This file is part of libbuffer.
 *
 * libbuffer is Memory pool project.
 */

#pragma once

#include <string>

#include "buffer/RingBuffer.h"

class CharBuffer : public RingBuffer<char> {
 public:
  char *getBuffer() override;
  int read(char *_buffer, int *_size, bool _copy = false) override;
  int write(const char *_buffer, int _size, bool _copy = false) override;
  long getPosition(int _mode) override;
  int getCount(int _mode) override;
  long getCapacity() override;
  long getCacheSize() override;
  int reset(int _mode = MODE_ALL) override;

  bool isEmpty() override;
  bool isPoor() override;
  bool isModerate() override;
  bool isRich() override;
  bool isFull() override;

 public:
  explicit CharBuffer(long _capacity);
  CharBuffer(long _capacity, std::string name);
  virtual ~CharBuffer();
  CharBuffer(const CharBuffer &) = delete;
  CharBuffer &operator=(const CharBuffer &) = delete;

 private:
  void init(long _capacity, std::string name);
  std::string &getModeStr(int mode);

 private:
  char *realCBuffer_ = nullptr;
  std::string modeStr_;
  std::string bufferName_;
};
