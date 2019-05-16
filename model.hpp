#ifndef _MODEL_
#define _MODEL_

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

#endif
