#include "motor_servo.h"

#include <stdint.h>

#include "pins.h"
#include "config.h"

/*
 * Library to drive two servo motors.
 *
 * Used config:
 * - MOTOR_S0_ENABLE: Enable servo 0
 * - MOTOR_S1_ENABLE: Enable serve 1
 * - OPTION_ASM_CCP_ISR: Use the assembly version of the interrupt service routines
 *
 * Used pins:
 * - PIN_S0: PWM signal for servo 0
 * - PIN_S1: PWM signal for servo 1
 *
 * Used interrupts:
 * - CCP1: To send a precisely-timed PWM signal to servo 0
 * - CCP2: To send a precisely-timed PWM signal to servo 1
 *
 * Used timers:
 * - Timer1: To compare CCP1 and CCP2 with.
 */

#ifdef MOTOR_S0_ENABLE
static volatile uint16_t servo0_time = 0;
#endif
#ifdef MOTOR_S1_ENABLE
static volatile uint16_t servo1_time = 0;
#endif

void motor_servo_set(const enum motor_servo motor, uint16_t cycles)
{
    switch (motor) {
#ifdef MOTOR_S0_ENABLE
    case MOTOR_S0:
        CCP1IE = 0;
        servo0_time = cycles;
        CCP1IE = 1;
        break;
#endif

#ifdef MOTOR_S1_ENABLE
    case MOTOR_S1:
        CCP2IE = 0;
        servo1_time = cycles;
        CCP2IE = 1;
        break;
#endif
    }
}

void motor_servo_init(void)
{
#ifdef MOTOR_S0_ENABLE
    TRISD &= ~_PIN_S0;
    PIN_S0 = 0;
#endif
#ifdef MOTOR_S1_ENABLE
    TRISD &= ~_PIN_S1;
    PIN_S1 = 0;
#endif

    // Reset timer1
    T1CON = TMR1L = TMR1H = 0;

    // Reset the compare modules
    CCP1IE = CCP2IE = 0;
    CCP1CON = CCP2CON = 10;

    // Reset the servos
#ifdef MOTOR_S0_ENABLE
    servo0_time = MOTOR_S0_DEFAULT;
    CCPR1L = SERVO_PULSE_WIDTH & 0xFF;
    CCPR1H = (SERVO_PULSE_WIDTH >> 8) & 0xFF;
    CCP1IE = 1;
#endif
#ifdef MOTOR_S1_ENABLE
    servo1_time = MOTOR_S1_DEFAULT;
    CCPR2L = SERVO_PULSE_WIDTH & 0xFF;
    CCPR2H = (SERVO_PULSE_WIDTH >> 8) & 0xFF;
    CCP2IE = 1;
#endif

    // Enable timer1, don't divide the clock
    T1CON = _TMR1ON;

    // Enable interrupts
    GIE = PEIE = 1;
}

// SDCC doesn't support 16-bit addition on hardware registers.
// As such, it generates a bunch of bogus code when using workarounds.
// (Or at least, I haven't been able to properly do a 16-bit CCPR1 += value)

// Sorry for the blatant copy-pastes that these two functions are,
//  and for all the hardcoded things.

#ifdef MOTOR_S0_ENABLE
void ccp1_isr(void)
{
#ifdef OPTION_ASM_CCP_ISR
    __asm__(
        "\tbanksel PORTD\n"
        "\tbtfsc " ASM_PIN_S0 "\n"
        "\tgoto _ccp1_isr_low\n"

        "\tbsf " ASM_PIN_S0 "\n"

        // Add low byte
        "\tbanksel _servo0_time\n"
        "\tmovf _servo0_time, w\n"
        "\tbanksel CCPR1L\n"
        "\taddwf CCPR1L, f\n"
        "\tbtfsc STATUS, C\n"
        "\tincf CCPR1H, f\n"

        // Add high byte
        "\tbanksel _servo0_time\n"
        "\tmovf (_servo0_time + 1), w\n"
        "\tbanksel CCPR1H\n"
        "\taddwf CCPR1H, f\n"

        "\treturn\n"

    "_ccp1_isr_low:\n"

        "\tbcf " ASM_PIN_S0 "\n"

        // Substract the lower byte
        "\tbanksel _servo0_time\n"
        "\tmovf _servo0_time, w\n"
        "\tbanksel CCPR1L\n"
        "\tsublw " ASM_SERVO_PULSE_WIDTH_LOW "\n"

        // If it carries, decrement the high byte of the result
        "\tbtfss STATUS, C\n"
        "\tdecf CCPR1H, f\n"

        // Add the lower byte to the result
        "\taddwf CCPR1L, f\n"
        "\tbtfsc STATUS, C\n"
        "\tincf CCPR1H, f\n"

        // Substract the high byte and add it to the result
        "\tbanksel _servo0_time\n"
        "\tmovf (_servo0_time + 1), w\n"
        "\tbanksel CCPR1H\n"
        "\tsublw " ASM_SERVO_PULSE_WIDTH_HIGH "\n"
        "\taddwf CCPR1H, f\n"
    );
#else
    if (!PIN_S0) {
        uint16_t temp;
        PIN_S0 = 1;

        //CCPR1 += servo0_time;
        temp = (CCPR1L | CCPR1H << 8) + servo0_time;
        CCPR1L = temp & 0xFF;
        CCPR1H = (temp >> 8) & 0xFF;
    } else {
        uint16_t temp;
        PIN_S0 = 0;

        //CCPR1 += (SERVO_PULSE_WIDTH - servo0_time);
        temp = (CCPR1L | CCPR1H << 8) + (SERVO_PULSE_WIDTH - servo0_time);
        CCPR1L = temp & 0xFF;
        CCPR1H = (temp >> 8) & 0xFF;
    }
#endif
}
#endif

#ifdef MOTOR_S1_ENABLE
void ccp2_isr(void)
{
#ifdef OPTION_ASM_CCP_ISR
    __asm__(
        "\tbanksel PORTD\n"
        "\tbtfsc " ASM_PIN_S1 "\n"
        "\tgoto _ccp2_isr_low\n"

        "\tbsf " ASM_PIN_S1 "\n"

        // Add low byte
        "\tbanksel _servo1_time\n"
        "\tmovf _servo1_time, w\n"
        "\tbanksel CCPR2L\n"
        "\taddwf CCPR2L, f\n"
        "\tbtfsc STATUS, C\n"
        "\tincf CCPR2H, f\n"

        // Add high byte
        "\tbanksel _servo1_time\n"
        "\tmovf (_servo1_time + 1), w\n"
        "\tbanksel CCPR2H\n"
        "\taddwf CCPR2H, f\n"

        "\treturn\n"

    "_ccp2_isr_low:\n"

        "\tbcf " ASM_PIN_S1 "\n"

        // Substract the lower byte
        "\tbanksel _servo1_time\n"
        "\tmovf _servo1_time, w\n"
        "\tbanksel CCPR2L\n"
        "\tsublw " ASM_SERVO_PULSE_WIDTH_LOW "\n"

        // If it carries, decrement the high byte of the result
        "\tbtfss STATUS, C\n"
        "\tdecf CCPR2H, f\n"

        // Add the lower byte to the result
        "\taddwf CCPR2L, f\n"
        "\tbtfsc STATUS, C\n"
        "\tincf CCPR2H, f\n"

        // Substract the high byte and add it to the result
        "\tbanksel _servo1_time\n"
        "\tmovf (_servo1_time + 1), w\n"
        "\tbanksel CCPR2H\n"
        "\tsublw " ASM_SERVO_PULSE_WIDTH_HIGH "\n"
        "\taddwf CCPR2H, f\n"
    );
#else
    if (!PIN_S1) {
        uint16_t temp;
        PIN_S1 = 1;

        //CCPR2 += servo1_time;
        temp = (CCPR2L | CCPR2H << 8) + servo1_time;
        CCPR2L = temp & 0xFF;
        CCPR2H = (temp >> 8) & 0xFF;
    } else {
        uint16_t temp;
        PIN_S1 = 0;

        //CCPR2 += (SERVO_PULSE_WIDTH - servo1_time);
        temp = (CCPR2L | CCPR2H << 8) + (SERVO_PULSE_WIDTH - servo1_time);
        CCPR2L = temp & 0xFF;
        CCPR2H = (temp >> 8) & 0xFF;
    }
#endif
}
#endif
