#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "win-argv.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <tchar.h>
#else
#define _T(...) __VA_ARGS__
#define _tcscspn(...) strcspn(__VA_ARGS__)
#define _tcslen(...) strlen(__VA_ARGS__)
#endif

static void str_resize(_TCHAR **str, size_t *max, size_t req)
{
    if (*max < req) {
        while (*max < req) *max += 1024;
        *str = realloc(*str, sizeof(_TCHAR) * *max);
    }
}

// Windows argv[] strings are passed as a whole, instead of being split up into
// an array. This means that *we* have to make sure to quote our arguments, and
// for some reason nobody ever thought to make a library for this.
_TCHAR *win_argv_build(const _TCHAR *const *argv)
{
    size_t args_max = 1024;
    size_t args_pos = 0;
    _TCHAR *args = malloc(sizeof(_TCHAR) * args_max);

    int first = 1;

    while (*argv) {
        const _TCHAR *arg = *argv;

        // Add a space between arguments
        if (!first) {
            str_resize(&args, &args_max, args_pos + 1);
            args[args_pos++] = ' ';
        }
        first = 0;

        // If it doesn't contain any special chars, copy as-is
        size_t arg_len = _tcslen(arg);
        if (_tcscspn(arg, _T(" \t\n\r")) == arg_len) {
            str_resize(&args, &args_max, args_pos + arg_len);
            memcpy(args + args_pos, arg, sizeof(_TCHAR) * arg_len);
            args_pos += arg_len;
            argv++;
            continue;
        }

        // Quote the argument
        int backslashes = 0;
        str_resize(&args, &args_max, args_pos + 1);
        args[args_pos++] = '"';
        while (*arg) {
            if (*arg == '\\') {
                backslashes++;
            } else if (*arg == '"') {
                // Add extra backslashes to escape the quote
                backslashes++;
                str_resize(&args, &args_max, args_pos + backslashes);
                while (backslashes--) args[args_pos++] = '\\';
                backslashes = 0;
            } else {
                backslashes = 0;
            }

            str_resize(&args, &args_max, args_pos + 1);
            args[args_pos++] = *arg;

            arg++;
        }
        // Add extra backslashes to avoid escaping the final quote
        if (backslashes) {
            str_resize(&args, &args_max, args_pos + backslashes);
            while (backslashes--) args[args_pos++] = '\\';
        }
        str_resize(&args, &args_max, args_pos + 1);
        args[args_pos++] = '"';

        argv++;
    }

    args = realloc(args, sizeof(_TCHAR *) * (args_pos + 1));
    args[args_pos] = '\0';

    return args;
}
