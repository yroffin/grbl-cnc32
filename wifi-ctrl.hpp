#ifndef _WIFI_CONTROLLER_
#define _WIFI_CONTROLLER_

#include "config.h"
#include "TFT_eSPI_ms/TFT_eSPI.h"
#include "evt-ctrl.hpp"
#include "WebServer.h"
#include "WiFi.h"

class WifiCtrl
{
public:
  WifiCtrl(TFT_eSPI &_tft, EvtCtrl &evtCtrl);
  void connect();
  void serve();

protected:
  TFT_eSPI &tft;
  EvtCtrl &evtCtrl;
};

#endif
