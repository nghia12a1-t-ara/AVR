#include <avr/io.h>
#define F_CPU 16000000UL
#include "avr/sfr_defs.h"
#include "UART.h"

void UART_Init(void)
{
	UBRRL = 51;
	UCSRC |= (1 << UCSZ1) | (1 << UCSZ0);
	UCSRB |= (1 << RXEN) | (1 << TXEN);
}

void UART_Send_Char(unsigned char c)
{
	while((UCSRA & (1 << UDRE)) == 0);
	UDR = c;
}

void UART_Send_Str(char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		UART_Send_Char(str[i]);
		i++;
	}
}

char UART_Receive(void)
{
	while((UCSRA & (1 << RXC)) == 0);
	return UDR; 
}

