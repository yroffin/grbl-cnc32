#include "config.h"
#include "storage-ctrl.hpp"
#include "utils.hpp"

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
    if (!SD.exists("/"))
    { // check if root exist
        log_e("Root ('/') not found on SD card");
        return;
    }
    // Set "/" as root folder
    Utils::strcpy(cwd, "/", MAXSIZE_OF_CWD);
    for (int i = 0; i < MAXSIZE_OF_ENTRY; i++)
    {
        this->storageEntry[i] = 0;
    }
    log_i("File system ok.");
}

int basename(const char *filename)
{
    int l = strlen(filename) - 1;
    for (; l >= 0 && filename[l] != '/'; l--)
        ;
    return l + 1;
}

void StorageCtrl::scan(const char *base)
{
    File root = SD.open(base);
    File entry = root.openNextFile();
    for (; entry; entry = root.openNextFile())
    {
        // ignore hidden files
        if (entry.name()[basename(entry.name())] == '.')
        {
            continue;
        }
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

int StorageCtrl::open(const char *filename)
{
    if (this->isOpen)
    {
        this->file.close();
    }
    this->file = SD.open(filename);
    // computes lines
    int lines = 0;
    char buffer[1024];
    for (; this->file.available();)
    {
        int sz = this->file.readBytes(buffer, 1024);
        for (int i = 0; i < sz; i++)
            if (buffer[i] == 10)
                lines++;
        ;
    }
    this->file.close();
    this->file = SD.open(filename);
    return lines;
}

boolean StorageCtrl::readline(char *buffer, int16_t maxLength)
{
    if (this->file.available())
    {
        const char *value = this->file.readStringUntil(10).c_str();
        size_t m = min(strlen(value), (size_t)maxLength - 1);
        strncpy(buffer, value, m);
        buffer[m] = 0;
        return true;
    }
    return false;
}

void StorageCtrl::close()
{
    this->file.close();
    this->isOpen = false;
}
