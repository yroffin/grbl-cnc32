#ifndef _INIFILE_H
#define _INIFILE_H

// Maximum length for filename, excluding NULL char 26 chars allows an
// 8.3 filename instead and 8.3 directory with a leading slash
#define INI_FILE_MAX_FILENAME_LEN 26

#include "SD.h"

class IniFile
{
public:
    // Create an IniFile object. It isn't opened until open() is called on it.
    IniFile(const char *filename);
    ~IniFile();

    // Validate file
    bool validate();

    // Get value
    bool getValue(const char *section, const char *key, char *value, size_t len);

private:
    char _filename[INI_FILE_MAX_FILENAME_LEN];
    char *sections[128];
    char *keys[128];
    char *values[128];
    char data[512];
    int16_t count = 0;
};

#endif
