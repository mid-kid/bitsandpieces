#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "win-utf.h"

#include <stdlib.h>
#include <string.h>

char *tctoutf(const _TCHAR *str)
{
    if (!str) return NULL;
#ifdef _UNICODE
    size_t size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    if (!size) return NULL;
    char *res = malloc(size);
    if (!res) return NULL;
    WideCharToMultiByte(CP_UTF8, 0, str, -1, res, size, NULL, NULL);
    return res;
#else
    return strdup(str);
#endif
}

_TCHAR *utftotc(const char *str)
{
    if (!str) return NULL;
#ifdef _UNICODE
    size_t size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    if (!size) return NULL;
    _TCHAR *res = malloc(size);
    if (!res) return NULL;
    MultiByteToWideChar(CP_UTF8, 0, str, -1, res, size);
    return res;
#else
    return strdup(str);
#endif
}
