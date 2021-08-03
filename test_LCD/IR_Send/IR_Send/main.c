#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART.h"
#include "Keypad.h"

char g=0;

ISR(TIMER0_OVF_vect)
{
	g = ~g;
	if (g!=0)
	PORTD |= (1<<7);
	if (g==0)
	PORTD &= ~(1<<7);
	TCNT0 = 0xF4;
}

void Timer_dalay()
{
	TIMSK=(1<<TOIE0);	/* Enable timer interrupt */
	TCNT0 = 0xF4;		/* load TCNT0, count for 13us */
	TCCR0 = (1<<CS01);	/* Timer0, normal mode, /8 prescalar */
}

int main(void)
{
	DDRD |= 0xFF;		/* set PORTD as output port */
	UART_Init(9600);	/* set baud rate 1200 */
	sei();			/* enable global interrupt */
	Timer_dalay();		/* call timer function */
	char j, d=0;
	UART_TxChar('$');
	
	while(1)
	{
		j = keyfind();	/* search and find which key is pressed */
		switch (j)
		{
			case ('1'):
			UART_TxChar('$');	/* send the special character '$' */
			UART_TxChar('1');	/* send '1' */
			d = ~j;
			UART_TxChar(d);	/* send invert of '1' */
			_delay_ms(10);
			break;
			
			case ('2'):
			UART_TxChar('$');
			UART_TxChar('2');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('3'):
			UART_TxChar('$');
			UART_TxChar('3');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('4'):
			UART_TxChar('$');
			UART_TxChar('4');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('5'):
			UART_TxChar('$');
			UART_TxChar('5');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('6'):
			UART_TxChar('$');
			UART_TxChar('6');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('7'):
			UART_TxChar('$');
			UART_TxChar('7');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('8'):
			UART_TxChar('$');
			UART_TxChar('8');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;

			case ('9'):
			UART_TxChar('$');
			UART_TxChar('9');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('0'):
			UART_TxChar('$');
			UART_TxChar('0');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('*'):
			UART_TxChar('$');
			UART_TxChar('*');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
			
			case ('#'):
			UART_TxChar('$');
			UART_TxChar('#');
			d = ~j;
			UART_TxChar(d);
			_delay_ms(10);
			break;
		}
	}
}