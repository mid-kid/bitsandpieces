#pragma once

#ifdef _WIN32
#include <tchar.h>
#else
typedef char _TCHAR;
#endif

char *tctoutf(const _TCHAR *str);
_TCHAR *utftotc(const char *str);
