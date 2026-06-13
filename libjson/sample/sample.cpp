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

#include "TestConfig.h"
#include "json/DataPacket.h"

static std::string testTxt =
    "{\"intValue\":123,\"llValue\":3147483639,\"resVersion\":1001,"
    "\"resType\":\"test_type\",\"libConfig\":{\"libName\":\"libTest.so\","
    "\"libPath\":\"/usr/local/"
    "lib\",\"libType\":\"TestEngine\",\"libVersion\":1001},\"testMap\":{"
    "\"key1\":\"value1\",\"key2\":\"value2\",\"中文键\":\"中文值\","
    "\"特殊字符键\":\"!@#$%^&*()\",\"空值测试\":\"\",\"key3\":\"value3\"}}";

void testTestConfig() {
  printf("testTestConfig + json=%s\n", testTxt.c_str());
  TestConfig config;
  config.SetJsonText(testTxt);
  printf("testTestConfig GetJsonText config.llValue=%lld\n", config.llValue);

  std::string jsonStr = config.GetJsonText();
  printf("testTestConfig GetJsonText jsonStr=%s\n", jsonStr.c_str());

  printf("testTestConfig -\n");
}

int main(int argc, char *argv[]) {
  printf("main +\n");

  testTestConfig();

  printf("main -\n");
  return 0;
}