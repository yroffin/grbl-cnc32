#ifndef _EVENT_CONTROLLER_
#define _EVENT_CONTROLLER_

#include "TFT_eSPI_ms/TFT_eSPI.h"

enum EventType
{
  touch,
  release,
  buttonDown,
  grblStatus,
  grblProperty
};

struct Event
{
  EventType type;
  int16_t timestamp;
  int16_t sender;
  union {
    struct
    {
      int16_t x, y;
    } touch;
    char message[32];
  };
};

class EvtCtrl
{
public:
  EvtCtrl(TFT_eSPI &_tft);
  void init();
  void capture();
  void flush();
  int16_t countEvents();
  const Event *getEvent(int16_t index);

  // Events
  void touchEvent(int16_t _x, int16_t _y);
  void releaseEvent();
  void grblStatusEvent(const char *status);

protected:
  TFT_eSPI &tft;
  struct Event eventStore[128];
  int16_t count = 0;
  bool touched = false;
  uint32_t touchedTimestamp = 0;
};

#endif
