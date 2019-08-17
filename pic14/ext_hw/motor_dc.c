#include "motor_dc.h"

#include "pins.h"

/*
 * Small, dumb library to control multiple DC motors.
 *
 * Used pins:
 * - PIN_DC0R1: Motor 0 pin 1
 * - PIN_DC0R2: Motor 0 pin 2
 * - PIN_DC1R1: Motor 1 pin 1
 * - PIN_DC1R2: Motor 1 pin 2
 */

void motor_dc_rotate(const enum motor_dc motor, const enum motor_dc_dir direction)
{
    motor_dc_stop(motor);
    __asm__ ("nop");

    switch (motor) {
    case MOTOR_DC0:
        PIN_DC0R1 = (direction == MOTOR_DC_CW) ? 0 : 1;
        PIN_DC0R2 = (direction == MOTOR_DC_CW) ? 1 : 0;
        break;

    case MOTOR_DC1:
        PIN_DC1R1 = (direction == MOTOR_DC_CW) ? 0 : 1;
        PIN_DC1R2 = (direction == MOTOR_DC_CW) ? 1 : 0;
        break;
    }
}

void motor_dc_stop(const enum motor_dc motor)
{
    switch (motor) {
    case MOTOR_DC0:
        PORTC |= _PIN_DC0R1 | _PIN_DC0R2;
        break;

    case MOTOR_DC1:
        PORTC |= _PIN_DC1R1 | _PIN_DC1R2;
        break;
    }
}

void motor_dc_init(void)
{
    // Set the correct pins as outputs
    TRISC &= ~_PIN_DC0R1 & ~_PIN_DC0R2 & ~_PIN_DC1R1 & ~_PIN_DC1R2;
    PORTC |= _PIN_DC0R1 | _PIN_DC0R2 | _PIN_DC1R1 | _PIN_DC1R2;

    motor_dc_stop(MOTOR_DC0);
    motor_dc_stop(MOTOR_DC1);
}
