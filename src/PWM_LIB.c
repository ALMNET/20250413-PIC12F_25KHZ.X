/*
 * File:   ADC_LIB.c
 * Author: arman
 *
 * Created on 26 de Abril de 2025, 09:43
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
    
//#if defined(_12F675)
    
//#if defined (_12F683)
    // Set up PWM
    TRISIObits.TRISIO0 = 0;      // Set GP0/CCP1 as output
    PR2 = 39;         // Set PR2 for 25kHz PWM
    CCP1CON = 0b00001100; // PWM mode
    // CCPR1L = 20;      // 50% duty cycle initially (20/40)
    
    pwm_set_duty_cycle(30);

    T2CON = 0b00000100;  // Timer2 ON, prescaler 1
//#endif
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

