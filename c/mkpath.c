#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef _WIN32
#define PATH_DELIM "/\\"
#define mkdir(path, mode) mkdir(path)
#else
#define PATH_DELIM "/"
#define mkdir(path, mode) mkdir(path, mode)
#endif

static int _mkpath(char *path)
{
    char *p = strchr(path, '\0');
    while (p > path) if (strchr(PATH_DELIM, *--p)) break;
    if (p == path) return 0;
    char sep = *p;
    *p = '\0';
    if (mkdir(path, 0755) != -1 || errno == EEXIST) {
        *p = sep;
        return 0;
    }
    if (_mkpath(path) == -1) return -1;
    if (mkdir(path, 0755) == -1) return -1;
    *p = sep;
    return 0;
}

int mkpath(const char *path)
{
    char *_path = strdup(path);
    if (!_path) return -1;
    int rc = _mkpath(_path);
    free(_path);
    return rc;
}
