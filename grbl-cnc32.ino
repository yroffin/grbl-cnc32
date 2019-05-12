#include "TFT_eSPI_ms/TFT_eSPI.cpp"   // setup file has to be edited for some parameters like screen device, pins

#include "config.h"
#include "widget.hpp"
#include "evt-ctrl.hpp"

// create for touchscreeen
TFT_eSPI tft = TFT_eSPI();
// event controller
EvtCtrl evtCtrl(tft);
// screen controller
TFT_Screen screen(tft, evtCtrl);

void setup() {
    // Setup TFT
    pinMode(TFT_LED_PIN , OUTPUT) ;
    digitalWrite(TFT_LED_PIN , HIGH) ;

    // TFT init
    screen.init();

    // Event controller init
    evtCtrl.init();
}

void loop() {
    // capture events
    evtCtrl.capture();
    // notify, then render screen if invalidate state
    if(screen.isInvalidated()) {
        screen.render();
    }
    // flush events
    evtCtrl.flush();
}
