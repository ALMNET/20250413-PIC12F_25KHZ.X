/*
 * File:   ADC_LIB.c
 * Author: arman
 *
 * Created on 12 de agosto de 2023, 22:02
 */

#include <stdint.h>
#include "..\inc\PWM_LIB.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ	4000000
#endif






/**
 * @brief   Sets up and initializes CCP1 Module as PWM
 *
 * @param   None
 * 
 * @return  None
 */
void pwm_setup(){
    
#if defined(_12F675)
    
#elif defined (_12F683)
    // Set up PWM
    TRISIObits.TRISIO0 = 0;      // Set GP0/CCP1 as output
    PR2 = 39;         // Set PR2 for 25kHz PWM
    CCP1CON = 0b00001100; // PWM mode
    CCPR1L = 20;      // 50% duty cycle initially (20/40)

    T2CON = 0b00000100;  // Timer2 ON, prescaler 1
#endif
}

/**
 * @brief   Sets PWM cycle from 0 to 100%
 *
 * @param   dutyCycle: Value from 0 to 100 to determine the pwm duty cycle out
 * 
 * @return  0 if everything went fine :).
 */
unsigned short pwm_set_duty_cycle(unsigned short dutyCycle)
{
    unsigned short result = 1; 
    // Decimal Max Value: 159
    // Bin Max Value: 0b 
    
    if(dutyCycle > 100)
        dutyCycle = 100;
    
    dutyCycle = (unsigned short) (159 * dutyCycle / 100);
    
    CCPR1L = (unsigned char) (dutyCycle >> 2);
    
    CCP1CONbits.DC1B1 = 1 & (dutyCycle >> 1);
    CCP1CONbits.DC1B0 = 1 & dutyCycle;
    
    result = 0;
    
    return result;
}

/**
 * @brief   Sets PWM cycle according button sequence:
 *          button sequence starts from 30% and do 10% increments til reach
 *          100%, then, it returns to its lower value: 30%
 *
 * @param   currentPwmState: Value passed from main function to avoid using
 *          global variables
 * 
 * @return  and updated version of currentPwmState (value incremented according
 *          sequence
 */
pwmDutyState_t set_pwm_cycle_according_button(pwmDutyState_t currentPwmState)
{
    switch(currentPwmState)
    {
        case PWM_30_DUTY: pwm_set_duty_cycle(30); currentPwmState = PWM_40_DUTY; break;

        case PWM_40_DUTY: pwm_set_duty_cycle(40); currentPwmState = PWM_50_DUTY; break;

        case PWM_50_DUTY: pwm_set_duty_cycle(50); currentPwmState = PWM_60_DUTY; break;

        case PWM_60_DUTY: pwm_set_duty_cycle(60); currentPwmState = PWM_70_DUTY; break;

        case PWM_70_DUTY: pwm_set_duty_cycle(70); currentPwmState = PWM_80_DUTY; break;

        case PWM_80_DUTY: pwm_set_duty_cycle(80); currentPwmState = PWM_90_DUTY; break;

        case PWM_90_DUTY: pwm_set_duty_cycle(90); currentPwmState = PWM_100_DUTY; break;

        case PWM_100_DUTY: pwm_set_duty_cycle(100); currentPwmState = PWM_30_DUTY; break;
    }
    
    return currentPwmState;
}