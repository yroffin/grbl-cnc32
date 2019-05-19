#ifndef _WIDGET_TFT_
#define _WIDGET_TFT_

#include "stdio.h"
#include "config.h"
#include "TFT_eSPI_ms/TFT_eSPI.h"
#include "language.h"
#include "evt-ctrl.hpp"

class TFT_Widget
{
public:
  TFT_Widget();
  virtual void notify(const Event *event);
  virtual void render();
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
  TFT_Button *xleft;
  TFT_Button *xright;
  TFT_Button *yup;
  TFT_Button *ydown;
  TFT_Button *zup;
  TFT_Button *zdown;
  TFT_Button *pas;
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

class TFT_FileGrid : public TFT_Widget
{
public:
  TFT_FileGrid(int16_t _id, void (*onLeft)(TFT_FileGrid *), void (*onRight)(TFT_FileGrid *), const char *_label, int16_t _x, int16_t _y, int16_t _w = 40, int16_t _h = 40);
  virtual void notify(const Event *event);

  virtual void onRight()
  {
    this->onRightCallback(this);
  }
  virtual void onLeft()
  {
    this->onLeftCallback(this);
  }

  void clear();
  void set(int16_t index, const char *label);

private:
  void (*onLeftCallback)(TFT_FileGrid *);
  void (*onRightCallback)(TFT_FileGrid *);

  TFT_Button *left;
  TFT_Button *lines[4];
  TFT_Button *right;
};

#endif
