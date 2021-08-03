/*
 * UART_slave.c
 *
 * Created: 21/07/2020 10:02:42 PM
 * Author : Admin
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void UART_init(long USART_BAUDRATE);
unsigned char UART_RxChar();

int main(void)
{
	DDRB |= (1 << 0);		//LED is OUTPUT
	PORTB &= ~(1 << 0);		//LED is LOW
	UART_init(9600);
	char c;
    /* Replace with your application code */
    while (1) 
    {
		c = UART_RxChar();
		if (c == '0')
		{ 
			PORTB |= (1 << 0);
		}
		if (c == '1')
		{
			PORTB &= ~(1 << 0);
		}
    }
}

void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);						//Cho phép truy?n nh?n d? li?u
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);	//Khung truy?n 8 bit
	UBRRL = BAUD_PRESCALE;									//C?u hình Baudrate
	UBRRH = (BAUD_PRESCALE >> 8);
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0);
	return(UDR);		
}