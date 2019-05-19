#include "storage-ctrl.hpp"
#include "ui.hpp"
#include "iniFile.h"

// Storage controller
StorageCtrl *__instance_storage = 0;

// singleton
StorageCtrl *StorageCtrl::instance()
{
    if (__instance_storage == 0)
    {
        __instance_storage = new StorageCtrl();
    }
    return __instance_storage;
}

StorageCtrl::StorageCtrl()
{
}

void StorageCtrl::init()
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

void explore(const char *base)
{
    File root = SD.open(base);
    File entry = root.openNextFile();
    for (; entry; entry = root.openNextFile())
    {
        if (entry.isDirectory())
        {
            TFT_Screen::instance()->status("Dir: %s", entry.name());
        }
        else
        {
            TFT_Screen::instance()->status("File: %s", entry.name());
        }
    }
}

void StorageCtrl::refresh()
{
    explore("/");
}