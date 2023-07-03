/*
 * Stepper_motor.c
 *
 *  Created on: Sep 11, 2022
 *      Author: Clara Isaac
 */

#include <avr/io.h>
#include <util/delay.h> /* To use Delay Function */
#include "gpio.h"
#include "Stepper_motor.h"

/*******************************************************************************
 *                       Variables Declarations                                *
 *******************************************************************************/

uint8 final_position = 0; /* store the last position of the motor (12 , 6, 3, 9) */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the Stepper motor:
 * 1. Setup the Stepper motor pins directions by use the GPIO driver.
 * 2. Initialize Values on pins.
 */
void Stepper_motor_init(void)
{
	/* Configure the direction for A, B, Abar, Bbar pins as output pins */
	GPIO_setupPinDirection(Stepper_motor_A_PORT_ID, Stepper_motor_A_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(Stepper_motor_B_PORT_ID, Stepper_motor_B_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(Stepper_motor_ABar_PORT_ID, Stepper_motor_ABar_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(Stepper_motor_BBar_PORT_ID, Stepper_motor_BBar_PIN_ID, PIN_OUTPUT);

	/* Write on the PINS Logic Low */
	GPIO_writePin(Stepper_motor_A_PORT_ID, Stepper_motor_A_PIN_ID, LOGIC_LOW);
	GPIO_writePin(Stepper_motor_B_PORT_ID, Stepper_motor_B_PIN_ID, LOGIC_LOW);
	GPIO_writePin(Stepper_motor_ABar_PORT_ID, Stepper_motor_ABar_PIN_ID, LOGIC_LOW);
	GPIO_writePin(Stepper_motor_BBar_PORT_ID, Stepper_motor_BBar_PIN_ID, LOGIC_LOW);
}

/*
 * Description:
 * Setup the angle and the direction (Left or Right) of the Stepper motor.
 */
void Stepper_motor_direction_and_angle(uint8 direction, uint8 angle, sint16 current_angle)
{
	/* Return to 0 degree from your position */
	if (current_angle < 0)
	{
		clockwiseturn((current_angle * -1));
	}
	else if (current_angle > 0)
	{
		anticlockwiseturn(current_angle);
	}

	/* Go to the angle specified */
	if (direction == 'R') // Right Direction
	{
		clockwiseturn(angle);
	}
	else if (direction == 'L') // Left Direction
	{
		anticlockwiseturn(angle);
	}
}

/*
 * Description:
 * Move clockwise.
 */
void clockwiseturn(uint8 angles)
{
	/* Initialization of variables.
	 * counter for looping until reach the angle value
	 * clockwise_array to rotate in clockwise direction
	 * i for looping over the clockwise_array
	 * j for looping over the array the check which position the motor hold
	 */
	uint8 counter = 0, i, j, clockwise_array[4] = {12,6,3,9};
	if (final_position == 0) /* if the motor didn't move yet */
	{
		PORTC = (PORTC & 0xC3) | (12 << 2); /* initial Position */
		_delay_ms(50);
		i = 1;
	}
	else{
		/* find the last position and start from it */
		for (j = 0; j <4; j++)
		{
			if(final_position == clockwise_array[j])
				i = j;
		}
	}
	while(counter < angles)
	{
		/* write on the 4 pins the required value to move the motor */
		PORTC = (PORTC & 0xC3) | (clockwise_array[i] << 2);
		i++;
		if (i == 4) /* return to the first element in the array */
			i = 0;
		counter++;
		_delay_ms(50);
	}
	PORTC = (PORTC & 0xC3) | ( clockwise_array[i]<< 2); /* initial Position */
	_delay_ms(50);
	final_position = clockwise_array[i];
}

/*
 * Description:
 * Move anti-clockwise.
 */
void anticlockwiseturn(uint8 angles)
{
	/* Initialization of variables.
	 * counter for looping until reach the angle value
	 * anticlockwise_array to rotate in clockwise direction
	 * i for looping over the anticlockwise_array
	 * j for looping over the array the check which position the motor hold
	 */
	uint8 counter = 0, i, j, anticlockwise_array[4] = {3,6,12,9};
	if (final_position == 0) /* if the motor didn't move yet */
	{
		PORTC = (PORTC & 0xC3) | (3 << 2); /* initial Position */
		_delay_ms(50);
		i = 1;
	}
	else{
		/* find the last position and start from it */
		for (j = 0; j <4; j++)
		{
			if(final_position == anticlockwise_array[j])
				i = j;
		}
	}
	while(counter < angles)
	{
		/* write on the 4 pins the required value to move the motor */
		PORTC = ((PORTC & 0xC3) | (anticlockwise_array[i] << 2));
		i++;
		if (i == 4) /* return to the first element in the array */
			i = 0;
		counter++;
		_delay_ms(50);
	}
	PORTC = ((PORTC & 0xC3) | (anticlockwise_array[i] << 2)); /* initial Position */
	_delay_ms(50);
	final_position = anticlockwise_array[i];
}
