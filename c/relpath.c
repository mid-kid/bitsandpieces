#include <stdlib.h>
#include <string.h>

char *relpath(const char *cwd, const char *dst)
{
    const char *dst_p = dst;
    const char *cwd_p = cwd;

    // Strip any leading path components
    for (;;) {
        if (*dst_p != '/') break;
        char *c = strchr(dst_p + 1, '/');
        if (!c) break;
        int l = c - dst_p;
        if (strncmp(dst_p, cwd_p, l) != 0) break;
        dst_p += l; cwd_p += l;
    }

    // Figure out how many ../ to add
    int l = 0;
    if (strcmp(dst_p, cwd_p) == 0) {
        dst_p = ".";
    } else if (*dst_p == '/') {
        dst_p++;
        while (*cwd_p) if (*cwd_p++ == '/') l++;
    }

    // Allocate and build final string
    int dst_p_len = strlen(dst_p) + 1;
    char *dst_m = malloc(3 * l + dst_p_len);
    if (!dst_m) return NULL;
    for (int i = 0; i < l; i++) memcpy(dst_m + 3 * i, "../", 3);
    memcpy(dst_m + 3 * l, dst_p, dst_p_len);

    return dst_m;
}
