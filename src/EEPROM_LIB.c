/*
 * File:   ADC_LIB.c
 * Author: arman
 *
 * Created on 21 de Junio de 2025, 10:20
 */

#include <stdint.h>
#include "..\inc\EEPROM_LIB.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ	4000000
#endif


/**
 * @brief   Reads byte from eeprom according address byte passed as argument
 *
 * @param   address: EEPROM Address byte
 * 
 * @return  Read value
 */
unsigned char EEPROM_Read(unsigned char address) {
    while (WR);           // Wait for pending writings
    EEADR = address;      // EEPROM Address set
    EECON1bits.RD = 1;    // Read start
    return EEDATA;        // Return read data from read buffer
}

/**
 * @brief   Write byte to EEPROM in a given address
 *
 * @param   address: Address where data will be  written
 *          data: data to be written
 * 
 * @return  None
 */
void EEPROM_Write(unsigned char address, unsigned char data) {
    while (WR);           // Wait for pending writings
    EEADR = address;      // EEPROM Address set
    EEDATA = data;        // Data to write
    EECON1bits.WREN = 1;  // Enable EEPROM Write

    INTCONbits.GIE = 0;   // Global interrupts disable for security
    EECON2 = 0x55;        // Manufacturer suggested sequence for EEPROM Write
    EECON2 = 0xAA;
    EECON1bits.WR = 1;    // Writing start
    INTCONbits.GIE = 1;   // Re-enable interrupts

    while (WR);           // Waiting for EEPROM writing end
    EECON1bits.WREN = 0;  // disables EEPROM writing
}
