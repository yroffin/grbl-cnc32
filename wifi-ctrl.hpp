#ifndef _WIFI_CONTROLLER_
#define _WIFI_CONTROLLER_

#include "config.h"
#include "ui.hpp"
#include "evt-ctrl.hpp"
#include "WebServer.h"
#include "WiFi.h"

#define MAXSIZE_OF_IP 32

enum WifiPhase
{
  wifiNil,
  wifiInit,
  wifiNext,
  wifiConnected,
  unixTime,
  serveData
};

class WifiCtrl
{
public:
  WifiCtrl();
  void setup();
  void loop();

  static WifiCtrl *instance();

private:
  WifiPhase phase = wifiNil;
  long lastConnect = 0;
  int current = 0;
  int retry = 0;
};

#endif
