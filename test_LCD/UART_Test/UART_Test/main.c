/*
 * UART_Test.c
 *
 * Created: 21/07/2020 9:11:07 PM
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
void UART_TxChar(char ch);

int main()
{
	DDRB = 0x00;
	PORTB |= (1 << 0) | (1 << 1);
	UART_init(9600);
	
	while(1)
	{	
		if (!(PINB & (1 << 0)))
		{
			_delay_ms(20);
			if (!(PINB & (1 << 0)))
			{
				while (!(PINB & (1 << 0)));
				UART_TxChar('0');
			}
		}
		if (!(PINB & (1 << 1)))
		{
			UART_TxChar('1');
			while (!(PINB & (1 << 1)));
		}
	}
}

void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);						//cho phep truyen nhan du lieu
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);	
	UBRRL = BAUD_PRESCALE;			
	UBRRH = (BAUD_PRESCALE >> 8);		// chọn tốc độ truyền
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));		//đợi UDRE = 1
	UDR = ch ;
}


