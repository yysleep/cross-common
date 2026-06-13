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

#include "buffer/BlockBuffer.h"

#include <memory.h>

char *BlockBuffer::getBuffer() const {
  return buffer_;
}

int BlockBuffer::getBufferLength() const {
  return bufferLen_;
}

BlockBuffer::BlockBuffer(const char *_data, int _length, bool _copyData) {
  setBuffer(_data, _length, _copyData);
}

BlockBuffer::BlockBuffer(const BlockBuffer &BlockBuffer) {
  setBuffer(BlockBuffer.buffer_, BlockBuffer.bufferLen_, false);
}

BlockBuffer::~BlockBuffer() {
  release();
}

void BlockBuffer::setBuffer(const char *_data,
                            const int _length,
                            const bool _copyData) {
  release();

  if (!_data || _length <= 0) {
    return;
  }

  if (!_copyData) {
    buffer_ = reinterpret_cast<char *>(const_cast<char *>(_data));
    bufferLen_ = _length;
    return;
  }

  buffer_ = new char[_length];
  if (!buffer_) {
    return;
  }

  memcpy(buffer_, _data, _length);
  bufferLen_ = _length;
}

void BlockBuffer::release() {
  if (buffer_) {
    delete[] buffer_;
    buffer_ = nullptr;
  }

  bufferLen_ = 0;
}