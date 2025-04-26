/*
 * File:   ADC_LIB.c
 * Author: arman
 *
 * Created on 12 de agosto de 2023, 22:02
 */


#include <xc.h>
#include "..\inc\ADC_LIB.h"


#ifndef _XTAL_FREQ
#define _XTAL_FREQ	4000000
#endif

/**
 * @brief   Initializates ADC Internal Module of microcontroller
 *
 * @param   none
 * 
 * @return  none.
 */
void ADC_Init(void){
    ADCON0bits.ADFM = 1;        // Right Justified
    
    ADCON0bits.VCFG = 0;        // VREF Pin
    
    ANSELbits.ADCS = 0b011;     // (clock derived from a dedicated internal 
                                // oscillator = 500 kHz max)
    
    ANSELbits.ANS = 0b0001; // Channel AN0 as Analog (Fixed channel)
            
    ADCON0bits.GO_DONE = 0;
    
    ADCON0bits.ADON = 1;        // ADC Module ON
}

/**
 * @brief   Gets ADC raw value according ADC ADRESS L and H
 *
 * @param   none
 * 
 * @return  Value obtained from conversion from 0 (0v) to 1023 (5v) 
 */
unsigned int ADC_raw_read(void){
    
    unsigned int ADCConv = 0;   // Temporal variable
    
    // Fixed Channel 0 select
    ADCON0bits.CHS = 0b0000;
    
    PIR1bits.ADIF = 0;          // Clears ADC interruption Flag (ADIF)
    __delay_us(20);             // 20mS Data Adquisition Time
    ADCON0bits.GO_nDONE = 1;    // Wait until conversion is done
    while(PIR1bits.ADIF == 0);  // Wait until conversion is done 
    
    // Conversion done
    // Save ADRESL and ADRESH Values and combines it in a single value
    // Keep in mind that ADRESL is a 8 bit register (LSB) and ADRESH is a 2 bit
    // register (MSB) from the ADC
    ADCConv = (unsigned int) (ADRESL + (ADRESH << 8));
    
    // Return ADC_CONV Value
    return ADCConv;
}