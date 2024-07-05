#include "mkpath.h"

int main(int argc, char *argv[]) {
    if (argc <= 1) return 1;
    return mkpath(argv[1]) == -1;
}
