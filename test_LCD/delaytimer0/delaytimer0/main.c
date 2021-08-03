/*
 * delaytimer0.c
 *
 * Created: 21/06/2020 12:50:46 PM
 * Author : Admin
 */ 
#define F_CPU 8000000ul
#include <avr/io.h>

void delayMicroseconds(unsigned int us)
{
	if (--us == 0)
	return;
	us <<= 2;
	us -= 2;
	__asm__ __volatile__ (
	"1: sbiw %0,1" "\n\t" // 2 cycles
	"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

int main(void)
{
    DDRB = 0xFF;		/* PORTB as output */
    PORTB = 0;
    while(1)  		/* Repeat forever */
    {
	    PORTB= ~ PORTB;
	    delayMicroseconds(1);
    }
}

