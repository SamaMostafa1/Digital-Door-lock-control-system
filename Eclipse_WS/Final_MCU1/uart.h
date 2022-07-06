/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Sama Mostafa
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum {
	Disable, Reserved, Even, Odd
} Parity_mode;
typedef enum {
	one_bit, two_bits
} Stopbit_ID;
typedef enum {
	Five_bits,
	Sex_bits,
	Seven_bits,
	Eight_bits,
	Reserved_0,
	Reserved_1,
	Reserved_2,Nine_bits
} Number_bits;
typedef enum {
	rate1 = 10,
	rate2 = 300,
	rate3 = 600,
	rate4 = 1200,
	rate5 = 2400,
	rate6 = 4800,
	rate7 = 9600,
	rate8 = 14400,
	rate9 = 19200,
	rate10 = 38400,
	rate11 = 57600,
	rate12 = 115200,
	rate13 = 128000,
	rate14 = 256000
} Baud_rate;
typedef struct {
	Parity_mode parity;
	Stopbit_ID stop_bit;
	Number_bits bit_num;
	Baud_rate rate;

} UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
