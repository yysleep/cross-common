/*
 * RingBuffer prototypes
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

enum RMode
{
  MODE_ALL = 0x0000,
  MODE_READ = 0x0010,
  MODE_WRITE = 0x0100,
};

/*
 * 存储数据的环形buff(可扩容)
 */

template <class T>
class RingBuffer {
 public:
  RingBuffer(/* args */) {
  }
  virtual ~RingBuffer() {
  }
  RingBuffer(const RingBuffer &) = delete;
  RingBuffer &operator=(const RingBuffer &) = delete;
  virtual T *getBuffer() = 0;
  virtual int read(T *_buffer, int *_size, bool _copy) = 0;
  virtual int write(const T *_buffer, int _size, bool _copy) = 0;

  virtual long getPosition(int _mode) = 0;
  virtual int getCount(int _mode) = 0;
  virtual long getCapacity() = 0;
  virtual long getCacheSize() = 0;
  virtual int reset(int _mode) = 0;

  virtual bool isEmpty() = 0;  // 是否空(0%容量)
  virtual bool isPoor() = 0;   // 是否数据稀缺(<=25%容量)
  virtual bool isModerate() = 0;  // 是否数据适中(25%容量 < 数据量 < 75%容量)
  virtual bool isRich() = 0;  // 是否数据充足(>=75%容量)
  virtual bool isFull() = 0;  // 是否几乎已满(>=100%容量)

 protected:
  volatile long readPos_ = 0;   // 当前读取的位置
  volatile int readCount_ = 0;  // 读取数据的循环次数

  volatile long writePos_ = 0;   // 当前写入的位置
  volatile int writeCount_ = 0;  // 写入数据的循环次数

  long bufferCapacity_ = 0;  // 初始化时候的容量。
  long bufferLimit_ = 0;     // 有效长度
};
