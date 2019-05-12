#ifndef _WIDGET_TFT_
#define _WIDGET_TFT_

#include "evt-ctrl.hpp"
#include "TFT_eSPI_ms/TFT_eSPI.h"
#include "language.h"

#define WIDGET_ID_DEFAULT 0x00000

#define WIDGET_ID_SCREEN 0x00001
#define WIDGET_ID_LAYER_MENU 0x01001
#define WIDGET_ID_LAYER_MENU_FOOTER 0x01011
#define WIDGET_ID_LAYER_MENU_BTNA 0x01004
#define WIDGET_ID_LAYER_MENU_BTNB 0x01005

#define WIDGET_ID_LAYER_CTRL 0x02001
#define WIDGET_ID_LAYER_CTRL_BTNXP 0x02001
#define WIDGET_ID_LAYER_CTRL_BTNXM 0x02003
#define WIDGET_ID_LAYER_CTRL_BTNYP 0x02004
#define WIDGET_ID_LAYER_CTRL_BTNYM 0x02005
#define WIDGET_ID_LAYER_CTRL_BTNZP 0x02005
#define WIDGET_ID_LAYER_CTRL_BTNZM 0x02007

#define WIDGET_ID_LAYER_STAT 0x03001

class TFT_Widget {
public:
  TFT_Widget();
  virtual  bool notify(const Event *event) {
  };
  virtual  void render() = 0;
  virtual  void setOwner(TFT_Widget *owner);
  virtual  void submit(Event *event);
  virtual  void setPosition(int16_t _x, int16_t _y) {
    this->x += _x;
    this->y += _y;
  }
  virtual  void setVisible(bool _active) {
    this->visible = _active;
  }
  virtual  void setLabel(char *_label) {
    strcpy(this->label, _label);
  }
  virtual  bool isVisible() {
    return this->visible;
  }
  // Registry manipulation
  virtual TFT_Widget *findById(int16_t _id);
  virtual void setVisibleById(int16_t _id, bool _active);
  virtual void setLabelById(int16_t _id, char *label);
protected:
  void init(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h);
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  TFT_Widget *owner = 0;
  int16_t id;
  char name[32];
  char label[32];
  bool visible = false;
};

class TFT_Layer : public TFT_Widget {
public:
  TFT_Layer(TFT_eSPI& _tft, int16_t _id, int16_t _x, int16_t _y, int16_t _w = 320, int16_t _h = 200);
  virtual bool notify(const Event *event);
  virtual void render();
  TFT_Widget *add(TFT_Widget *widget);
private:
  TFT_eSPI& tft;
  TFT_Widget *widgets[16];
  int8_t count = 0;
};

enum ButtonState {
  on, off
};

class TFT_Button : public TFT_Widget {
public:
  TFT_Button(TFT_eSPI& _tft, int16_t _id, const char * _label, int16_t _x, int16_t _y, int16_t _w = 32, int16_t _h = 32);
  virtual bool notify(const Event *event);
  virtual void render();
private:
  TFT_eSPI& tft;
  ButtonState state = off;
};

class TFT_Label : public TFT_Widget {
public:
  TFT_Label(TFT_eSPI& _tft, int16_t _id, const char * _label, int16_t _x, int16_t _y);
  virtual void render();
private:
  TFT_eSPI& tft;
};

class TFT_Screen : public TFT_Widget {
public:
  TFT_Screen(TFT_eSPI& _tft, EvtCtrl& _evtCtrl);
  void init();
  void calibrate();
  virtual bool notify(const Event *event);
  bool isInvalidated();
  void render();
  void menuLayer();
  void controlLayer();
  void statLayer();
  virtual void submit(Event *event);
  TFT_Layer *add(TFT_Layer *layer);
private:
  TFT_eSPI& tft;
  EvtCtrl& evtCtrl;
  TFT_Layer *layers[16];
  int8_t count = 0;
};

#endif
