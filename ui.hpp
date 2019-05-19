#ifndef _UI_TFT_
#define _UI_TFT_

#include "widget.hpp"

class TFT_LayerMenu : public TFT_Layer
{
public:
  TFT_LayerMenu(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 320, int16_t _h = 200);
  virtual void notify(const Event *event);

protected:
  TFT_Label *title;
  TFT_Button *a;
  TFT_Button *b;
  TFT_Button *c;
  TFT_Label *footer;
};

class TFT_LayerControl : public TFT_Layer
{
public:
  TFT_LayerControl(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 320, int16_t _h = 200);

protected:
  TFT_Layer *layer;
  TFT_Label *title;
  TFT_Button *home;
  TFT_Button *unlock;
  TFT_Button *reset;
  TFT_Button *status;
  TFT_Button *pause;
  TFT_Button *resume;
  TFT_Joystick *joystick;
};

class TFT_LayerStatistic : public TFT_Layer
{
public:
  TFT_LayerStatistic(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 320, int16_t _h = 200);
  virtual void notify(const Event *event);
  void write(const char *message);

protected:
  TFT_Label *title;
  TFT_Label *grblStatusLabel;
  TFT_Label *grblStatusValue;
  TFT_Label *grblIoStatus;
  TFT_Label *grblIoStatusValues;
  TFT_Console *console;
};

class TFT_LayerFile : public TFT_Layer
{
public:
  TFT_LayerFile(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 320, int16_t _h = 200);
  void notify(const Event *event);
  virtual void refresh();

protected:
  TFT_Label *title;
  TFT_Label *dir;
  TFT_Label *cwd;
  TFT_Label *file;
  TFT_Label *cwf;
  TFT_Label *misc;
  TFT_Label *miscValue;
  TFT_FileGrid *files;
};

class TFT_Screen : public TFT_Widget
{
public:
  TFT_Screen();
  void init();
  void calibrate();
  virtual void notify(const Event *event);
  void render();
  void status(const char *message, ...);
  boolean getTouch(int16_t *x, int16_t *y);

  static TFT_Screen *instance();

private:
  TFT_LayerMenu *menu;
  TFT_LayerControl *control;
  TFT_LayerStatistic *statistic;
  TFT_LayerFile *file;
};

#endif
