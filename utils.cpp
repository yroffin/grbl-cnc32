#include "utils.hpp"

const char *Utils::strcpy(char *dest, const char *src, size_t size)
{
    int16_t len = max(size - 1, strlen(src));
    strncpy(dest, src, len);
    dest[len] = 0;
}

char __vsprintfBuffer[1024];

char *Utils::vsprintfBuffer()
{
    return (char *)__vsprintfBuffer;
}
