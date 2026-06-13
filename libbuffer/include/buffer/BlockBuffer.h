/*
 * BlockBuffer prototypes
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 *
 * This file is part of libbuffer.
 *
 * libbuffer is Memory pool project.
 *
 * !!! It is recommended to use the shared_ptr package to use !!!
 */

#pragma once

class BlockBuffer {
 public:
  // _copyData 代表是拷贝_data 还是直接持有
  BlockBuffer(const char *_data, int _length, bool _copyData = true);

  BlockBuffer(const BlockBuffer &BlockBuffer);

  virtual ~BlockBuffer();

  char *getBuffer() const;

  int getBufferLength() const;

 private:
  void setBuffer(const char *_data, const int _length, const bool _copyData);
  void release();

 private:
  char *buffer_ = nullptr;
  int bufferLen_ = 0;
};
