/*
 * timer1.c
 *
 *  Created on: Sep 12, 2022
 *      Author: Clara Isaac
 */

#include <avr/interrupt.h> /* To use interrupts*/
#include <avr/io.h> /* To use Timer1 Registers */
#include "std_types.h"
#include "common_macros.h"
#include "timer1.h"

/*Initialize the time 00:00:00 */
TIME now = {0, 0, 0};

/*******************************************************************************
 *                       Interrupt Service Routine                             *
 *******************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	SET_BIT(TIFR, OCF1A); /* clear Timer1, Output Compare A Match Flag */
	set_time(&now); /* update the time */
	/* ISR mustn't have a function call within it but in our case with only one interrupt we can do that */
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Description :
 * 1. Select Compare mode
 * 2. Put initial value in TCNT1 register
 * 3. Select the required prescaler
 * 4. choose the compare value
 */
void Timer1_start(void)
{
	/* initial value */
	TCNT1 = 0;
	/*store compare value in OCR1A register */
	OCR1A = 15625;
	/*Enable Timer1 Compare A Interrupt */
	TIMSK |= (1<<OCIE1A);
	/* Set FOC1A for non-PWM mode */
	TCCR1A |= (1<<FOC1A);
	/*select prescaler of N = 64 and select compare mode */
	TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);
}

/*
 * Description:
 * Update the time with the current time.
 */
void set_time(TIME *time)
{
		time->seconds++;
		if(time->seconds > 59)
		{
			time->minutes++;
			time->seconds = 0;
		}
		if (time->minutes > 59)
		{
			time->hours++;
			time->minutes = 0;
		}
		if(time->hours > 23)
		{
			time->seconds = 0;
			time->minutes = 0;
			time->hours = 0;
		}
}
