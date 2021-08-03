#define F_CPU 8000000ul
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	// IO Setting
	DDRD |= (1 << 0) | (1 << 4);
	DDRD &= ~((1 << 1) | (1 << 2) | (1 << 3));
	PORTD |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 0);
	
	MCUCR |= (1 << ISC01) | (0 << ISC00);
	GICR |= (1 << INT0);
	 
	sei();
	
	while(1)
	{
		if ((PIND & (1 << 3)) == 0)
		{
			_delay_ms(20);
			if ((PIND & (1 << 3)) == 0)
			{
				PORTD ^= (1 << 4);
			}
		}
		
		// sleep mode
		if ((PIND & (1 << 1)) == 0)
		{
			_delay_ms(20);
			if ((PIND & (1 << 1)) == 0)
			{
				PORTD &= ~ ((1 << 0) | (1 << 4));
				MCUCR |= (1 << SE);
				__asm__ __volatile__ ("sleep");
				MCUCR &= ~(1 << SE);
			}
		}
	}
}

ISR(INT0_vect)
{
	PORTD |= (1 << 0);	
}


