#ifndef _WIDGET_TFT_
#define _WIDGET_TFT_

#include "config.h"
#include "TFT_eSPI_ms/TFT_eSPI.h"
#include "language.h"
#include "grbl-ctrl.hpp"
#include "evt-ctrl.hpp"

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

class TFT_Widget
{
public:
  TFT_Widget();
  virtual bool notify(const Event *event);
  virtual void render();
  virtual void setOwner(TFT_Widget *owner);
  virtual void submit(Event *event);
  virtual void setPosition(int16_t _x, int16_t _y);
  virtual void setVisible(bool _active)
  {
    this->visible = _active;
  }
  virtual void setLabel(const char *_label)
  {
    strcpy(this->label, _label);
  }
  virtual bool isVisible()
  {
    return this->visible;
  }
  // Registry manipulation
  virtual TFT_Widget *findById(int16_t _id);
  virtual void setVisibleById(int16_t _id, bool _active);
  virtual void setLabelById(int16_t _id, const char *label);
  TFT_Widget *add(TFT_Widget *widget);

protected:
  void init(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h);
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  int16_t id;
  char name[32];
  char label[128];
  bool visible = true;
  // child
  TFT_Widget *owner = 0;
  TFT_Widget *children[32];
  int8_t child = 0;
  TFT_eSPI *tft;
};

class TFT_Layer : public TFT_Widget
{
public:
  TFT_Layer(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 320, int16_t _h = 200);

private:
};

enum ButtonState
{
  on,
  off
};

class TFT_Button : public TFT_Widget
{
public:
  TFT_Button(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w = 40, int16_t _h = 40);
  virtual bool notify(const Event *event);
  virtual void render();

private:
  ButtonState state = off;
};

class TFT_Joystick : public TFT_Widget
{
public:
  TFT_Joystick(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w = 40, int16_t _h = 40);

private:
};

class TFT_Label : public TFT_Widget
{
public:
  TFT_Label(int16_t _id, const char *_label, int16_t _x, int16_t _y);
  virtual void render();

private:
};

class TFT_Screen : public TFT_Widget
{
public:
  TFT_Screen();
  void init();
  void calibrate();
  virtual bool notify(const Event *event);
  bool isInvalidated();
  void render();
  void menuLayer();
  void controlLayer();
  void statLayer();
  virtual void submit(Event *event);
  void status(const char *message);
  boolean getTouch(int16_t *x, int16_t *y);

  static TFT_Screen *instance();

private:
};

#endif
