#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "relpath.h"

int test_manual(char *dir)
{
#ifndef _WIN32
    char *pth = realpath(dir, NULL);
#else
    char *pth = _fullpath(NULL, dir, 0);
#endif
    if (!pth) { perror("realpath"); return EXIT_FAILURE; }
    char *cwd = getcwd(NULL, 0);
    if (!cwd) { perror("getcwd"); return EXIT_FAILURE; }
    printf("pth: %s\n", pth);
    printf("cwd: %s\n", cwd);

    char *pth_rel = relpath(cwd, pth);

    printf("pth_rel: %s\n", pth_rel);

    free(pth_rel);
    free(cwd);
    free(pth);

    return EXIT_SUCCESS;
}

void test_pth(char *cwd, char *pth)
{
    char *pth_rel = relpath(cwd, pth);
    printf("%s -> %s = %s\n", cwd, pth, pth_rel);
    free(pth_rel);
}

int test(void)
{
    test_pth("/a/b", "/");  // ../..
    test_pth("/a/b", "/a");  // ..
    test_pth("/a/b", "/a/b/c");  // c
    test_pth("/a/b", "/a/b/c/d");  // c/d
    test_pth("/a/b", "");  // .
    test_pth("/a/b", "a");  // a
    test_pth("/a/b", "a/b/c");  // a/b/c
    test_pth("/a/b", "a/b/c/d");  // a/b/c/d
    test_pth("a/b", "");  // .
    test_pth("a/b", "a");  // a
    test_pth("a/b", "a/b/c");  // a/b/c
    test_pth("a/b", "a/b/c/d");  // a/b/c/d

    test_pth("/a/b", "/a/b");  // .
    test_pth("/a/b", "a/b");  // a/b
    test_pth("a/b", "a/b");  // a/b

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        return test_manual(argv[1]);
    }

    return test();
}
