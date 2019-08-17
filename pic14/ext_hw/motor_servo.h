#pragma once

#include <stdint.h>

enum motor_servo {
    MOTOR_S0,
    MOTOR_S1
};

void motor_servo_set(const enum motor_servo motor, uint16_t cycles);
void motor_servo_init(void);
