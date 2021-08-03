/*
 * LED_7_seg.c
 *
 * Created: 28/02/2021 12:22:43 PM
 * Author : Admin
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED_Direction	DDRD
#define LED_PORT		PORTD
#define LED_dvi		1
#define LED_chuc	0

char array[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void display(int);

int main(void)
{
	DDRC |= (1 << LED_dvi) | (1 << LED_chuc);
	LED_Direction = 0xff;
	LED_PORT = 0xff;
	
	while(1)
	{
		display(27);
	}
}

void display(int a)
{
	int chuc = a/10;
	int dvi = a%10;
	PORTC &= ~(1 << LED_dvi);
	LED_PORT = array[chuc];
	PORTC |= (1 << LED_chuc);
	_delay_ms(5);
	PORTC &= ~(1 << LED_chuc);
	LED_PORT = array[dvi];
	PORTC |= (1 << LED_dvi);
	_delay_ms(5);
}

