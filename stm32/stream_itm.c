// Redirects stdout to debugger

#include "main.h"

int _write(int fd, char *ptr, int len)
{
    for (int i = 0; i < len; i++) ITM_SendChar(ptr[i]);
    return len;
}
