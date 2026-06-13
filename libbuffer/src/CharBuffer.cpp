/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#include "buffer/CharBuffer.h"

#include <string.h>

#include "logger/logger.h"

#define EMPTY_THRESHOLD 0
#define POOR_THRESHOLD 25
#define RICH_THRESHOLD 75
#define FULL_THRESHOLD 100

#define TAG "CharBuffer.cpp"

CharBuffer::CharBuffer(long _capacity) {
  init(_capacity, "");
}

CharBuffer::CharBuffer(long _capacity, std::string name) {
  init(_capacity, name);
}

CharBuffer::~CharBuffer() {
  if (realCBuffer_ != nullptr) {
    delete[] realCBuffer_;
    realCBuffer_ = nullptr;
  }
  // ILOGI("~CharBuffer name=%s, this%p", bufferName_.c_str(), this);
  ILOGI("~CharBuffer") << " bufferName=" << bufferName_ << ", this=" << this;
}

char *CharBuffer::getBuffer() {
  return realCBuffer_;
}

int CharBuffer::read(char *_buff, int *_size, bool _copy) {
  int pos = readPos_;
  if (readPos_ + *_size >= bufferCapacity_) {
    *_size = bufferCapacity_ - readPos_;
    readPos_ = 0;
    readCount_++;
  } else {
    readPos_ += *_size;
  }

  if (_copy) {
    memcpy(_buff, realCBuffer_ + pos, (*_size) * sizeof(char));
  }
  return readPos_;
}

int CharBuffer::write(const char *_buff, int _size, bool _copy) {
  int cbwLeftSize = 0;
  int cbwSingleSize = 0;
  int cbwOffset = 0;

  cbwLeftSize = _size;
  cbwSingleSize = cbwLeftSize;
  cbwOffset = 0;
  while (cbwLeftSize > 0) {
    if (writePos_ + cbwSingleSize > bufferCapacity_) {
      cbwSingleSize = bufferCapacity_ - writePos_;
    }

    if (_copy) {
      memcpy(realCBuffer_ + writePos_,
             _buff + cbwOffset,
             cbwSingleSize * sizeof(char));
    }

    cbwOffset += cbwSingleSize;
    cbwLeftSize -= cbwSingleSize;
    writePos_ += cbwSingleSize;

    cbwSingleSize = cbwLeftSize;

    if (writePos_ >= bufferCapacity_) {
      writePos_ = 0;
      writeCount_++;
    }
  }
  return _size;
}

long CharBuffer::getPosition(int _mode) {
  switch (_mode) {
    case MODE_READ:
      return readPos_;

    case MODE_WRITE:
      return writePos_;

    default:
      break;
  }

  return 0;
}

int CharBuffer::getCount(int _mode) {
  switch (_mode) {
    case MODE_READ:
      return readCount_;

    case MODE_WRITE:
      return writeCount_;

    default:
      break;
  }

  return 0;
}

long CharBuffer::getCapacity() {
  return bufferCapacity_;
}

long CharBuffer::getCacheSize() {
  return (writeCount_ - readCount_) * bufferCapacity_ + writePos_ - readPos_;
}

int CharBuffer::reset(int _mode) {
  // ILOGI("reset() name=%s, mode=%s, bufferCapacity_=%d, w_pos=%d,
  // writeCount_=%d, r_pos=%d, readCount_=%d", bufferName_.c_str(),
  // getModeStr(_mode).c_str(), bufferCapacity_, writePos_, writeCount_,
  // readPos_, readCount_);

  switch (_mode) {
    case MODE_READ:
      readPos_ = 0;
      readCount_ = 0;
      break;

    case MODE_WRITE:
      writePos_ = 0;
      writeCount_ = 0;
      break;

    default:
      readPos_ = 0;
      readCount_ = 0;
      writePos_ = 0;
      writeCount_ = 0;
      break;
  }
  return 0;
}

bool CharBuffer::isRich() {
  return getCacheSize() >= (bufferCapacity_ * RICH_THRESHOLD / 100);
}

bool CharBuffer::isEmpty() {
  return getCacheSize() == 0;
}

bool CharBuffer::isPoor() {
  return getCacheSize() <= (bufferCapacity_ * POOR_THRESHOLD / 100);
}

bool CharBuffer::isModerate() {
  return getCacheSize() > (bufferCapacity_ * POOR_THRESHOLD / 100) &&
         getCacheSize() < (bufferCapacity_ * RICH_THRESHOLD / 100);
}

bool CharBuffer::isFull() {
  return getCacheSize() >= bufferCapacity_;
}

void CharBuffer::init(long _capacity, std::string name) {
  bufferName_ = name;
  bufferCapacity_ = _capacity;
  realCBuffer_ = new char[bufferCapacity_];
  // ILOGI("init() bufferCapacity_=%d, realCBuffer_=%p, name=%s, this=%p",
  // bufferCapacity_, realCBuffer_, name.c_str(), this);
}

std::string &CharBuffer::getModeStr(int mode) {
  switch (mode) {
    case MODE_ALL:
      modeStr_ = "ALL";
      break;

    case MODE_READ:
      modeStr_ = "READ";
      break;

    case MODE_WRITE:
      modeStr_ = "WRITE";
      break;

    default:
      modeStr_ = "UNKNOW";
      break;
  }

  return modeStr_;
}