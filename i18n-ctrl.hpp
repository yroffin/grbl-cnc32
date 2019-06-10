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

#define I18N_STD "Std"
#define I18N_GRBL "grbl"

class I18nCtrl : public JsonStore
{
public:
  I18nCtrl();
  void setup();
  static I18nCtrl *instance();

  virtual const char *getKey(const char *k1, const char *k2);
  virtual const char *translate(const char *section, const char *code, ...);

private:
  StaticJsonDocument<JSON_I18N_SIZE> i18n;

private:
  char i18n_message[MAXSIZE_OF_I18N_MESSAGE];
};

#endif
