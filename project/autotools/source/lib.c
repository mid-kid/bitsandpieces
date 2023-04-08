#include "lib.h"

#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include <autoconf.h>
#endif

void lib(void)
{
    puts(PACKAGE_STRING);
}
