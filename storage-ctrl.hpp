#ifndef _STORAGE_CONTROLLER_
#define _STORAGE_CONTROLLER_

#include "SD.h"
#include "utils.hpp"
#define MAXSIZE_OF_CWD 128
#define MAXSIZE_OF_ENTRY 128

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
    Utils::strcpy(this->path, path, MAXSIZE_OF_CWD);
  }

private:
  boolean dir;
  char path[MAXSIZE_OF_CWD];
};

class StorageCtrl
{
public:
  StorageCtrl();
  void init();

  void scan(const char *base);
  int16_t getCount();
  StorageEntry *getEntries(int16_t index);

  int open(const char *filename);
  boolean readline(char *buffer, int16_t maxLength);
  void close();

  static StorageCtrl *instance();

protected:
  char cwd[MAXSIZE_OF_CWD];
  int16_t storageEntryCount = 0;
  StorageEntry *storageEntry[MAXSIZE_OF_ENTRY];
  boolean isOpen = false;
  File file;
};

#endif
