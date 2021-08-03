/*
 * Test_keypad.c
 *
 * Created: 15/06/2020 9:22:05 PM
 * Author : Admin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"
#include "Keypad.h"
#include "LCD_4.h"
#include "PORT_4.h"
#include "UART.h"

#define KEY_PRT 	PORTA
#define KEY_DDR		DDRA
#define KEY_PIN		PINA

void Timer1_Init(void);

int main(void)
{
	UART_Init();
	Timer1_Init();		//phat xung 38Khz ra chan D5
	LCD_Init();
	
	LCD_String_xy(0,0,"Press a key");
	char j, d=0;
	
	while(1)
	{
		j = keyfind();
		LCD_Command(0xC0);
		LCD_Char(keyfind());
		switch (j)
		{
			case ('1'):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('1');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
			
			case ('2'):
			UART_Write_Chr('$');
			UART_Write_Chr('2');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('3'):
			UART_Write_Chr('$');
			UART_Write_Chr('3');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('4'):
			UART_Write_Chr('$');
			UART_Write_Chr('4');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('5'):
			UART_Write_Chr('$');
			UART_Write_Chr('5');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('6'):
			UART_Write_Chr('$');
			UART_Write_Chr('6');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('7'):
			UART_Write_Chr('$');
			UART_Write_Chr('7');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('8'):
			UART_Write_Chr('$');
			UART_Write_Chr('8');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;

			case ('9'):
			UART_Write_Chr('$');
			UART_Write_Chr('9');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('0'):
			UART_Write_Chr('$');
			UART_Write_Chr('0');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('*'):
			UART_Write_Chr('$');
			UART_Write_Chr('*');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
			
			case ('#'):
			UART_Write_Chr('$');
			UART_Write_Chr('#');
			d = ~j;
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		}	
	}
}

void Timer1_Init()
{
	DDRD = 0xFF;
	TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10);  // chia 1
	ICR1 = 422;
	OCR1A = 211;
}

