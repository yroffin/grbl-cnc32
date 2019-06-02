#include "json-config.hpp"
#include "config.h"
#include "utils.hpp"

#include "ArduinoJson/ArduinoJson.h"
#include <SD.h>
#include <SPI.h>

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

JsonConfigCtrl::JsonConfigCtrl()
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
  this->loadConfiguration("/config.json", this->config);
}

// Loads the configuration from a file
void JsonConfigCtrl::loadConfiguration(const char *filename, Config &config)
{
  // Open file for reading
  File file = SD.open(filename);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<1024> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    log_i("Failed to read file, using default configuration");

  // Load configuration wifi
  config.wifiConfigSize = doc["wifi"].size();
  for (int key = 0; key < config.wifiConfigSize; key++)
  {
    Utils::strcpy(config.wifi[key].ssid, doc["wifi"].getElement(key)["ssid"], sizeof(config.wifi[key].ssid));
    Utils::strcpy(config.wifi[key].pass, doc["wifi"].getElement(key)["pass"], sizeof(config.wifi[key].pass));
  }

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void JsonConfigCtrl::saveConfiguration(const char *filename, const Config &config)
{
  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(".bak");
  SD.rename(filename, ".bak");

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file)
  {
    log_i("Failed to create file");
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  //Utils::strcpy(doc["wifi"].getMember("ssid"), config.wifi.ssid, sizeof(config.wifi.ssid));
  //Utils::strcpy(doc["wifi"]["pass"], config.wifi.pass, sizeof(config.wifi.pass));

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0)
  {
    log_i("Failed to write to file");
  }

  // Close the file
  file.close();
}

// Prints the content of a file to the Serial
void JsonConfigCtrl::printFile(const char *filename)
{
  // Open file for reading
  File file = SD.open(filename);
  if (!file)
  {
    log_i("Failed to read file");
    return;
  }

  // Extract each characters by one by one
  while (file.available())
  {
    const char *data = file.readStringUntil(10).c_str();
    log_i("%s", data);
  }

  // Close the file
  file.close();
}
