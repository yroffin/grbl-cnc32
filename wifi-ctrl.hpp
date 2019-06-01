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
  wifiConnected,
  serveData
};

class WifiCtrl
{
public:
  WifiCtrl();
  void init();
  void serve();

  static WifiCtrl *instance();

private:
  char ssid[32];
  WifiPhase phase = wifiNil;
  long lastConnect = 0;
};

#endif
