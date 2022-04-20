/*
 * main2.c
 *
 *  Created on: Dec 17, 2020
 *      Author: mrmah
 */

#include "external_eeprom.h"
#include "uart.h"

extern uint8 buzzer_work;
extern uint8 motor_work;
extern uint8 choose;
extern uint8 motor_stop;

#define first_time_ack 0x10
#define first_password_ack 0x18
#define want_password_ack 0x22
#define buzzer_ack 0x26
#define door_ack 0x30
#define buzzer_ended_ack 0x34

int main (void)
{
	uint8 data=0;
	uint8 buzzer=0;
	uint8 first_usage_bool;
	EEPROM_init();
	UART_init();
	SET_BIT(DDRD,PD2);
	CLEAR_BIT(PORTD,PD2);
	SET_BIT(DDRB,PB0);
	CLEAR_BIT(PORTB,PB0);
	SET_BIT(DDRB,PB1);
	CLEAR_BIT(PORTB,PB1);

	while(!(EEPROM_readByte(0x322,&first_usage_bool)));
	if (first_usage_bool==1)
	{
		first_usage_bool=0 ;
	}
	else
	{
		first_usage_bool=1;
		while(!(EEPROM_writeByte(0x322,first_usage_bool)));
	}


	while(UART_recieveByte() != first_time_ack);
	UART_sendByte(first_usage_bool);

	if (first_usage_bool)
	{
		UART_sendByte(first_password_ack);
		for (uint8 i=0;i<5;i++)
		{
			while(!(EEPROM_writeByte((0x200+i),UART_recieveByte())));
		}
	}
	else
	{
		while(UART_recieveByte() != want_password_ack);
		for (uint8 i=0;i<5;i++)
		{
			while (!(EEPROM_readByte(0x200,&data)));
			UART_sendByte(data);
		}

		UART_sendByte(buzzer_ack);
		buzzer=UART_recieveByte();
		if (buzzer)
		{
			TIMER1_init(60000);
			SET_BIT(PORTD,PD2);
		}
		while (buzzer_work)	;
		if (buzzer)
		{
			UART_sendByte(buzzer_ended_ack);
			TIMER1_Deinit();
			CLEAR_BIT(PORTD,PD2);
			buzzer_work=1;
		}
	}

	while(1)
	{
		UART_sendByte(door_ack);
		data=UART_recieveByte();
		if (data == '=')
		{
			TIMER1_init(60000);
			CLEAR_BIT(PORTB,PB1);
			SET_BIT(PORTB,PB0);
			while(motor_work);
			TIMER1_Deinit();
			TIMER1_init(16000);
			CLEAR_BIT(PORTB,PB0);
			choose=0;
			while(motor_stop);
			TIMER1_Deinit();
			TIMER1_init(60000);
			SET_BIT(PORTB,PB1);
			CLEAR_BIT(PORTB,PB0);
			while(motor_work);
			CLEAR_BIT(PORTB,PB1);
			TIMER1_Deinit();
		}
		if (data == '*')
		{
			UART_sendByte(first_password_ack);
			for (uint8 i=0;i<5;i++)
			{
				while(!(EEPROM_writeByte((0x200+i),UART_recieveByte())));
			}
		}
	}
}
