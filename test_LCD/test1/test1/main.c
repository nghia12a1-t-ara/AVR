/*
 * test1.c
 *
 * Created: 29/06/2020 1:45:05 PM
 * Author : Admin
 */ 
#define F_CPU 16000000ul
#include <avr/io.h>
#include "util/delay.h"


int main(void)
{	
	DDRA = 0xFF;
    /* Replace with your application code */
    while (1) 
    {
		PORTA ^= (1 << 0);
		_delay_ms(500);
    }
}

