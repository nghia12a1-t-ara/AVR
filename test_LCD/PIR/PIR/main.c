#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"
#define LED_OUTPUT		PORTB0
#define PIR_Input		PINC0

char kiemtra()
{
	if ((PINC & (1 << PIR_Input)) == 0)  
	{
		return 0;
	}
	else 
	{
		return 1;
	}
} 

int main(void)
{
	DDRC &= ~(1 << DDC0);	/* Set the PIR port as input port */
	DDRB |= (1 << DDB0);	/* Set the LED port as output port */
	PORTB &= ~(1 << PORTB0);
	char x = kiemtra();
	while(1)
	{
		if (x)
		{
			PORTB |= (1 << LED_OUTPUT);
			_delay_ms(500);
			x = kiemtra();	
		}
		if (x == 0)
		{
			PORTB &= ~(1 << LED_OUTPUT);
			x = kiemtra();
		}
	}
}
