#ifndef _WIDGET_TFT_
#define _WIDGET_TFT_

#include "stdio.h"
#include "config.h"
#include "TFT_eSPI_ms/TFT_eSPI.h"
#include "language.h"
#include "grbl-ctrl.hpp"
#include "evt-ctrl.hpp"

class TFT_Widget
{
public:
  TFT_Widget();
  virtual void notify(const Event *event);
  virtual void render();
  virtual void setOwner(TFT_Widget *owner);
  virtual void submit(Event *event);
  virtual void setPosition(int16_t _x, int16_t _y);
  virtual void setVisible(bool _active);
  virtual void setInvalidated(bool _active);
  virtual void setLabel(const char *_label, ...);
  virtual const char *getLabel()
  {
    return this->label;
  }
  virtual bool isVisible()
  {
    return this->visible;
  }
  // Registry manipulation
  virtual TFT_Widget *findById(int16_t _id);
  virtual void setVisibleById(int16_t _id, bool _active);
  virtual void setLabelById(int16_t _id, const char *label, ...);
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
  bool invalidated = true;
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
  virtual void notify(const Event *event);
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

class TFT_Console : public TFT_Widget
{
public:
  TFT_Console(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w = 40, int16_t _h = 40);
  void write(const char *message);

private:
  TFT_Label *lines[6];
};

class TFT_Screen : public TFT_Widget
{
public:
  TFT_Screen();
  void init();
  void calibrate();
  virtual void notify(const Event *event);
  void dispatch();
  void render();
  void menuLayer();
  void controlLayer();
  void statLayer();
  virtual void submit(Event *event);
  void status(const char *message, ...);
  boolean getTouch(int16_t *x, int16_t *y);

  static TFT_Screen *instance();

private:
  TFT_Console *console;
};

#endif
