#ifndef _JSON_CONFIG_CONTROLLER_
#define _JSON_CONFIG_CONTROLLER_

typedef struct
{
  char ssid[32];
  char pass[32];
} ConfigWifi;

struct Config
{
  int wifiConfigSize;
  ConfigWifi wifi[4];
};

class JsonConfigCtrl
{
public:
  JsonConfigCtrl();
  static JsonConfigCtrl *instance();

  void setup();
  void loadConfiguration(const char *filename, Config &config);
  void saveConfiguration(const char *filename, const Config &config);
  void printFile(const char *filename);

public:
  Config config;
};

#endif
