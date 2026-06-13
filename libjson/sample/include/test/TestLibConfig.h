/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <string>

#include "json/DataPacket.h"

class TestLibConfig : public Base::DataPacket {
 public:
  std::string libName;

  std::string libPath;

  std::string libType;

  double libVersion;

  void DecodeJson(cJSON *reader) override;

  void EncodeJson(cJSON *writer) override;
};

void TestLibConfig::EncodeJson(cJSON *writer) {
  Base::AddJsonValue(writer, "libName", libName);
  Base::AddJsonValue(writer, "libPath", libPath);
  Base::AddJsonValue(writer, "libType", libType);
  Base::AddJsonValue(writer, "libVersion", libVersion);
}

void TestLibConfig::DecodeJson(cJSON *reader) {
  Base::GetJsonValue(reader, "libName", libName);
  Base::GetJsonValue(reader, "libPath", libPath);
  Base::GetJsonValue(reader, "libType", libType);
  Base::GetJsonValue(reader, "libVersion", libVersion);
}
