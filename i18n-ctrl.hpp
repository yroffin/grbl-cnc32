#ifndef _I18N_CONTROLLER_
#define _I18N_CONTROLLER_

#include "stdio.h"
#include "stdarg.h"
#include "TFT_eSPI_ms/TFT_eSPI.h"

#define MAXSIZE_OF_I18N_MESSAGE 256

enum STD_CAT
{
  I18N_STD,
  I18N_GRBL
};

enum STD_MSG
{
  I18N_ZERO,
  I18N_INIT,
  I18N_OK,
  I18N_WIFI_TRYING,
  I18N_WIFI_IP,
  I18N_WIFI_KO,
  I18N_WIFI_SERVE
};

class I18nCtrl
{
public:
  I18nCtrl();
  static I18nCtrl *instance();

  virtual const char *grblError(int code)
  {
    if (code < 0 || code > 38)
    {
      code = 0;
    }
    return en_grbl_error[code];
  }

  virtual const char *std(int code)
  {
    if (code < 0 || code > 99)
    {
      code = 0;
    }
    return en_default[code];
  }

  virtual const char *translate(int type, int code, ...);

private:
  char i18n_message[MAXSIZE_OF_I18N_MESSAGE];
  char *en_default[9] = {
      "Unknown ...",
      "%s init ...",
      "%s ok",
      "trying to connect to %s",
      "wifi ip is %s",
      "fail to connect to %s",
      "serving data on port %d"};

  char *
      en_grbl_error[39] = {
          "0.Unknown error",
          "1.Expected cmd letter",
          "2.Bad number format",
          "3.Invalid $ sytem cmd",
          "4.Negative value",
          "5.Homing not enabled",
          "6.Step pulse <3 usec",
          "7.EEPROM read fail",
          "8. $ while not IDLE",
          "9.Locked(alarm or jog)",
          "10.Soft limit(no Homing)",
          "11.Line overflow",
          "12.Step rate to high",
          "13.Safety door detected",
          "14.Line length exceeded",
          "15.Jog travel exceeded",
          "16.Invalid jog command",
          "17.Laser requires PWM",
          "18.Unknown error",
          "19.Unknown error",
          "20.Unsupported command",
          "21.Modal group violation",
          "22.Undef. feed rate",
          "23.Cmd requires integer",
          "24.Several axis Gcode",
          "25.Repeated Gcode",
          "26.Axis missing in Gcode",
          "27.Invalid line number",
          "28.Value missing in Gcode",
          "29.G59 WCS not supported",
          "30.G53 without G0 and G1",
          "31.Axis not allowed",
          "32.G2,G3 require a plane",
          "33.Invalid motion target",
          "34.Invalid arc radius",
          "35.G2,G3 require offset",
          "36.Unused value",
          "37.G43.1 tool length",
          "38.Tool number > max"};
};

#endif
