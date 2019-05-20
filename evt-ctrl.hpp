#ifndef _EVENT_CONTROLLER_
#define _EVENT_CONTROLLER_

#include "model.hpp"

class EvtCtrl
{
public:
  EvtCtrl();
  void init();
  void capture();
  void dispatchPrimaryEvents();
  boolean dispatchSecondaryEvents();
  void flush();

  // Events
  void touchEvent(int16_t sender, int16_t _x, int16_t _y);
  void releaseEvent(int16_t sender);
  void grblStatusEvent(int16_t sender, const char *status);
  void showEvent(int16_t sender, int16_t target);
  void hideEvent(int16_t sender, int16_t target);
  void buttonDownEvent(int16_t sender);
  void fileGridEvent(int16_t sender);
  void fileGridSelect(int16_t sender, const char *file);

  static EvtCtrl *instance();

protected:
  struct Event eventStore[128];
  int16_t count = 0;
  int16_t primaryEvents = 0;
  bool touched = false;
  uint32_t touchedTimestamp = 0;
};

#endif
