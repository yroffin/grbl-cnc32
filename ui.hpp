#ifndef _UI_TFT_
#define _UI_TFT_

#include "widget.hpp"
#include "utils.hpp"

#define MAXSIZE_OF_LOG_MESSAGE 1024
#define MAXSIZE_OF_VSPRINTF_MESSAGE 256

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
  TFT_Button *print;
  TFT_Button *unlock;
  TFT_Button *reset;
  TFT_Button *status;
  TFT_Button *pause;
  TFT_Button *resume;
  TFT_Console *grblCommand;
  char log_message[MAXSIZE_OF_LOG_MESSAGE];
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
  TFT_Button *home;
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
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), format, args);
    va_end(args);
    this->grblIoStatusValues->setLabel(Utils::vsprintfBuffer());
  }
  void outputConsole(const char *format, ...);

protected:
  TFT_Group *group;
  TFT_Group *groupNunchuk;
  TFT_Label *title;
  TFT_Label *grblStatusLabel;
  TFT_Label *grblStatusValue;
  TFT_Label *grblIoStatus;
  TFT_Label *grblIoStatusValues;
  TFT_Label *grblMpos;
  TFT_Label *grblMposValue;
  TFT_Label *grblWpos;
  TFT_Label *grblWposValue;
  TFT_Label *nunchuk;
  TFT_Label *nunchukZ;
  TFT_Label *nunchukC;
  TFT_Label *nunchukMove;
  TFT_Label *nunchukPlane;
  TFT_Label *nunchukLader;
  TFT_Console *console;
  char log_message[MAXSIZE_OF_LOG_MESSAGE];
};

class TFT_LayerFile : public TFT_Layer
{
public:
  TFT_LayerFile(int16_t _id, int16_t _x, int16_t _y, int16_t _w = 10, int16_t _h = 10);
  void notify(const Event *event);
  virtual void refresh();

protected:
  TFT_Group *group;
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

public:
  TFT_StatusBar *status;

protected:
  // local component
  TFT_Label *title;
  TFT_Button *a;
  TFT_Button *b;
  TFT_Button *c;
  TFT_Button *d;
  char log_message[MAXSIZE_OF_LOG_MESSAGE];
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

  void notifyWrite(uint16_t sz);
  void notifyBusy(boolean _busyState);
  void notifyWifiStatus(const char *status);
  void notifyPrintStatus(boolean isPrinting, int printed, int toPrint);

public:
  TFT_LayerMenu *menu;
  TFT_LayerAdmin *admin;
  TFT_LayerControl *control;
  TFT_LayerStatistic *statistic;
  TFT_LayerFile *file;
  char log_message[MAXSIZE_OF_LOG_MESSAGE];
};

#endif
