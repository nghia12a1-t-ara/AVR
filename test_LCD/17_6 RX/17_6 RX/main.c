/*
 * 17_6 RX.c
 *
 * Created: 17/06/2020 2:58:07 PM
 * Author : Admin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"
#include "avr/interrupt.h"
#include "PORT_4.h"
#include "LCD_4.h"

char check, data, invdata, count = 0;

void UART_Init(void);
char UART_Read(void);

int main(void)
{
	LCD_Init();
	UART_Init();
	LCD_Clear();
	LCD_String("Data_IN");
	
	sei();

    while (1) 
    {
    }
}

ISR(USART_RXC_vect)
{
	if (count == 0)
	{
		if (UART_Read() == '$')
		{
			count++;
		} 
		else
		{
			count = 0;
		}
	}
	if (count == 1)
	{
		data = UART_Read();
		count++;
	}
	if (count == 2)
	{
		invdata = UART_Read();
		if ((data | invdata) == 0xFF)
		{
			LCD_Command(0xC0);
			LCD_Char(data);
		}
	count = 0;
	}
}

void UART_Init()
{
	UBRRL = 102;
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
	UCSRC |= (1 << UCSZ1) | (1 << UCSZ0);
}

char UART_Read()
{
	while(bit_is_clear(UCSRA, RXC));
	return UDR;
}