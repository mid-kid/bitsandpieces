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
    size_t win_match = 0;
    // If it's a drive letter, make sure it matches
#define X(x) (x[0] >= 'A' && x[0] <= 'Z' && x[1] == ':' && x[2] == sep)
    if (X(dst_p) || X(cwd_p)) {
        win_match = 3;
    }
#undef X
    // If it's a network share, make sure it matches
#define X(x) (x[0] == sep && x[1] == sep)
    if (X(dst_p) || X(cwd_p)) {
        char *dst_net = strchr(dst_p + 2, sep);
        size_t dst_net_l = dst_net ? (size_t)(dst_net - dst_p) : strlen(dst_p);
        char *cwd_net = strchr(cwd_p + 2, sep);
        size_t cwd_net_l = cwd_net ? (size_t)(cwd_net - cwd_p) : strlen(cwd_p);
        win_match = dst_net_l > cwd_net_l ? dst_net_l : cwd_net_l;
    }
#undef X
    // If any of the above are detected, make sure it matches
    // Otherwise, return a copy of the string unmodified
    if (win_match) {
        if (strncmp(dst_p, cwd_p, win_match) == 0) {
            dst_p += win_match - 1;
            cwd_p += win_match - 1;
        } else {
            // Duplicate string and return
            size_t dst_p_len = strlen(dst_p) + 1;
            char *dst_m = malloc(dst_p_len);
            return dst_m ? memcpy(dst_m, dst_p, dst_p_len) : dst_m;
        }
    }
#endif

    // Strip any leading path components
    for (;;) {
        if (*dst_p != sep) break;
        char *c = strchr(dst_p + 1, sep);
        if (!c) break;
        size_t l = c - dst_p;
        if (strncmp(dst_p, cwd_p, l) != 0) break;
        dst_p += l; cwd_p += l;
    }

    // Strip the final path component if it's an exact match
    if (*cwd_p == sep) {
        char *c = strchr(cwd_p + 1, sep);
        if (!c) c = strchr(cwd_p + 1, '\0');
        size_t l = c - cwd_p;
        if (strncmp(dst_p, cwd_p, l) == 0 && dst_p[l] == '\0') {
            dst_p += l; cwd_p += l;
        }
    }

    // Figure out how many ../ to add
    int p = 0;
    if (strcmp(dst_p, cwd_p) == 0) {
        dst_p = ".";
    } else if (*dst_p == sep) {
        dst_p++;
    }
    for (; (cwd_p = strchr(cwd_p, sep)); cwd_p++) p++;

    // Allocate and build final string
    size_t dst_p_len = strlen(dst_p);
    if (dst_p_len) dst_p_len++;
    char *dst_m = malloc(3 * p + dst_p_len);
    if (!dst_m) return NULL;
    for (int i = 0; i < p; i++) memcpy(dst_m + 3 * i, sep_parent, 3);
    if (!dst_p_len) dst_m[3 * p - 1] = '\0';
    memcpy(dst_m + 3 * p, dst_p, dst_p_len);

    return dst_m;
}
