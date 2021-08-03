/*
 * bai1.c
 *
 * Created: 25/03/2020 5:00:08 PM
 * Author : Admin
 */ 
#define F_CPU 8000000ul
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	// set_sleep_mode();
	DDRD |= (1 << 0) | (1 << 4);
	DDRD &= ~(1 << 2);
	PORTD |= (0 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	PORTD |= (1 << 0);
	
	MCUCR &= ~((1 << SM0) | (1 << SM1) | (1 << SM2));
	MCUCR|= ( (1<<ISC01)|(0<<ISC00) );
	GICR |= (1 << INT0);
	sei();
	
	while(1)
	{
		
		if ((PIND & (1 << 1)) == 0)
		{
			_delay_ms(20);
			if ((PIND & (1 << 1)) == 0)
			{
				PORTD &= ~(1 << 0);
				PORTD &= ~(1 << 4); 
				MCUCR |= (1 << SE);
				__asm__ __volatile__ ("sleep");
				MCUCR &= ~(1 << SE);
			}
			
		}
		if ((PIND & (1 << 3)) == 0)
		{
			_delay_ms(20);
			if ((PIND & (1 << 3)) == 0)
			{
				PORTD ^= (1 << 4);
			}
			
		}
	}
}

ISR(INT0_vect)
{
	PORTD |= (1 << 0);
}


