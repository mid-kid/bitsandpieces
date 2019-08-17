#include "ultra.h"

#include <stdint.h>
#include <pic14regs.h>

#include "config.h"
#include "pins.h"

/*
 * Library to read out two ultrasonic sensors, alternating them.
 *
 * Used config:
 * - ULTRA0_ENABLE: Enable ultra 0
 * - ULTRA1_ENABLE: Enable ultra 1
 *
 * Used pins:
 * - U0TRIG: Ultra 0 trigger
 * - U0ECHO: Ultra 0 echo
 * - U1TRIG: Ultra 1 trigger
 * - U1ECHO: Ultra 1 echo
 *
 * Used interrupts:
 * - RAB: To detect changes to the echo pins
 * - T0: To send the next trigger signal at a fixed interval.
 *
 * Used timers:
 * - Timer0: To send the next trigger signal at a fixed interval
 * - Timer1: To calculate the difference in time between the trigger and echo signals
 */

// Inaccurate as all heck, but it doesn't matter in this case.
// Just make sure it's at least 60ms, as per the datasheet.
#define ULTRA_TIMER overflows_t0(80000)

enum ultra_status {
    ULTRA_STATUS_STOP,
    ULTRA_STATUS_LOW,
    ULTRA_STATUS_HIGH
};

#ifdef ULTRA0_ENABLE
volatile uint16_t ultra0_value;
#endif
#ifdef ULTRA1_ENABLE
volatile uint16_t ultra1_value;
#endif
volatile uint16_t ultra_start;
volatile enum ultra ultra_current;
volatile enum ultra_status ultra_status;
volatile uint8_t ultra_timer;

void ultra_init(void)
{
    // Make sure nothing interrupts while we set up
    RABIE = 0;

    // Set up the pins
    TRISC &= ~_PIN_U0TRIG & ~_PIN_U1TRIG;
    TRISA |= _PIN_U0ECHO | _PIN_U1ECHO;

    // Ensure everything is clear
#ifdef ULTRA0_ENABLE
    ultra0_value = 0;
#endif
#ifdef ULTRA1_ENABLE
    ultra1_value = 0;
#endif
    ultra_start = 0;
    ultra_current = ULTRA_NONE;
    ultra_status = ULTRA_STATUS_STOP;
    ultra_timer = 0;

    // Enable the interrupt for PIN_U0ECHO and PIN_U1ECHO
#ifdef ULTRA0_ENABLE
    IOCA2 = 1;
#endif
#ifdef ULTRA1_ENABLE
    IOCA3 = 1;
#endif
    RABIE = 1;
}

void ultra_pulse(const enum ultra ultra)
{
    // Send a trigger signal to either of the ultrasonic sensors,
    //   and start waiting for the echo signal to come back.

    switch ((ultra_current = ultra)) {
#ifdef ULTRA0_ENABLE
    case ULTRA0:
        PIN_U0TRIG = 1;
        for (char i = 10; i != 0; i--);
        PIN_U0TRIG = 0;
        break;
#endif

#ifdef ULTRA1_ENABLE
    case ULTRA1:
        PIN_U1TRIG = 1;
        for (char i = 10; i != 0; i--);
        PIN_U1TRIG = 0;
        break;
#endif
    }

    ultra_status = ULTRA_STATUS_LOW;
}

char ultra_done(void)
{
    // Check if a signal is still being processed.

    if (ultra_current == ULTRA_NONE) return 1;
    if (ultra_status == ULTRA_STATUS_STOP) return 1;
    return 0;
}

void isr_rab(void)
{
    // Interrupt when either of the echo pins changes its state
    // Depending on what pin changed, what we were waiting on,
    //   and how it changed, we'll do something else.

    uint16_t timer;
    uint16_t time;

    if (ultra_done()) return;

    timer = (TMR1H << 8) | TMR1L;

    switch (ultra_status) {
    case ULTRA_STATUS_LOW:
#ifdef ULTRA0_ENABLE
        if (ultra_current == ULTRA0) if (!PIN_U0ECHO) break;
#endif
#ifdef ULTRA1_ENABLE
        if (ultra_current == ULTRA1) if (!PIN_U1ECHO) break;
#endif
        ultra_start = timer;
        ultra_status = ULTRA_STATUS_HIGH;
        break;

    case ULTRA_STATUS_HIGH:
#ifdef ULTRA0_ENABLE
        if (ultra_current == ULTRA0) if (PIN_U0ECHO) break;
#endif
#ifdef ULTRA1_ENABLE
        if (ultra_current == ULTRA1) if (PIN_U1ECHO) break;
#endif

        time = timer - ultra_start;
#ifdef ULTRA0_ENABLE
        if (ultra_current == ULTRA0) ultra0_value = time;
#endif
#ifdef ULTRA1_ENABLE
        if (ultra_current == ULTRA1) ultra1_value = time;
#endif
        ultra_status = ULTRA_STATUS_STOP;
        break;
    }
}

void isr_t0(void)
{
    // Waits at least TIMER_ULTRA * T0_OVFTIME cycles.
    // Alternates between the two ultrasonic sensors,
    //   such that they will never conflict.

    if (ultra_timer != 0) {
        ultra_timer--;
        return;
    }

    switch (ultra_current) {
#ifdef ULTRA0_ENABLE
    case ULTRA0:
#ifdef ULTRA1_ENABLE
        ultra_pulse(ULTRA1);
#else
        ultra_pulse(ULTRA0);
#endif
        break;
#endif

#ifdef ULTRA1_ENABLE
    case ULTRA1:
#ifdef ULTRA0_ENABLE
        ultra_pulse(ULTRA0);
#else
        ultra_pulse(ULTRA1);
#endif
        break;
#endif
    default:
#if defined(ULTRA0_ENABLE)
        ultra_pulse(ULTRA0);
#elif defined(ULTRA1_ENABLE)
        ultra_pulse(ULTRA1);
#endif
        break;
    }

    ultra_timer = ULTRA_TIMER;
}
