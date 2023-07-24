#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <locale.h>

#include "win-utf.h"

#ifdef _WIN32
#include <tchar.h>
#else
#define _T(...) __VA_ARGS__
#define _tmain main
#endif

#ifdef _UNICODE
#define FMT_TS "%ls"
#else
#define FMT_TS "%s"
#endif

int _tmain(int argc, _TCHAR *argv[])
{
    printf("Orig:\n");
    for (int x = 0; x < argc; x++) {
        printf(FMT_TS "\n", argv[x]);
    }
    char *utf_argv[argc];
    for (int x = 0; x < argc; x++) {
        utf_argv[x] = tctoutf(argv[x]);
    }
    printf("UTF8:\n");
    for (int x = 0; x < argc; x++) {
        printf("%s\n", utf_argv[x]);
    }
    _TCHAR *tc_argv[argc];
    for (int x = 0; x < argc; x++) {
        tc_argv[x] = utftotc(utf_argv[x]);
    }
    printf("TCH:\n");
    for (int x = 0; x < argc; x++) {
        printf(FMT_TS "\n", tc_argv[x]);
    }
}
