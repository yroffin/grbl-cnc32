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

private:
  JsonDocument &store;
};

#endif
