/**
  @File Name
    main.c
 
  @File Author
    Armando Almarales (ALMNET)
 
  @Creation Date
    2025-04-15 10:59

  @Summary
    This is the generated main.c using PIC12F MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  12F 0.01
        Device            :  PIC12F675 / PIC12F873
    The generated drivers are tested against the following:
        Compiler          :  XC8 v3.00
        MPLAB 	          :  MPLAB X v6.25
*/

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CLOCK DEFINITION /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#ifndef _XTAL_FREQ
#define _XTAL_FREQ	4000000
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// INCLUDES ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <xc.h>
#include <stdbool.h>
#include <builtins.h>
#include "..\inc\PWM_LIB.h"

#if defined(_12F675)    // Retro Compatibility for PIC12F675
#include <pic12f675.h>
#else
#include <pic12f683.h>
#endif

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// FUSES CONFIG /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#if defined(_12F675)    // Retro Compatibility for PIC12F675
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
// #else // Defined 12F583
#elif defined (_12F683)
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#endif

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// GPIO DEFINITIONS /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// GENERAL
#define ON              1
#define OFF             0

#define HIGH            1
#define LOW             0

#define INPUT           1
#define OUTPUT          0

#define ANALOG          1
#define DIGITAL         0

#define MAIN_FREQ       250	// Frequency in hz

#if defined(_12F675)    // Retro Compatibility for PIC12F675

// output MOTOR
#define output_DIR         TRISIObits.TRISIO5
#define output             GPIObits.GPIO5

// TEMP SENSOR
#define button_DIR  TRISIObits.TRISIO4
#define button_PIN  ANSELbits.ANS3
#define button      GPIObits.GPIO4

#elif defined (_12F683)
#define output_DIR                 TRISIObits.TRISIO2
#define output                     GPIObits.GP2

// TEMP SENSOR (LM35)
#define button_DIR     TRISIObits.TRISIO0
#define button_PIN     ANSELbits.ANS0
#define button         GPIObits.GP0

#endif


////////////////////////////////////////////////////////////////////////////////
///////////////////////////// GLOBAL VARIABLES /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// PROTOTYPES ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//// ADC Prototypes
//void ADC_Init(void);
//unsigned int ADC_raw_read(void);

//// Prototypes for PWM and fan control
//void PWM_output(uint8_t dutyCycle);
//void duty_cycle_according_temperature(unsigned int temperature);

// Delay / Timer prototype
void delay_ms(long delay_value);
void delay_us(double delay_value);

// Setup / initialization prototype
void setup(void);


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// SETUP //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup(void)
{
    
#if defined(_12F675)
    CMCON = 7; 
#elif defined (_12F683)
    CMCON0 = 7;
#endif
            
    // output CONFIG
    output_DIR = OUTPUT;
    output = OFF;
    
    // LM35 TEMP SENSOR CONFIG
    button_DIR = INPUT;
    button_PIN = DIGITAL;
    
    ANSELbits.ANS = 0b0000; // All Digital
    
    // ADC_Init();
    
    pwm_setup();
    
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// MAIN FUNCTION //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void main(void) {    
    pwmDutyState_t pwmDutyState = PWM_30_DUTY; // Default Value
        
    setup();
    
    while(1){
        if(button == HIGH)
        {
            delay_ms(100);
            while(button == HIGH);
            delay_ms(100);

            pwmDutyState = set_pwm_cycle_according_button(pwmDutyState);

        }
    }
    
    return;
}

///////////////////////////////// AUX FUNCTIONS ////////////////////////////////

/**
 * @brief   Sets PWM cycle on fan according read temperature
 *
 * @param   temperature: Temperature ref value (Used to be the one which was
 *                       read from the temperature sensor)
 * 
 * @return  none.
 */

void delay_ms(long delay_value){
    CLRWDT();
    long x;
    for(x=0; x < delay_value; x++) __delay_ms(1);
}


void delay_us(double delay_value){
    while((bool)(delay_value)){
        delay_value = delay_value - 1;
        __delay_ms(1);
    }
}