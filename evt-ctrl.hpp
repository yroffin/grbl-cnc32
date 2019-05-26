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

  void send(int16_t sender, EventType action);
  void sendInt(int16_t sender, EventType action, int value);
  void sendTouch(int16_t sender, EventType action, int16_t _x, int16_t _y);
  void sendWithString(int16_t sender, EventType action, const char *value);
  void sendWithFloat(int16_t sender, EventType action, float value);
  void sendWithVector(int16_t sender, EventType action, float f1, float f2, float f3);

  static EvtCtrl *instance();

protected:
  struct Event eventStore[256];
  int16_t count = 0;
  int16_t primaryEvents = 0;
  bool touched = false;
  uint32_t touchedTimestamp = 0;
};

#endif
