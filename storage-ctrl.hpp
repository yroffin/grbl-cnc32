#ifndef _STORAGE_CONTROLLER_
#define _STORAGE_CONTROLLER_

#include "SD.h"

#include "config.h"
#include "evt-ctrl.hpp"
#include "storage-ctrl.hpp"

class StorageCtrl
{
public:
  StorageCtrl(TFT_eSPI &_tft, EvtCtrl &evtCtrl);
  void mount();
  void config();

protected:
  TFT_eSPI &tft;
  EvtCtrl &evtCtrl;
};

#endif
