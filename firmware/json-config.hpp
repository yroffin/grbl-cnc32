#ifndef _JSON_CONFIG_CONTROLLER_
#define _JSON_CONFIG_CONTROLLER_

#include "json-store.hpp"

#include "vars.hpp"

#include "ArduinoJson/ArduinoJson.h"
#include <SD.h>
#include <SPI.h>

class JsonConfigCtrl : public JsonStore
{
public:
  JsonConfigCtrl();
  static JsonConfigCtrl *instance();

  void setup();
  void write();

public:
  char lang[32];

private:
};

#endif
