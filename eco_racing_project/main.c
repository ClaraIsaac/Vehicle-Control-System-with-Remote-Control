/*
 * main.c
 *
 *  Created on: Sep 11, 2022
 *      Author: Clara Isaac
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "common_macros.h"
#include "std_types.h"
#include "gpio.h"
#include "DC_motor.h"
#include "Stepper_motor.h"
#include "lcd.h"
#include "uart.h"
#include "pwm_timer0.h"
#include "timer1.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/* Struct to gather all the vehicle information */
typedef struct
{
	uint8 speed; // speed of DC motor
	uint8 angle; // angle of Stepper motor
	uint8 direction_of_DC_motor; // Forward or Backward
	uint8 direction_of_Stepper_motor; // Left or Right
} VEHICLE;

/* Initialization of the vehicle */
VEHICLE vehicle1 = {0, 0, 'F', 0};

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

uint8 functionalize_str(uint8 str[8]); // function to check if the data received from UART is correct then handle it
void LCD_DISPLAY(const VEHICLE *vec); // function to display on LCD

/*******************************************************************************
 *                      Main Function                                          *
 *******************************************************************************/

int main(void)
{
	SREG |= (1<<7); // Enable interrupts by setting I-bit

	/* initialize variables and call functions */
	uint8 str[8], flag = 0; // string to get in the data from UART, flag indicates the UART has finished receiving
	sint16 current_angle = 0; // current position of Stepper motor
	UART_ConfigType uart_type = {EIGHT_BIT, DISABLED, TWO_BIT, 9600}; // UART specs
	DC_motor_init(); // initialize DC motor
	Stepper_motor_init(); // initialize Stepper motor
	LCD_init(); // initialize the LCD
	UART_init(&uart_type); // initialize UART
	Timer1_start(); // start Timer1 which is responsible for the current time

	/* Prepare The LCD */
	LCD_displayString("Speed:");
	LCD_moveCursor(0,10);
	LCD_displayString("Dic");
	LCD_moveCursor(1,0); // go to next line
	LCD_displayString("Time: ");

	while(1)
	{
		LCD_DISPLAY(&vehicle1); // display the data on the LCD

		// check the condition of UART is receiving something
		if (BIT_IS_SET(UCSRA,RXC))
		{
			UART_receiveString(str); // receive the data from UART
			flag = 1; // set flag
		}
		if (flag == 1) // check the flag
		{
			if (functionalize_str(str)) // check if the data received from UART is correct and handle it
			{
				speed_of_vehicle(vehicle1.speed); // set the DC motor speed
				direction_of_vehicle(vehicle1.direction_of_DC_motor); // set the direction of the DC motor (F or B)
				Stepper_motor_direction_and_angle(vehicle1.direction_of_Stepper_motor, vehicle1.angle, current_angle); // set the angle of the Stepper motor and its direction (L or R)

				// set the current angle value
				if (vehicle1.direction_of_Stepper_motor == 'L')
				{
					current_angle = -1 * vehicle1.angle;
				}
				else current_angle = vehicle1.angle;
			}
			flag = 0; // clear flag
		}

	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:
 * Display on LCD
 */
void LCD_DISPLAY(const VEHICLE *vec)
{
	// if the speed is 100 print "100%" else print the actual speed
	LCD_moveCursor(0,6);
	if(vec->speed == 100)
	{
		LCD_displayCharacter('1');
		LCD_displayCharacter('0');
		LCD_displayCharacter('0');
	}
	else{
		LCD_displayCharacter((vec->speed / 10) + 48);
		LCD_displayCharacter((vec->speed % 10) + 48);
	}
	LCD_displayCharacter('%');
	LCD_moveCursor(0,13);
	// if the Stepper motor in Left direction then print on LCD -ve of the angle
	if (vec->direction_of_Stepper_motor == 'L')
	{
		LCD_displayCharacter('-');
	}
	// print the Stepper motor's angle
	LCD_displayCharacter((vec->angle / 10) + 48);
	LCD_displayCharacter((vec->angle % 10) + 48);
	LCD_displayCharacter(' '); // indicates the end of the first line

	// print the current Time
	LCD_moveCursor(1,6);
	LCD_displayCharacter((now.hours / 10) + 48);
	LCD_displayCharacter((now.hours % 10) + 48);
	LCD_displayCharacter(':');
	LCD_displayCharacter((now.minutes / 10) + 48);
	LCD_displayCharacter((now.minutes % 10) + 48);
	LCD_displayCharacter(':');
	LCD_displayCharacter((now.seconds / 10) + 48);
	LCD_displayCharacter((now.seconds % 10) + 48);
}

/*
 * Description:
 * Check if the data received from UART is correct then handle it.
 */
uint8 functionalize_str(uint8 str[8])
{/* The commented C code is for if we don't have strict rules on the length so we can accept 76F18LE */
	//if (str [8] == '\0')
	//{
	/* check if the frame was correct one */
	if (str[7] != 'E') // last letter is not 'E'
		return 0;
	if (str[0] < '0' || str[0] > '1') // speed hundreds must be 0 or 1 only
		return 0;
	if (str[1] < '0' || str[1] > '9') // speed tens must be a number
		return 0;
	if (str[2] < '0' || str[2] > '9') // speed units must be a number
		return 0;
	// check the direction of DC motor
	if (str[3] != 'F')
		if(str[3] != 'B')
			return 0;
	if (str[4] < '0' || str[4] > '9') // angle tens must be a number
		return 0;
	if (str[5] < '0' || str[5] > '9') // angle units must be a number
		return 0;
	// check the direction of Stepper motor
	if (str[6] != 'R')
		if(str[6] != 'L')
			return 0;
	/* handle the data */
	vehicle1.speed = (str[0] - 48) * 100;
	vehicle1.speed += (str[1] - 48) * 10;
	vehicle1.speed += (str[2] - 48);

	/* speed must be from 0 to 100 */
	if (vehicle1.speed > 100)
	{
		vehicle1.speed = 0;
		return 0;
	}

	vehicle1.direction_of_DC_motor = str[3];
	vehicle1.angle = (str[4] - 48) * 10;
	vehicle1.angle += (str[5] - 48);
	vehicle1.direction_of_Stepper_motor = str[6];
	//}
	/*else if(str[7] == '\0')
	{
		if (str[6] != 'E')
			return 0;
		if (str[0] < '0' || str[0] > '1')
			return 0;
		if (str[1] < '0' || str[1] > '9')
			return 0;
		if (str[2] != 'F')
			if(str[2] != 'B')
				return 0;
		if (str[3] < '0' || str[3] > '9')
			return 0;
		if (str[4] < '0' || str[4] > '9')
			return 0;
		if (str[5] != 'R')
			if(str[5] != 'L')
				return 0;
		vehicle1.speed = (str[0] - 48) * 10;
		vehicle1.speed += (str[1] - 48);
		vehicle1.direction_of_DC_motor = str[2];
		vehicle1.angle = (str[3] - 48) * 10;
		vehicle1.angle += (str[4] - 48);
		vehicle1.direction_of_Stepper_motor = str[5];
	}*/

	return 1;
}
