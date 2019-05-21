#ifndef _MODEL_
#define _MODEL_

enum EventType
{
  touch,
  release,
  buttonDown,
  grblStatus,
  grblProperty,
  hideComponent,
  showComponent,
  fileGrid,
  fileSelect
};

class Event
{
public:
  EventType type;
  int16_t timestamp;
  int16_t sender;
  int16_t target;
  union {
    struct
    {
      int16_t x, y;
    } touch;
    char message[32];
  };
};

#define WIDGET_ID_DEFAULT 0x00000
#define WIDGET_ID_GRBL 0x08000

#define WIDGET_ID_SCREEN 0x00001
#define WIDGET_ID_LAYER_MENU 0x01001
#define WIDGET_ID_LAYER_MENU_FOOTER 0x01011
#define WIDGET_ID_LAYER_MENU_BTNA 0x01004
#define WIDGET_ID_LAYER_MENU_BTNB 0x01005
#define WIDGET_ID_LAYER_MENU_BTNC 0x01006
#define WIDGET_ID_LAYER_MENU_GRBL_STATUS 0x01020

#define WIDGET_ID_LAYER_CTRL 0x02001
// Warning joystick take many IDs
#define WIDGET_ID_LAYER_CTRL_JOYSTICK 0x02002
#define WIDGET_ID_LAYER_CTRL_HOME 0x02020
#define WIDGET_ID_LAYER_CTRL_UNLOCK 0x02021
#define WIDGET_ID_LAYER_CTRL_RESET 0x02022
#define WIDGET_ID_LAYER_CTRL_RESUME 0x02023
#define WIDGET_ID_LAYER_CTRL_PAUSE 0x02024
#define WIDGET_ID_LAYER_CTRL_STATUS 0x02025

#define WIDGET_ID_LAYER_STAT 0x03001
#define WIDGET_ID_LAYER_STAT_GRBL_STATUS 0x03011
#define WIDGET_ID_LAYER_STAT_GRBL_IO 0x03012

#define WIDGET_ID_LAYER_FILE 0x04001
#define WIDGET_ID_LAYER_FILE_LIST 0x04011
#define WIDGET_ID_LAYER_BTN_PRINT 0x04032
#define WIDGET_ID_LAYER_CWF 0x04034

#endif
