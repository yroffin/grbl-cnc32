#ifndef _JSON_CONFIG_CONTROLLER_
#define _JSON_CONFIG_CONTROLLER_

#include "json-store.hpp"

#include "config.h"

#include "ArduinoJson/ArduinoJson.h"
#include <SD.h>
#include <SPI.h>

class JsonConfigCtrl : public JsonStore
{
public:
  JsonConfigCtrl();
  static JsonConfigCtrl *instance();

  void setup();

private:
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<JSON_CONFIG_SIZE> config;
};

#endif
