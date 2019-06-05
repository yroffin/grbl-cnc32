#include "json-config.hpp"
#include "config.h"

// I18nCtrl controller
JsonConfigCtrl *__instance_json_config = 0;

// singleton
JsonConfigCtrl *JsonConfigCtrl::instance()
{
  if (__instance_json_config == 0)
  {
    __instance_json_config = new JsonConfigCtrl();
  }
  return __instance_json_config;
}

// Allocate a temporary JsonDocument
// Don't forget to change the capacity to match your requirements.
// Use arduinojson.org/v6/assistant to compute the capacity.
StaticJsonDocument<JSON_CONFIG_SIZE> config;

JsonConfigCtrl::JsonConfigCtrl() : JsonStore(config)
{
}

void JsonConfigCtrl::setup()
{
  // Initialize SD library
  while (!SD.begin(SD_CHIPSELECT_PIN))
  {
    log_i("Failed to initialize SD library");
    delay(1000);
  }

  // load global configuration
  this->load("/config.json");
}

void JsonConfigCtrl::write()
{
  this->save("/config.json", "/config.bak");
}