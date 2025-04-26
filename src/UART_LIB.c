/*
 * File:   UART.c
 * Author: arman
 *
 * Created on 12 de agosto de 2023, 21:01
 */

#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ	4000000
#endif

// Function to initialize GPIO
//void GPIO_Init() {
//#warning Discomment these lines and assign correct GPIOs if you want to use UART
//    TRISIOx = 1; // RX pin as input
//    TRISIOx = 0; // TX pin as output
//}

/* 

// Function to generate a delay in milliseconds
void delay_ms(uint16_t ms) {
    while (ms--) {
        __delay_us(1000); // Use XC8's built-in function for microsecond delay
    }
}

// Function to transmit a single bit
void UART_WriteBit(uint8_t bit) {
    uint8_t TX;
    
    TX = bit;
    __delay_us(104); // Delay for half a bit time (9600 baud)
    TX = 1;
    __delay_us(104); // Delay for half a bit time (9600 baud)
}

// Function to transmit a byte
void UART_WriteByte(uint8_t data) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        UART_WriteBit(data & 0x01);
        data >>= 1;
    }
}

// Function to transmit a character
void UART_WriteChar(char data) {
    UART_WriteByte((uint8_t)data);
}

// Function to transmit a string
void UART_WriteString(const char *str) {
    while (*str) {
        UART_WriteChar(*str);
        str++;
    }
}
 * 
 * */