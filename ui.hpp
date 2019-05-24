#ifndef _UI_TFT_
#define _UI_TFT_

#include "widget.hpp"

class TFT_LayerAdmin : public TFT_Layer
{
public:
  TFT_LayerAdmin(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 10, int16_t _h = 10);
  void grblInputConsole(const char *format, ...);
  void grblOutputConsole(const char *format, ...);

protected:
  TFT_Group *group;
  TFT_Layer *layer;
  TFT_Label *title;
  TFT_Button *home;
  TFT_Button *unlock;
  TFT_Button *reset;
  TFT_Button *status;
  TFT_Button *pause;
  TFT_Button *resume;
  TFT_Console *grblCommand;
};

class TFT_LayerControl : public TFT_Layer
{
public:
  TFT_LayerControl(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 10, int16_t _h = 10);

protected:
  TFT_Group *group;
  TFT_Layer *layer;
  TFT_Label *title;
  TFT_Button *setx;
  TFT_Button *sety;
  TFT_Button *setz;
  TFT_Button *setall;
  TFT_Joystick *joystick;
};

class TFT_LayerStatistic : public TFT_Layer
{
public:
  TFT_LayerStatistic(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 10, int16_t _h = 10);
  virtual void notify(const Event *event);
  virtual void setGrblIo(const char *format, ...)
  {
    va_list args;
    va_start(args, format);
    static char msg[64];
    vsprintf(msg, format, args);
    va_end(args);
    this->grblIoStatusValues->setLabel(msg);
  }
  void outputConsole(const char *format, ...);

protected:
  TFT_Group *group;
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
  TFT_LayerFile(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 10, int16_t _h = 10);
  void notify(const Event *event);
  virtual void refresh();

protected:
  TFT_Group *group;
  TFT_Button *print;
  TFT_Label *title;
  TFT_Label *dir;
  TFT_Label *cwd;
  TFT_Label *file;
  TFT_Label *cwf;
  TFT_Label *misc;
  TFT_Label *miscValue;
  TFT_FileGrid *files;
};

class TFT_LayerMenu : public TFT_Layer
{
public:
  TFT_LayerMenu(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 10, int16_t _h = 10);
  virtual void notify(const Event *event);

protected:
  // local component
  TFT_Label *title;
  TFT_Button *a;
  TFT_Button *b;
  TFT_Button *c;
  TFT_Button *d;
};

class TFT_Screen : public TFT_Widget
{
public:
  TFT_Screen();
  void init();
  void calibrate();
  virtual void notify(const Event *event);
  boolean getTouch(int16_t *x, int16_t *y);

  void outputConsole(const char *format, ...);
  void grblInputConsole(const char *format, ...);
  void grblOutputConsole(const char *format, ...);

  static TFT_Screen *instance();

public:
  TFT_LayerMenu *menu;
  TFT_LayerAdmin *admin;
  TFT_LayerControl *control;
  TFT_LayerStatistic *statistic;
  TFT_LayerFile *file;
};

#endif
