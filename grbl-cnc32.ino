#include "TFT_eSPI_ms/TFT_eSPI.cpp" // setup file has to be edited for some parameters like screen device, pins

#include "config.h"
#include "widget.hpp"
#include "evt-ctrl.hpp"
#include "wifi-ctrl.hpp"
#include "storage-ctrl.hpp"
#include "grbl-ctrl.hpp"

// create for touchscreeen
TFT_eSPI tft = TFT_eSPI();
// event controller
EvtCtrl evtCtrl(tft);
// screen controller
TFT_Screen screenCtrl(tft, evtCtrl);
// screen controller
WifiCtrl wifiCtrl(tft, evtCtrl);
// storage controller
StorageCtrl storageCtrl(tft, evtCtrl);
// grbl controller
GrblCtrl grblCtrl(screenCtrl, evtCtrl);

void setup()
{
    // Setup TFT
    pinMode(TFT_LED_PIN, OUTPUT);
    digitalWrite(TFT_LED_PIN, HIGH);

    // TFT init
    screenCtrl.init();

    // Event controller init
    evtCtrl.init();

    // Storage controller init
    storageCtrl.mount();

    // Wifi
    wifiCtrl.connect();

    // Grbl
    grblCtrl.init();
}

void loop()
{
    // capture events
    evtCtrl.capture();
    // capture http request
    wifiCtrl.serve();
    // flush grbl events
    grblCtrl.capture();
    // notify, then render screen if invalidate state
    if (screenCtrl.isInvalidated())
    {
        screenCtrl.render();
    }
    // flush events
    evtCtrl.flush();
}
