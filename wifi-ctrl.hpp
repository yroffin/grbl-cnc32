#ifndef _WIFI_CONTROLLER_
#define _WIFI_CONTROLLER_

#include "config.h"
#include "ui.hpp"
#include "evt-ctrl.hpp"
#include "WebServer.h"
#include "WiFi.h"

class WifiCtrl
{
public:
  WifiCtrl();
  void init();
  void serve();

  static WifiCtrl *instance();
};

#endif
