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
#include "..\inc\EEPROM_LIB.h"

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
#define OUTPUT_PWM_DIR         TRISIObits.TRISIO5
#define OUTPUT_PWM             GPIObits.GPIO5

// TEMP SENSOR
#define BUTTON_PWM_INC_DIR  TRISIObits.TRISIO4
#define BUTTON_PWM_INC_AIO  ANSELbits.ANS3
#define BUTTON_PWM_INC      GPIObits.GPIO4

#elif defined (_12F683)
#define OUTPUT_PWM_DIR      TRISIObits.TRISIO2
#define OUTPUT_PWM          GPIObits.GP2

// PWM Increment Button
#define BUTTON_PWM_INC_DIR  TRISIObits.TRISIO0
#define BUTTON_PWM_INC_AIO  ANSELbits.ANS0
#define BUTTON_PWM_INC      GPIObits.GP0

// PWM Enable / Disable Button
#define BUTTON_PWM_ENA_DIR  TRISIObits.TRISIO1
#define BUTTON_PWM_ENA_AIO  ANSELbits.ANS1
#define BUTTON_PWM_ENA      GPIObits.GP1

#endif

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// GLOBAL VARIABLES /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    OUTPUT_ENABLED,
    OUTPUT_DISABLED
}pwmMode_t;

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// PROTOTYPES ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Delay / Timer prototype
void delay_ms(long delay_value);
void delay_us(double delay_value);

// Setup / initialization prototype
void setup(void);

unsigned char EEPROM_Read(unsigned char address);
void EEPROM_Write(unsigned char address, unsigned char data);

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
    OUTPUT_PWM_DIR = OUTPUT;
    OUTPUT_PWM = OFF;
    
    // PWM INCREMENT BUTTON CONFIGURATION
    BUTTON_PWM_INC_DIR = INPUT;
    BUTTON_PWM_INC_AIO = DIGITAL;
    
    // PWM ENABLE DISABLE BUTTON CONFIGURATION
    BUTTON_PWM_ENA_DIR = INPUT;
    BUTTON_PWM_ENA_AIO = DIGITAL;
    
    //ANSELbits.ANS = 0b0000; // All Digital
    
    // ADC_Init();
    
    pwm_setup();
    
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// MAIN FUNCTION //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void main(void) {    
    
    // Main local variable definitions
    uint8_t pulseWidth = 0;
    pwmMode_t PWM_MODE = OUTPUT_ENABLED;
            
            
    setup();
    
    // Set Initial PWM duty cycle according last used
    pulseWidth = EEPROM_Read(0x00);
    pwm_set_duty_cycle(pulseWidth);
    
    while(1){
        
        // Check if PWM increment button was pressed
        if(BUTTON_PWM_INC == HIGH && PWM_MODE == OUTPUT_ENABLED)
        {
            delay_ms(100);
            while(BUTTON_PWM_INC == HIGH);
            delay_ms(100);
            
            if(pulseWidth >= 100)
                pulseWidth = 30;
            else
                pulseWidth += 10;
            
            pwm_set_duty_cycle(pulseWidth);
            EEPROM_Write(0x00, pulseWidth);

        }
        
        // Check if PWM increment button was pressed
        if(BUTTON_PWM_ENA == HIGH)
        {
            delay_ms(100);
            while(BUTTON_PWM_ENA == HIGH);
            delay_ms(100);
            
            if(PWM_MODE == OUTPUT_ENABLED)
            {
                pwm_set_duty_cycle(0);      // 0%, simulates a disabled output
                //EEPROM_Write(0x00, pulseWidth);
                PWM_MODE = OUTPUT_DISABLED;
            }
            else
            {
                pwm_set_duty_cycle(pulseWidth);
                //EEPROM_Write(0x00, pulseWidth);
                PWM_MODE = OUTPUT_ENABLED;
            }
            

        }
        
    }
    
    return;
}

///////////////////////////////// AUX FUNCTIONS ////////////////////////////////

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


