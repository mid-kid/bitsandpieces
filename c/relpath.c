#include <stdlib.h>
#include <string.h>

char *relpath(const char *cwd, const char *dst)
{
    const char *dst_p = dst;
    const char *cwd_p = cwd;

#ifndef _WIN32
    static const char sep = '/';
    static const char *sep_parent = "../";
#else
    static const char sep = '\\';
    static const char *sep_parent = "..\\";
    // Strip drive letter if it matches
#define X(x) (x[0] >= 'A' && x[0] <= 'Z' && x[1] == ':' && x[2] == sep)
    if (X(dst_p) && X(cwd_p) && strncmp(dst_p, cwd_p, 3) == 0) {
        dst_p += 2;
        cwd_p += 2;
    }
#undef X
#endif

    // Strip any leading path components
    for (;;) {
        if (*dst_p != sep) break;
        char *c = strchr(dst_p + 1, sep);
        if (!c) break;
        int l = c - dst_p;
        if (strncmp(dst_p, cwd_p, l) != 0) break;
        dst_p += l; cwd_p += l;
    }

    // Figure out how many ../ to add
    int l = 0;
    if (strcmp(dst_p, cwd_p) == 0) {
        dst_p = ".";
    } else if (*dst_p == sep) {
        dst_p++; l++;
        while ((cwd_p = strchr(cwd_p + 1, sep))) l++;
    }

    // Allocate and build final string
    int dst_p_len = strlen(dst_p) + 1;
    char *dst_m = malloc(3 * l + dst_p_len);
    if (!dst_m) return NULL;
    for (int i = 0; i < l; i++) memcpy(dst_m + 3 * i, sep_parent, 3);
    memcpy(dst_m + 3 * l, dst_p, dst_p_len);

    return dst_m;
}
