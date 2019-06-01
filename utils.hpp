#ifndef _UTILS_
#define _UTILS_

#include "TFT_eSPI_ms/TFT_eSPI.h"

class Utils
{
public:
  static const char *strcpy(char *dest, const char *src, size_t size);
  static char *vsprintfBuffer();

protected:
};

#endif
