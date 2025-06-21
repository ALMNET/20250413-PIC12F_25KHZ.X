
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EEPROM_LIB_H
#define	EEPROM_LIB_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <builtins.h>

#if defined(_12F675)
#include <pic12f675.h>
#else
#include <pic12f683.h>
#endif

unsigned char EEPROM_Read(unsigned char address);
void EEPROM_Write(unsigned char address, unsigned char data);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 
    


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* EEPROM_LIB_H */

