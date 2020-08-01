#include "config.h"
#include "storage-ctrl.hpp"
#include "json-config.hpp"
#include "utils.hpp"
#include "i18n-ctrl.hpp"

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
    this->state = FILES;
}

int _write(const char *filename, const char *data, bool remove)
{
    if (remove && SD.exists(filename))
    {
        SD.remove(filename);
    }
    File file = SD.open(filename, FILE_APPEND);
    int count = file.println(data);
    file.close();
    return count;
}

int StorageCtrl::touch(const char *filename, const char *data)
{
    return _write(filename, data, true);
}

int StorageCtrl::append(const char *filename, const char *data)
{
    return _write(filename, data, false);
}

selector StorageCtrl::files()
{
    this->state = FILES;
    return this->state;
}

selector StorageCtrl::commands()
{
    this->state = COMMANDS;
    return this->state;
}

boolean StorageCtrl::isFiles()
{
    return this->state == FILES;
}

boolean StorageCtrl::isCommands()
{
    return this->state == COMMANDS;
}

int StorageCtrl::scan()
{
    switch (this->state)
    {
    case FILES:
        return this->fileStore.scan("/");
        break;
    case COMMANDS:
        return this->cmdStore.scan("/");
        break;
    }
}

const char *StorageCtrl::path(int index)
{
    switch (this->state)
    {
    case FILES:
        return this->fileStore.get(index)->getPath();
        break;
    case COMMANDS:
        return this->cmdStore.get(index)->getPath();
        break;
    }
}

boolean StorageCtrl::isFile(int index)
{
    switch (this->state)
    {
    case FILES:
        return !this->fileStore.get(index)->isDirectory();
        break;
    case COMMANDS:
        return false;
    }
}

int StorageCtrl::open(const char *filename)
{
    switch (this->state)
    {
    case FILES:
        return this->fileStore.open(filename);
        break;
    case COMMANDS:
        return this->cmdStore.open(filename);
        break;
    }
}

boolean StorageCtrl::readline(char *buffer, int16_t maxLength)
{
    switch (this->state)
    {
    case FILES:
        return this->fileStore.readline(buffer, maxLength);
        break;
    case COMMANDS:
        return this->cmdStore.readline(buffer, maxLength);
        break;
    }
}

void StorageCtrl::close()
{
    switch (this->state)
    {
    case FILES:
        this->fileStore.close();
        break;
    case COMMANDS:
        this->cmdStore.close();
        break;
    }
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
        int sz = this->file.readBytes(buffer, sizeof(buffer));
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

int StorageCmdStore::scan(const char *base)
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();
    if (jsonConfig->store.containsKey("commands"))
    {
        // clear all entry
        this->clear();

        JsonObject root = jsonConfig->store["commands"].as<JsonObject>();
        for (JsonObject::iterator it = root.begin(); it != root.end(); ++it)
        {
            this->addFile(it->key().c_str());
        }
    }
    return this->storageEntryCount;
}

int StorageCmdStore::open(const char *filename)
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();
    this->offset = 0;
    Utils::strcpy(this->name, filename, sizeof(this->name));
    return jsonConfig->store["commands"][this->name].size();
}

boolean StorageCmdStore::readline(char *buffer, int16_t maxLength)
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();
    if (this->offset < jsonConfig->store["commands"][this->name].size())
    {
        Utils::strcpy(buffer, jsonConfig->store["commands"][this->name][this->offset++], maxLength);
        return true;
    }
    return false;
}

void StorageCmdStore::close()
{
}
