#pragma once

enum motor_dc {
    MOTOR_DC0,
    MOTOR_DC1
};

enum motor_dc_dir {
    MOTOR_DC_CW,
    MOTOR_DC_CCW
};

void motor_dc_rotate(const enum motor_dc motor, const enum motor_dc_dir direction);
void motor_dc_stop(const enum motor_dc motor);
void motor_dc_init(void);
