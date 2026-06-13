//
// Created by Yaozy on 2019/10/9.
//
#include "json/DataPacket.h"

#include <map>
#include <string>

using namespace std;

#define TAG "DataPacket.cpp"

namespace Base {

void GetJsonValue(cJSON *node, string &value) {
  if (!cJSON_IsNull(node) && (cJSON_IsString(node) || cJSON_IsNumber(node)))
    value = node->valuestring;
  else
    value = "";
}

void GetJsonValue(cJSON *node, int &value) {
  if (!cJSON_IsNull(node) && cJSON_IsNumber(node))
    value = node->valueint;
  else
    value = 0;
}

void GetJsonValue(cJSON *node, long &value) {
  if (!cJSON_IsNull(node) && cJSON_IsNumber(node))
    value = node->valueint;
  else
    value = 0;
}

void GetJsonValue(cJSON *node, long long &value) {
  if (!cJSON_IsNull(node) && cJSON_IsNumber(node))
    value = node->valueint;
  else
    value = 0;
}

void GetJsonValue(cJSON *node, double &value) {
  if (!cJSON_IsNull(node) && cJSON_IsNumber(node))
    value = node->valuedouble;
  else
    value = 0.0;
}

void GetJsonValue(cJSON *node, bool &value) {
  value = cJSON_IsTrue(node) != 0;
}

void GetJsonValue(cJSON *node, DataPacket &value) {
  value.DecodeJson(node);
}

void GetJsonValue(cJSON *node, map<string, string> &value) {
  value.clear();
  if (!cJSON_IsNull(node) && cJSON_IsObject(node)) {
    cJSON *current = node->child;
    while (current != nullptr) {
      if (!current->string) {
        current = current->next;
        continue;
      }
      string key = current->string;
      string val;
      GetJsonValue(current, val);
      if (!key.empty()) {
        value[key] = val;
      }
      current = current->next;
    }
  }
}

void AddJsonValue(cJSON *node,
                  const char *nodeName,
                  map<string, string> &value) {
  cJSON *obj = cJSON_Create(value);
  cJSON_AddItemToObject(node, nodeName, obj);
}

cJSON *cJSON_Create(map<string, string> &value) {
  cJSON *obj = cJSON_CreateObject();
  for (const auto &pair : value) {
    string val = pair.second;
    AddJsonValue(obj, pair.first.c_str(), val);
  }
  return obj;
}

cJSON *cJSON_Create(string &value) {
  return cJSON_CreateString(value.c_str());
}

cJSON *cJSON_Create(int &value) {
  return cJSON_CreateNumber(value);
}

cJSON *cJSON_Create(long &value) {
  return cJSON_CreateNumber(value);
}

cJSON *cJSON_Create(long long &value) {
  return cJSON_CreateNumber(value);
}

cJSON *cJSON_Create(double &value) {
  return cJSON_CreateNumber(value);
}

cJSON *cJSON_Create(float &value) {
  return cJSON_CreateNumber(value);
}

cJSON *cJSON_Create(bool &value) {
  return cJSON_CreateBool(value);
}

cJSON *cJSON_Create(DataPacket &value) {
  cJSON *item = value.CreateWriter();
  value.EncodeJson(item);
  return item;
}

string DataPacket::GetJsonText() {
  cJSON *writer = CreateWriter();
  char *text = cJSON_Print(writer);
  string result = text;
  free(text);
  cJSON_Delete(writer);
  return result;
}

void DataPacket::SetJsonText(const string &text) {
  cJSON *reader = cJSON_Parse(text.c_str());
  if (reader) {
    DecodeJson(reader);
    cJSON_Delete(reader);
  }
}

cJSON *DataPacket::CreateWriter() {
  cJSON *writer = cJSON_CreateObject();
  EncodeJson(writer);
  return writer;
}

}  // namespace Base