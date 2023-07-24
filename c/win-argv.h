#pragma once

#ifdef _WIN32
#include <tchar.h>
#else
typedef char _TCHAR;
#endif

_TCHAR *win_argv_build(const _TCHAR *const *argv);
