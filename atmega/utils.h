#pragma once

#include <avr/io.h>

#define cycles(us) (F_CPU * (long long)(us) / 1000000)

#define cbi(sfr, bit) (sfr &= ~_BV(bit))
#define sbi(sfr, bit) (sfr |= _BV(bit))

// Stupid definitions for handling pins.

#define _pin(port, pin) PORT ## port ## pin
#define _port(port, pin) PORT ## port
#define pin(pin) _BV(_pin(pin))
#define port(pin) _port(pin)

#define LOW 0
#define HIGH 1
#define OUT 1
#define IN 0

#define _pinsetup(port, pin, x) if (x) { sbi(DDR ## port, DD ## port ## pin); } else { cbi(DDR ## port, DD ## port ## pin); }
#define pinsetup(pin, x) _pinsetup(pin, x)

#define _writepin(port, pin, x) if (x) { sbi(PORT ## port, PORT ## port ## pin); } else { cbi(PORT ## port, PORT ## port ## pin); }
#define writepin(pin, x) _writepin(pin, x)

#define _readpin(port, pin) ((PIN ## port >> PORT ## port ## pin) & 1)
#define readpin(pin) _readpin(pin)
