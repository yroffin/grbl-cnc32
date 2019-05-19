#ifndef _STORAGE_CONTROLLER_
#define _STORAGE_CONTROLLER_

#include "SD.h"

class StorageEntry
{
public:
  boolean isDirectory()
  {
    return dir;
  }
  const char *getPath()
  {
    return path;
  }

  void setDirectory(boolean value)
  {
    this->dir = value;
  }

  void setPath(const char *path)
  {
    strcpy(this->path, path);
  }

private:
  boolean dir;
  char path[128];
};

class StorageCtrl
{
public:
  StorageCtrl();
  void init();

  void scan(const char *base);
  int16_t getCount();
  StorageEntry *getEntries(int16_t index);

  static StorageCtrl *instance();

protected:
  char cwd[128];
  int16_t storageEntryCount = 0;
  StorageEntry *storageEntry[128];
};

#endif
