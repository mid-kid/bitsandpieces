#include <stdio.h>
#include <avr/interrupt.h>
#include "serial.h"

int main()
{
    serial_init(9600);
    sei();

    printf("Hello World!\r\n");
}

void exit(__attribute__((unused)) int status)
{
    serial_drain();
    for (;;);
}
