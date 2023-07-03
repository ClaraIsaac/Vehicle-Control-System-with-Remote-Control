/*
 * timer1.h
 *
 *  Created on: Sep 12, 2022
 *      Author: Clara Isaac
 */

#ifndef TIMER1_H_
#define TIMER1_H_

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef struct{
	uint8 seconds;
	uint8 minutes;
	uint8 hours;
} TIME;

extern TIME now; /* Declare an extern variable so I can use in main function*/

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description :
 * 1. Select Compare mode
 * 2. Put initial value in TCNT1 register
 * 3. Select the required prescaler
 * 4. choose the compare value
 */
void Timer1_start(void);

/*
 * Description:
 * Update the time with the current time.
 */
void set_time(TIME *time);

#endif /* TIMER1_H_ */
