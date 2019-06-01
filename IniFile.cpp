#include "IniFile.h"
#include "utils.hpp"

#include <string.h>

IniFile::IniFile(const char *filename)
{
    if (strlen(filename) <= INI_FILE_MAX_FILENAME_LEN)
        Utils::strcpy(_filename, filename, INI_FILE_MAX_FILENAME_LEN);
    else
        _filename[0] = '\0';
}

IniFile::~IniFile()
{
}

// Retain only data printable
const char *filter(const char *value, size_t len)
{
    static char output[128];
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if (value[i] > 31 && value[i] < 128)
        {
            output[j++] = value[i];
        }
    }
    output[j++] = 0;
    return output;
}

// Store in global data
char *store(char *storage, char *src, uint16_t *pos)
{
    uint16_t from = *pos, save = *pos;
    memcpy(&(storage[from]), src, strlen(src));
    from += strlen(src);
    storage[from++] = 0;
    *pos = from;
    return &(storage[save]);
}

// Validate and store file in memory
bool IniFile::validate()
{
    char section[128];
    char key[128];
    char value[128];
    char buffer[128];
    File file = SD.open(_filename);
    uint16_t pos = 0;
    size_t size;
    for (int line = 1; (size = file.readBytesUntil('\n', buffer, 256)) > 0; line++)
    {
        memset(key, 0, 128);
        memset(value, 0, 128);
        const char *filtered = filter(buffer, size);
        switch (*filtered)
        {
        case '[':
            // Section
            memset(section, 0, 128);
            strncpy(section, &(filtered[1]), strlen(filtered) - 2);
            break;
        case ';':
        case '#':
            // Comment
            break;
        default:
            // Key and value (with =)
            {
                bool isKey = true;
                size_t ik = 0, iv = 0;
                memset(key, 0, 128);
                memset(value, 0, 128);
                for (int find = 0; find < strlen(filtered); find++)
                {
                    switch (filtered[find])
                    {
                    case '=':
                        isKey = false;
                        break;
                    default:
                        if (isKey)
                        {
                            key[ik++] = filtered[find];
                        }
                        else
                        {
                            value[iv++] = filtered[find];
                        }
                        break;
                    }
                }
            }
            break;
        }
        if (*key)
        {
            // store data in optimized way
            this->sections[count] = store(this->data, section, &pos);
            this->keys[count] = store(this->data, key, &pos);
            this->values[count] = store(this->data, value, &pos);
            count++;
        }
    }
    file.close();
}

bool IniFile::getValue(const char *section, const char *key, char *buffer, size_t len)
{
    for (int i = 0; i < this->count; i++)
    {
        if (strcmp(section, this->sections[i]) == 0 && strcmp(key, this->keys[i]) == 0)
        {
            memcpy(buffer, this->values[i], len);
            return true;
        }
    }
    return false;
}
