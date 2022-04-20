/*
 * timer.c
 *
 *  Created on: Dec 17, 2020
 *      Author: mrmah
 */

#include "timer.h"
static uint8 volatile g_counter=0;
uint8 volatile buzzer_work=1;
uint8 volatile motor_work=1;
uint8 volatile choose=1;
uint8 volatile motor_stop=1;

void TIMER1_init(uint16 time)
{
	SET_BIT(SREG,7);
	SET_BIT(TCCR1A,FOC1A);
	SET_BIT(TCCR1A,FOC1B);
	SET_BIT(TCCR1B,WGM12);
	SET_BIT(TCCR1B,CS12);
	SET_BIT(TCCR1B,CS10);
	SET_BIT(TIMSK,OCIE1A);
	TCNT1=0;
	OCR1A=time;
}

ISR(TIMER1_COMPA_vect)
{
	if (choose)
	{
		g_counter++;
		if (g_counter == 8)
		{
			buzzer_work=0;
			g_counter=0;
			motor_work=1;
		}
		if (g_counter == 2)
		{
			motor_work=0;
		}
	}
	else
	{
		motor_stop=0;
	}
}

void TIMER1_Deinit(void)
{
	CLEAR_BIT(SREG,7);
	CLEAR_BIT(TCCR1A,FOC1A);
	CLEAR_BIT(TCCR1A,FOC1B);
	CLEAR_BIT(TCCR1B,WGM12);
	CLEAR_BIT(TCCR1B,CS12);
	CLEAR_BIT(TCCR1B,CS10);
	CLEAR_BIT(TIMSK,OCIE1A);
	TCNT1=0;
	OCR1A=0;
}
