#ifndef _STORAGE_CONTROLLER_
#define _STORAGE_CONTROLLER_

#include "SD.h"

#include "config.h"
#include "evt-ctrl.hpp"
#include "storage-ctrl.hpp"

class StorageCtrl
{
public:
  StorageCtrl();
  void init();
  void config();
  void refresh();

  static StorageCtrl *instance();

protected:
};

#endif
