#include <avr/io.h>
#define F_CPU 16000000UL
#include "avr/sfr_defs.h"
#include "UART.h"

void UART_Init()
{
	UBRRL = 51;
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
	UCSRC |= (1 << UCSZ1) | (1 << UCSZ0);						//Baud = 9600, U2X = 0, F = 16MHz
}

void UART_Write_Chr(unsigned char c)
{
	while(bit_is_clear(UCSRA, UDRE));
	UDR = c;
}

void UART_Write_Str(char *str)
{
	unsigned char i = 0;
	while (str[i] != 0)
	{
		UART_Write_Chr(str[i]);
		i++;
	}
}

unsigned char UART_Read()
{
	while(bit_is_clear(UCSRA, RXC));
	return UDR;
}
