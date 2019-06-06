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
}

int StorageCtrl::scan()
{
    return this->fileStore.scan("/");
}

const char *StorageCtrl::path(int index)
{
    return this->fileStore.get(index)->getPath();
}

int StorageCtrl::open(const char *filename)
{
    return this->fileStore.open(filename);
}

boolean StorageCtrl::readline(char *buffer, int16_t maxLength)
{
    return this->fileStore.readline(buffer, maxLength);
}

void StorageCtrl::close()
{
    this->fileStore.close();
}

boolean StorageEntry::isDirectory()
{
    return dir;
}

const char *StorageEntry::getPath()
{
    return path;
}

void StorageEntry::setDirectory(boolean value)
{
    this->dir = value;
}

void StorageEntry::setPath(const char *path)
{
    Utils::strcpy(this->path, path, MAXSIZE_OF_CWD);
}

void StorageEntry::clear()
{
    Utils::strcpy(this->path, "", MAXSIZE_OF_CWD);
}

StorageStore::StorageStore()
{
    // clear all entry
    for (int i = 0; i < MAXSIZE_OF_ENTRY; i++)
    {
        this->storageEntry[i] = new StorageEntry();
    }
}

void StorageStore::clear()
{
    // clear all entry
    for (int i = 0; i < MAXSIZE_OF_ENTRY; i++)
    {
        this->storageEntry[i]->clear();
    }
    this->storageEntryCount = 0;
}

StorageEntry *StorageStore::get(int index)
{
    return this->storageEntry[index];
}

void StorageStore::addDirectory(const char *value)
{
    this->storageEntry[this->storageEntryCount]->setDirectory(true);
    this->storageEntry[this->storageEntryCount]->setPath(value);
    this->storageEntryCount++;
}

void StorageStore::addFile(const char *value)
{
    this->storageEntry[this->storageEntryCount]->setDirectory(false);
    this->storageEntry[this->storageEntryCount]->setPath(value);
    this->storageEntryCount++;
}

int StorageFileStore::scan(const char *base)
{
    if (!SD.exists(base))
    {
        return 0;
    }

    // Fix cwd
    Utils::strcpy(cwd, base, MAXSIZE_OF_CWD);

    // clear all entry
    this->clear();

    File root = SD.open(base);
    File entry = root.openNextFile();
    for (; entry; entry = root.openNextFile())
    {
        // ignore hidden files
        if (entry.name()[Utils::basename(entry.name())] == '.')
        {
            continue;
        }
        if (entry.isDirectory())
        {
            this->addDirectory(entry.name());
        }
        else
        {
            this->addFile(entry.name());
        }
    }
    return this->storageEntryCount;
}

int StorageFileStore::open(const char *filename)
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

boolean StorageFileStore::readline(char *buffer, int16_t maxLength)
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

void StorageFileStore::close()
{
    this->file.close();
    this->isOpen = false;
}
