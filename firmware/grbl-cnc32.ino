#include "TFT_eSPI_ms/TFT_eSPI.cpp" // setup file has to be edited for some parameters like screen device, pins

#include "vars.hpp"
#include "widget.hpp"
#include "evt-ctrl.hpp"
#include "wifi-ctrl.hpp"
#include "storage-ctrl.hpp"
#include "grbl-ctrl.hpp"
#include "i18n-ctrl.hpp"
#include "nunchuk-ctrl.hpp"
#include "json-config.hpp"

void setup()
{
    // prepare TFT
    TFT_Screen::instance()->prepare();

    // JsonConfigCtrl controller
    JsonConfigCtrl::instance()->setup();

    // I18nCtrl controller
    I18nCtrl::instance()->setup();

    // screen controller
    log_i("%s", I18nCtrl::instance()->translate(I18N_STD, "INI", "TFT_Screen"));
    TFT_Screen::instance()->init();

    // event controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, "INI", "EvtCtrl"));
    EvtCtrl::instance()->init();

    // storage controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, "INI", "StorageCtrl"));
    StorageCtrl::instance()->init();

    // wifi controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, "INI", "WifiCtrl"));
    WifiCtrl::instance()->setup();

    // grbl controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, "INI", "GrblCtrl"));
    GrblCtrl::instance()->setup();

    // NunchukCtrl controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, "INI", "NunchukCtrl"));
    NunchukCtrl::instance()->setup();
}

void loop()
{
    // capture events
    EvtCtrl::instance()->capture();
    // capture events
    NunchukCtrl::instance()->loop();
    // init wifi, capture http request
    WifiCtrl::instance()->loop();
    // flush grbl events
    GrblCtrl::instance()->loop();
    // dispatch event
    EvtCtrl::instance()->dispatchPrimaryEvents();
    // dispatch event (until any)
    for (; EvtCtrl::instance()->dispatchSecondaryEvents();)
        ;
    // render
    TFT_Screen::instance()->render();
    TFT_Screen::instance()->setInvalidated(false);
    // printing
    GrblCtrl::instance()->spool();
    // flush events
    EvtCtrl::instance()->flush();
}
