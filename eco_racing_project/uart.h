/*
 * uart.h
 *
 *  Created on: Sep 11, 2022
 *      Author: Clara Isaac
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
  FIVE_BIT , SIX_BIT , SEVEN_BIT , EIGHT_BIT , NINE_BIT = 7
} Character_Size ;

typedef enum
{
  DISABLED , EVEN_PARITY = 2 , ODD_PARITY = 3
} Parity_Mode ;

typedef enum
{
  ONE_BIT , TWO_BIT
} Stop_Bits;

typedef struct
{
	Character_Size character_size ;
	Parity_Mode    parity_mode    ;
	Stop_Bits      stop_bits      ;
	uint32         Baud_Rate      ;

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
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
