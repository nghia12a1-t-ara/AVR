/*
 * Project3_TX.c
 *
 * Created: 25/06/2020 11:18:14 PM
 * Author : Admin
 */ 
#define F_CPU 16000000ul
#include <avr/io.h>
#include "util/delay.h"
#include "UART.h"

#define btn1 !(PINC & (1 << PINC0))
#define btn2 !(PINC & (1 << PINC1))
#define btn3 !(PINC & (1 << PINC2))
#define btn4 !(PINC & (1 << PINC3))

void PWM_Init(void);
void Kiemtra(void);

char j, d = 0;
char a = 0;

int main(void)
{
	DDRC = 0x00;
	PORTC = 0xFF;
	DDRD = 0xFF;
	
	PWM_Init();
	OCR1B = 210;
	UART_Init();

    while (1) 
    {
		if(btn1)
		{
			a = 1;
		}
		if(btn2)
		{
			a = 2;
		}
		if(btn3)
		{
			a = 3;
		}
		if(btn4)
		{
			a = 4;
		}
		Kiemtra();
		a = 0;
    }
}

void PWM_Init()
{
	TCCR1A = 0;
	TCCR1B = 0;
	// RESET l?i 2 thanh ghi
	// ??u ra PB2 là OUTPUT ( pin 10)
	
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	// ch?n Fast PWM ch? ?? ch?n TOP_value t? do  ICR1
	TCCR1A |= (1 << COM1B1);
	// So sánh th??ng( none-inverting)
	TCCR1B |= (1 << CS10);
	// F_clock/1=16mhz
	//F_pwm=16mhz/(ICR1 + 1)
	ICR1 = 421;
	// xung r?ng c?a tràn sau 30000 P_clock
}

void Kiemtra()
{
	switch (a)
	{
		case (1):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('1');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
		case (2):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('2');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
		case (3):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('3');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
		case (4):
			UART_Write_Chr('$');	/* send the special character '$' */
			UART_Write_Chr('4');	/* send '1' */
			d = ~j;
			UART_Write_Chr(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
	}
}