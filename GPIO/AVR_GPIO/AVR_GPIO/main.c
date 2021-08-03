/*
 * AVR_GPIO.c
 *
 * Created: 24/02/2021 9:43:55 PM
 * Author : Admin
 */ 
#define F_CPU 16000000ul
#include <avr/io.h>
#include <util/delay.h>

#define led		1
#define button	0
// 1 nut bam & 1 den led
// bam nut bam => dao trang thai led

int main(void)
{
	// Set input - output
	DDRD &= ~(1 << button);		// input D0, mac dinh
	DDRD |= (1 << led);		// output D1
    // button = input_pullup
	PORTD |= (1 << button);		
	// led = output
	PORTD |= (1 << led); 
	
    while (1) 
    {
		//kiem tra button
		if ((PIND & (1 << button)) == 0)
		{
			_delay_us(300);
			if ((PIND & (1 << button)) == 0)
			{
				PORTD ^= (1 << led);
			}
		}
    }
}

