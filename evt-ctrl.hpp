#ifndef _EVENT_CONTROLLER_
#define _EVENT_CONTROLLER_

#include "model.hpp"

class EvtCtrl
{
public:
  EvtCtrl();
  void init();
  void capture();
  void flush();
  int16_t countEvents();
  const Event *getEvent(int16_t index);

  // Events
  void touchEvent(int16_t _x, int16_t _y);
  void releaseEvent();
  void grblStatusEvent(const char *status);

  static EvtCtrl *instance();

protected:
  struct Event eventStore[128];
  int16_t count = 0;
  bool touched = false;
  uint32_t touchedTimestamp = 0;
};

#endif
