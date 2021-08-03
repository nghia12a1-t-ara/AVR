/*
 * 16_06.c
 *
 * Created: 16/06/2020 2:50:26 PM
 * Author : Admin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"
#include "avr/sfr_defs.h"
#include "LCD_4.h"
#include "PORT_4.h"
#include "UART.h"
#include "Keypad.h"

#define KEY_PRT 	PORTA
#define KEY_DDR		DDRA
#define KEY_PIN		PINA

void Timer1_Init(void);
void Kiem_tra(char);

char check, data, invdata, count = 0;

int main(void)
{
	Timer1_Init();
	UART_Init();
	LCD_Init();
	
	char j = 0;

    while (1) 
    {
		j = keyfind();
		Kiem_tra(j);
    }
}

void Kiem_tra(char j)
{
	char d = ~j;
	switch (j)
	{
		case '1':
			UART_Write_Chr('$');
			UART_Write_Chr('1');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '2':
			UART_Write_Chr('$');
			UART_Write_Chr('2');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '3':
			UART_Write_Chr('$');
			UART_Write_Chr('3');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '4':
			UART_Write_Chr('$');
			UART_Write_Chr('4');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '5':
			UART_Write_Chr('$');
			UART_Write_Chr('5');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;	
		case '6':
			UART_Write_Chr('$');
			UART_Write_Chr('6');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '7':
			UART_Write_Chr('$');
			UART_Write_Chr('7');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '8':
			UART_Write_Chr('$');
			UART_Write_Chr('8');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;	
		case '9':
			UART_Write_Chr('$');
			UART_Write_Chr('9');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '0':
			UART_Write_Chr('$');
			UART_Write_Chr('0');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '*':
			UART_Write_Chr('$');
			UART_Write_Chr('*');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
		case '#':
			UART_Write_Chr('$');
			UART_Write_Chr('#');
			UART_Write_Chr(d);
			_delay_ms(10);
			break;
	}
}

void Timer1_Init()
{
	DDRD |= 0xFF;
	TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10);  // chia 1
	ICR1 = 422;
	OCR1A = 211;
}
