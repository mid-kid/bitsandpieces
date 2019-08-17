#include "counter.h"

#include <stdint.h>

#include "pins.h"

/*
 * Small library to drive two 7-segment displays through SPI.
 *
 * Used pins:
 * - PIN_CSER: Serial data
 * - PIN_CSH: Serial clock
 * - PIN_CST: Serial start
 * - PIN_CMR: Serial master reset (clears out the registers)
 */

static char font[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

static void counter_shiftout(char data)
{
    for (char i = 8; i; i--) {
        PIN_CSER = (data & 0x80) ? 1 : 0;
        PIN_CSH = 1;
        __asm__ ("nop");
        PIN_CSH = 0;
        data <<= 1;
    }
    PIN_CSER = 0;
}

void counter_writenum(const char number)
{
    char left = font[(number & 0x7F) / 10];
    char right = font[(number & 0x7F) % 10];

    if ((number & 0x80) != 0) {
        left |= 0x80;
        right |= 0x80;
    }

    counter_shiftout(right);
    counter_shiftout(left);

    PIN_CST = 1;
    __asm__ ("nop");
    PIN_CST = 0;
}

void counter_reset(void)
{
    PIN_CMR = 0;
    PIN_CST = 1;
    __asm__ ("nop");
    PIN_CST = 0;
    PIN_CMR = 1;
}

void counter_init(void)
{
    TRISD &= ~_PIN_CSER & ~_PIN_CSH & ~_PIN_CST & ~_PIN_CMR;
    PORTD &= ~_PIN_CSER & ~_PIN_CSH;

    counter_reset();
}
