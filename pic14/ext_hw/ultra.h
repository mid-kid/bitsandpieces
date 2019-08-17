#pragma once

#include <stdint.h>

#include "config.h"

// Convert milimeter to timer1 cycles at the speed of sound
#define ultra_distance(mm) (uint16_t)((T1_FREQ * (long)(mm)) / ((long)T1_PRESC * 1000 * (340 / 2)))

enum ultra {
    ULTRA_NONE,
#ifdef ULTRA0_ENABLE
    ULTRA0,
#endif
#ifdef ULTRA1_ENABLE
    ULTRA1,
#endif
};

#ifdef ULTRA0_ENABLE
extern volatile uint16_t ultra0_value;
#endif
#ifdef ULTRA1_ENABLE
extern volatile uint16_t ultra1_value;
#endif
extern volatile enum ultra ultra_current;

void ultra_init(void);
void ultra_pulse(const enum ultra ultra);
char ultra_done(void);
