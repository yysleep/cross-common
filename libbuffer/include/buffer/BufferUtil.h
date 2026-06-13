/*
 * BufferUtil prototypes
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

#include "logger/logger.h"

class BufferUtil {
 public:
  static int resize(char **_buffer, int _src_capacity, int _dest_capacity) {
    if (_buffer == nullptr || *_buffer == nullptr) {
      return _src_capacity;
    }

    ILOGI("BuffUtil::resize()") << " + _src_capacity=" << _src_capacity
                                << ", _dest_capacity=" << _dest_capacity;

    int base = _dest_capacity / 1024 + 1;
    _dest_capacity = base * 2 * 1024;

    ILOGI("BuffUtil::resize()") << " reallySize=" << _dest_capacity;

    char *temp = new char[_dest_capacity];
    if (temp == nullptr) {
      ILOGI("BuffUtil::resize()") << " - resize failed!";
      return _src_capacity;
    }

    delete[] *_buffer;
    *_buffer = temp;

    ILOGI("BuffUtil::resize()") << " - resize success!";
    return _dest_capacity;
  }

  static int asShortBuffer(char *byteBuff, int size, short *shortBuff) {
    if (size <= 0) {
      return -1;
    }
    int j = 0;
    for (int i = 0; i < size;) {
      short low = ((short(byteBuff[i + 1]) << 8) & 0x0000ff00);
      short high = ((short(byteBuff[i])) & 0x000000ff);
      shortBuff[j] = low | high;

      i = i + 2;
      j++;
    }
    return 0;
  }

  static int asCharBuffer(short *shortBuff, int size, char *byteBuff) {
    if (size <= 0) {
      return -1;
    }
    int j = 0;
    for (int i = 0; i < size; i++) {
      byteBuff[j++] = shortBuff[i] & 0xff;         // 低8位
      byteBuff[j++] = (shortBuff[i] >> 8) & 0xff;  // 高8位
    }
    return 0;
  }
};
