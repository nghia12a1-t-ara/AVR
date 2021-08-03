/*
 * Project3_RX.c
 *
 * Created: 25/06/2020 10:25:42 PM
 * Author : Admin
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include "util/delay.h"

#define  btn1	!(PINC & PINC0)
#define  btn2	!(PINC & PINC1)
#define  btn3	!(PINC & PINC2)
#define  btn4	!(PINC & PINC3)

void PWM_Init(void);
void Kiemtra(void);

char a = '0';
char j, d = 0;

int main(void)
{
	DDRC = 0x00;
	PORTC = 0xFF;
	PWM_Init();
	OCR1B = 210;
	UART_Init();
	
    while (1) 
    {
		if (btn1)
		{
			a = '1';
		}
		if (btn2)
		{
			a = '2';
		}
		if (btn3)
		{
			a = '3';
		}
		if (btn4)
		{
			a = '4';
		}
		Kiemtra();
		a = '0';
    }
}

void PWM_Init()
{
	 TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	 TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
	 ICR1 = 26;
	 OCR1B = 13; 
}

void Kiemtra()
{
	switch (a)
	{
		case ('1'):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('1');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
		case ('2'):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('2');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
		case ('3'):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('3');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
		case ('4'):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('4');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;	
	}
}