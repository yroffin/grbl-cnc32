#ifndef _UTILS_
#define _UTILS_

#include "WiFi.h"
#include "TFT_eSPI_ms/TFT_eSPI.h"

class Utils
{
public:
  static const char *strcpy(char *dest, const char *src, size_t size);
  static char *vsprintfBuffer();
  static time_t unixTime();
  static time_t fixTime();

protected:
};

#endif
