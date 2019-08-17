#pragma once

#include <stdint.h>

// Clock rate: 8 MHz
#define F_CPU 8000000
// NOTE: If changing this, fix delay.asm

// Timers
#define T0_PRESC 256
#define T0_FREQ (F_CPU / 4)
#define T0_MAXVAL (1 << 8)
#define T1_PRESC 8
#define T1_FREQ (F_CPU / 4)
#define T1_MAXVAL (1 << 16)
// NOTE: If changing this, fix timers.c

// Convert microseconds to instruction or timer cycles
#define cycles(us) ((F_CPU / 4) * (long)(us) / 1000000)
#define cycles_t0(us) (uint8_t)(T0_FREQ * (long)(us) / ((long)T0_PRESC * 1000000))
#define cycles_t1(us) (uint16_t)(T1_FREQ * (long)(us) / ((long)T1_PRESC * 1000000))

// Convert microseconds to timer overflows
#define overflows_t0(us) (uint8_t)((T0_FREQ / T0_PRESC / T0_MAXVAL) * (long)(us) / 1000000)
#define overflows_t1(us) (uint16_t)((T1_FREQ / T1_PRESC / T1_MAXVAL) * (long)(us) / 1000000)

// Interrupts
//#define INT_SSP
//#define INT_TX
//#define INT_RC
//#define INT_TMR2
//#define INT_TMR1
//#define INT_C1
//#define INT_C2
//#define INT_AD
//#define INT_EE
//#define INT_OSF
//#define INT_CCP1
//#define INT_T0
//#define INT_RAB
