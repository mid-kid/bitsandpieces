#pragma once

#include <pic14regs.h>

/*
 * RA0: Input pin       - Digital IN
 * RA1: Output pin      - Digital OUT
 */

/*
 * This file is generated with GNU m4. Source is pins.m4.
 */
dnl
define(`pin',
`#define PIN_$1 R$2$3
#define _PIN_$1 _R$2$3
#define ASM_PIN_$1 "PORT$2, $3"')dnl

pin(`PININ', `A', `0')
pin(`PINOUT', `A', `0')
