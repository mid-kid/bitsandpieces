#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>

#include "win-argv.h"

#ifdef _WIN32
#include <tchar.h>
#else
#define _tmain main
#endif

#ifdef _UNICODE
#define FMT_TS "%ls"
#else
#define FMT_TS "%s"
#endif

int _tmain(int argc, _TCHAR *argv[])
{
    _TCHAR *argv_quoted = win_argv_build((const _TCHAR **)argv);
    printf(FMT_TS "\n", argv_quoted);
}
