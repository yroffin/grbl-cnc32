#include "TFT_eSPI_ms/TFT_eSPI.cpp" // setup file has to be edited for some parameters like screen device, pins

#include "config.h"
#include "widget.hpp"
#include "evt-ctrl.hpp"
#include "wifi-ctrl.hpp"
#include "storage-ctrl.hpp"
#include "grbl-ctrl.hpp"
#include "i18n-ctrl.hpp"
#include "nunchuk-ctrl.hpp"

void setup()
{
    // screen controller
    log_i("%s", I18nCtrl::instance()->translate(I18N_STD, I18N_INIT, "TFT_Screen"));
    TFT_Screen::instance()->init();

    // event controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_INIT, "EvtCtrl"));
    EvtCtrl::instance()->init();

    // storage controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_INIT, "StorageCtrl"));
    StorageCtrl::instance()->init();

    // wifi controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_INIT, "WifiCtrl"));
    WifiCtrl::instance()->init();

    // grbl controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_INIT, "GrblCtrl"));
    GrblCtrl::instance()->init();

    // NunchukCtrl controller
    TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_INIT, "NunchukCtrl"));
    NunchukCtrl::instance()->init();
}

void loop()
{
    // capture events
    EvtCtrl::instance()->capture();
    // capture events
    NunchukCtrl::instance()->capture();
    // capture http request
    WifiCtrl::instance()->serve();
    // flush grbl events
    GrblCtrl::instance()->capture();
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
