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
