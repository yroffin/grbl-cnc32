#ifndef _STORAGE_CONTROLLER_
#define _STORAGE_CONTROLLER_

#include "SD.h"
#include "utils.hpp"
#define MAXSIZE_OF_CWD 128
#define MAXSIZE_OF_ENTRY 128

class StorageEntry
{
public:
  boolean isDirectory();
  const char *getPath();
  void setDirectory(boolean value);
  void setPath(const char *path);
  void clear();

private:
  boolean dir;
  char path[MAXSIZE_OF_CWD];
};

class StorageStore
{
public:
  StorageStore();
  void clear();
  StorageEntry *get(int index);
  void addDirectory(const char *value);
  void addFile(const char *value);

  virtual int scan(const char *base) {}
  virtual int open(const char *filename) {}
  virtual boolean readline(char *buffer, int16_t maxLength) {}
  virtual void close() {}

protected:

  int16_t storageEntryCount = 0;
  StorageEntry *storageEntry[MAXSIZE_OF_ENTRY];
};

class StorageFileStore : public StorageStore
{
public:
  virtual int scan(const char *base);
  virtual int open(const char *filename);
  virtual boolean readline(char *buffer, int16_t maxLength);
  virtual void close();

private:
  char cwd[MAXSIZE_OF_CWD];
  boolean isOpen = false;
  File file;
};

class StorageCtrl
{
public:
  StorageCtrl();
  void init();

  int scan();
  const char *path(int index);
  int open(const char *filename);
  boolean readline(char *buffer, int16_t maxLength);
  void close();

  static StorageCtrl *instance();

protected:
  StorageFileStore fileStore;
};

#endif
