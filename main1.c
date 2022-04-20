/*
 * main1.c
 *
 *  Created on: Dec 17, 2020
 *      Author: mrmah
 */

#include "keypad.h"
#include "lcd.h"
#include "uart.h"

#define first_time_ack 0x10
#define first_password_ack 0x18
#define want_password_ack 0x22
#define buzzer_ack 0x26
#define door_ack 0x30
#define buzzer_ended_ack 0x34

int main (void)
{
	uint8 first_usage_bool;
	uint8 data=0;
	uint8 ar[5]={0};
	uint8 ar1[5];
	uint8 matched_password_bool=1;
	uint8 first_time_user_friendly_bool=1;
	uint8 try_number=0;
	uint8 buzzer=0;
	uint8 correct=0;
	LCD_init();
	UART_init();
	UART_sendByte(first_time_ack);
	first_usage_bool=UART_recieveByte();

	while (first_usage_bool)
	{
		LCD_displayString("Enter New Pass:");
		LCD_goToRowColumn(1,0);

		for(uint8 i=0;i<5;i++)
		{
			ar[i]=KeyPad_getPressedKey();
			_delay_ms(2000);
			LCD_displayCharacter('*');
		}

		LCD_clearScreen();
		LCD_displayString("Enter Pass:");
		LCD_goToRowColumn(1,0);

		for(uint8 i=0;i<5;i++)
		{
			ar1[i]=KeyPad_getPressedKey();
			_delay_ms(2000);
			LCD_displayCharacter('*');
		}

		for(uint8 i=0;i<5;i++)
		{
			if (ar1[i] != ar[i])
			{
				matched_password_bool=0;
				break;
			}
			else
			{
				correct++;
			}
		}
		if (correct==5)
		{
			matched_password_bool=1;
			correct=0;
		}

		if (matched_password_bool)
		{
			first_usage_bool=0;
			first_time_user_friendly_bool=0;
			while(UART_recieveByte() != first_password_ack);
			for (uint8 i=0;i<5;i++)
			{
				UART_sendByte(ar[i]);
			}

		}
	}

	if (first_time_user_friendly_bool)
	{
		UART_sendByte(want_password_ack);

		for (uint8 i=0;i<5;i++)
		{
			ar[i]=UART_recieveByte();
			LCD_clearScreen();
			LCD_intgerToString(ar[i]);
			_delay_ms(10000);
		}

		while(correct !=5)
		{
			LCD_clearScreen();
			LCD_displayString("Enter Pass:");
			LCD_goToRowColumn(1,0);

			for(uint8 i=0;i<5;i++)
			{
				ar1[i]=KeyPad_getPressedKey();
				_delay_ms(2000);
				LCD_displayCharacter('*');
			}

			for(uint8 i=0;i<5;i++)
			{
				if (ar1[i] != ar[i])
				{
					try_number++;
					if (try_number==3)
					{
						buzzer=1;
						while(UART_recieveByte() != buzzer_ack);
						UART_sendByte(buzzer);
						while (UART_recieveByte()!= buzzer_ended_ack)
						{
							LCD_clearScreen();
							LCD_displayString("THIEF !!!!!");
						}
					}
					break;
				}
				else
				{
					correct++;
				}

			}

		}
	}


	while(1)
	{
		LCD_clearScreen();
		LCD_displayString("* : Change Pass");
		LCD_goToRowColumn(1,0);
		LCD_displayString("= : Open Door");
		data=KeyPad_getPressedKey();
		_delay_ms(2000);
		while(UART_recieveByte () != door_ack);
		UART_sendByte(data);

		if (data=='*')
		{
			matched_password_bool=1;
			LCD_clearScreen();
			LCD_displayString("Enter New Pass :");
			LCD_goToRowColumn(1,0);

			for(uint8 i=0;i<5;i++)
			{
				ar[i]=KeyPad_getPressedKey();
				_delay_ms(2000);
				LCD_displayCharacter('*');
			}

			LCD_clearScreen();
			LCD_displayString("Enter Pass:");
			LCD_goToRowColumn(1,0);

			for(uint8 i=0;i<5;i++)
			{
				ar1[i]=KeyPad_getPressedKey();
				_delay_ms(2000);
				LCD_displayCharacter('*');
			}

			for(uint8 i=0;i<5;i++)
			{
				if (ar1[i] != ar[i])
				{
					matched_password_bool=0;
					break;
				}
			}

			if (matched_password_bool)
			{
				first_usage_bool=0;
				first_time_user_friendly_bool=0;
				while(UART_recieveByte() != first_password_ack);
				for (uint8 i=0;i<5;i++)
				{
					UART_sendByte(ar[i]);
				}

			}
		}
	}
}
