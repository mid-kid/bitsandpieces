#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "relpath.h"

int main(int argc, char *argv[])
{
    if (argc <= 1) return EXIT_FAILURE;

    char *pth = realpath(argv[1], NULL);
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
