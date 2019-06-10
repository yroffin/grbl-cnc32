#ifndef _I18N_CONTROLLER_
#define _I18N_CONTROLLER_

#include "json-store.hpp"

#include "config.h"

#include "stdio.h"
#include "stdarg.h"

#include "ArduinoJson/ArduinoJson.h"
#include <SD.h>
#include <SPI.h>

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

class I18nCtrl : public JsonStore
{
public:
  I18nCtrl();
  void setup();
  static I18nCtrl *instance();

  virtual const char *getKey(const char *k1, const char *k2);
  virtual const char *grblError(int code);
  virtual const char *std(int code);
  virtual const char *translate(int type, int code, ...);

private:
  StaticJsonDocument<JSON_I18N_SIZE> i18n;

private:
  char i18n_message[MAXSIZE_OF_I18N_MESSAGE];
};

#endif
