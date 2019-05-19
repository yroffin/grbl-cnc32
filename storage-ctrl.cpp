#include "config.h"
#include "storage-ctrl.hpp"

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
    // Set "/" as root folder
    strcpy(cwd, "/");
    for (int i = 0; i < 128; i++)
    {
        this->storageEntry[i] = 0;
    }
    log_i("File system ok.");
}

void StorageCtrl::scan(const char *base)
{
    File root = SD.open(base);
    File entry = root.openNextFile();
    for (; entry; entry = root.openNextFile())
    {
        if (this->storageEntry[this->storageEntryCount] == 0)
        {
            this->storageEntry[this->storageEntryCount] = new StorageEntry();
        }
        if (entry.isDirectory())
        {
            this->storageEntry[this->storageEntryCount]->setDirectory(true);
            this->storageEntry[this->storageEntryCount]->setPath(entry.name());
            this->storageEntryCount++;
        }
        else
        {
            this->storageEntry[this->storageEntryCount]->setDirectory(false);
            this->storageEntry[this->storageEntryCount]->setPath(entry.name());
            this->storageEntryCount++;
        }
    }
}

int16_t StorageCtrl::getCount()
{
    this->storageEntryCount = 0;
    scan(cwd);
    return this->storageEntryCount;
}

StorageEntry *StorageCtrl::getEntries(int16_t index)
{
    return this->storageEntry[index];
}