/*
 * Stepper_motor.h
 *
 *  Created on: Sep 11, 2022
 *      Author: Clara Isaac
 */

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

#include "std_types.h"

/* DC motor HW Ports and Pins Ids */
#define Stepper_motor_A_PORT_ID PORTC_ID
#define Stepper_motor_B_PORT_ID PORTC_ID
#define Stepper_motor_ABar_PORT_ID PORTC_ID
#define Stepper_motor_BBar_PORT_ID PORTC_ID

#define Stepper_motor_A_PIN_ID PIN2_ID
#define Stepper_motor_B_PIN_ID PIN3_ID
#define Stepper_motor_ABar_PIN_ID PIN4_ID
#define Stepper_motor_BBar_PIN_ID PIN5_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the Stepper motor:
 * 1. Setup the Stepper motor pins directions by use the GPIO driver.
 * 2. Initialize Values on pins.
 */
void Stepper_motor_init(void);

/*
 * Description:
 * Setup the angle and the direction (Left or Right) of the Stepper motor.
 */
void Stepper_motor_direction_and_angle(uint8 direction, uint8 angle, sint16 current_angle);

/*
 * Description:
 * Move clockwise.
 */
void clockwiseturn(uint8 angles);

/*
 * Description:
 * Move anti-clockwise.
 */
void anticlockwiseturn(uint8 angles);

#endif /* STEPPER_MOTOR_H_ */
