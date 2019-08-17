#include "timers.h"

#include <pic14regs.h>

void timers_init(void)
{
    // Set up Timer0
    T0IE = 0;  // Disable interrupt
    T0CS = 0;  // Use internal clock
    PSA = 0;   // Assign prescaler to Timer0
    PS0 = 1;   // Prescale 1:256
    PS1 = 1;
    PS2 = 1;
    TMR0 = 0;  // Reset timer
    T0IF = 0;  // Enable interrupt
    T0IE = 1;

    // Set up Timer1
    T1CON = 3 << 4 | _TMR1ON;  // Prescale 1:8
    TMR1L = TMR1H = 0;  // Reset timer
}
