/*
 * Copyright (c) 2026
 *
 * @author yysleep
 * @date 2026/06/10
 * All rights reserved.
 */

#pragma once

#include <map>
#include <string>

#include "TestLibConfig.h"
#include "json/DataPacket.h"

class TestConfig : public Base::DataPacket {
 public:
  int intValue;

  long long llValue;

  double resVersion;

  std::string resType;

  TestLibConfig libConfig;

  std::map<std::string, std::string> testMap;

  void DecodeJson(cJSON *reader) override;

  void EncodeJson(cJSON *writer) override;
};

void TestConfig::EncodeJson(cJSON *writer) {
  Base::AddJsonValue(writer, "intValue", intValue);
  Base::AddJsonValue(writer, "llValue", llValue);
  printf("TestConfig::EncodeJson() EncodeJson llValue=%lld\n", llValue);
  Base::AddJsonValue(writer, "resVersion", resVersion);
  Base::AddJsonValue(writer, "resType", resType);
  Base::AddJsonValue(writer, "libConfig", libConfig);
  Base::AddJsonValue(writer, "testMap", testMap);
}

void TestConfig::DecodeJson(cJSON *reader) {
  printf("TestConfig::DecodeJson() + reader=%p\n", reader);

  Base::GetJsonValue(reader, "intValue", intValue);
  printf("TestConfig::DecodeJson() GetJsonValue intValue=%d\n", intValue);

  Base::GetJsonValue(reader, "llValue", llValue);
  printf("TestConfig::DecodeJson() GetJsonValue llValue=%lld\n", llValue);

  Base::GetJsonValue(reader, "resVersion", resVersion);
  printf("TestConfig::DecodeJson() GetJsonValue resVersion=%f\n", resVersion);

  printf("TestConfig::DecodeJson() GetJsonValue resType_p=%p\n", &resType);
  // cJSON *temp = cJSON_GetObjectItem(reader, "resType");
  // Base::GetJsonValue(temp, resType);

  Base::GetJsonValue(reader, "resType", resType);
  printf("TestConfig::DecodeJson() GetJsonValue resType=%s\n", resType.c_str());

  printf("TestConfig::DecodeJson() GetJsonValue + libConfig.address=%p\n",
         &libConfig);
  Base::GetJsonValue(reader, "libConfig", libConfig);
  printf("TestConfig::DecodeJson() GetJsonValue - libConfig.address=%p\n",
         &libConfig);

  Base::GetJsonValue(reader, "testMap", testMap);
  printf("TestConfig::DecodeJson() GetJsonValue testMap.size=%zu\n",
         testMap.size());

  printf("TestConfig::DecodeJson() GetJsonValue testMap.size=%zu\n",
         testMap.size());

  for (auto &item : testMap) {
    printf("TestConfig::DecodeJson() GetJsonValue testMap.item=%s:=%s\n",
           item.first.c_str(),
           item.second.c_str());
  }

  printf("TestConfig::DecodeJson() -\n");
}
