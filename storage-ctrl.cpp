#include "storage-ctrl.hpp"
#include "iniFile.h"

StorageCtrl::StorageCtrl(TFT_eSPI &_tft, EvtCtrl &_evtCtrl) : tft(_tft), evtCtrl(_evtCtrl)
{
    log_i("Storage controller ok.");
}

void StorageCtrl::mount()
{
    if (!SD.begin(SD_CHIPSELECT_PIN))
    {
        log_e("Card mount failed");
        return;
    }
    if (!SD.exists("/"))
    { // check if root exist
        log_e("Root ('/') not found on SD card");
        return;
    }
    log_i("File system ok.");
}

void StorageCtrl::config()
{
    // load configuration files
}