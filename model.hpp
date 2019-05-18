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

#define WIDGET_ID_DEFAULT 0x00000

#define WIDGET_ID_SCREEN 0x00001
#define WIDGET_ID_LAYER_MENU 0x01001
#define WIDGET_ID_LAYER_MENU_FOOTER 0x01011
#define WIDGET_ID_LAYER_MENU_BTNA 0x01004
#define WIDGET_ID_LAYER_MENU_BTNB 0x01005

#define WIDGET_ID_LAYER_CTRL 0x02001
#define WIDGET_ID_LAYER_CTRL_JOYSTICK 0x02002

#define WIDGET_ID_LAYER_STAT 0x03001
#define WIDGET_ID_LAYER_STAT_GRBL_STATUS 0x03011

#endif
