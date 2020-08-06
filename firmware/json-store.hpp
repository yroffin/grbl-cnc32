#ifndef _JSON_STORE_
#define _JSON_STORE_

#include "ArduinoJson/ArduinoJson.h"
#include <SD.h>
#include <SPI.h>

class JsonStore
{
public:
  JsonStore(JsonDocument &_store);

  void load(const char *filename);
  void save(const char *filename, const char *old);
  void printFile(const char *filename);

  int getSize(const char *k1);

  const char *getAsString(const char *k1, int k2, const char *k3, const char *def);
  const char *getAsString(const char *k1, const char *k2, const char *def);
  boolean getAsArray(const char *k1, const char *k2, uint16_t *calData, int size);
  void setAsArray(const char *k1, const char *k2, uint16_t *calData, int size);
  float getAsFloat(const char *k1, const char *k2, const char *k3, float def);
  int getAsInt(const char *k1, const char *k2, int def);
  int getAsInt(const char *k1, const char *k2, const char *k3, int def);
  boolean getAsBoolean(const char *k1, const char *k2, boolean def);

  void get(char *buffer, int size);
  void set(char *buffer, int size);

public:
  JsonDocument &store;
};

#endif
