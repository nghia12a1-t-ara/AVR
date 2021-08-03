/*
 * Project3.c
 *
 * Created: 25/06/2020 6:44:26 PM
 * Author : Admin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "UART.h"
#include "LCD_4.h"

#define btn1 !(PIND & (1 << 4))
#define btn2 !(PIND & (1 << 5))
#define btn3 !(PIND & (1 << 6))
#define btn4 !(PIND & (1 << 7))

void Ktranutbam(void);
char check, data, invdata, count = 0;

ISR(USART_RXC_vect)
{
	if(count == 0)
	{
		if (UART_Read() == '$')/* Check special character '$' */
		count++;		/* If yes, increment count by 1 */
		
		else			/* Else count set to 0 */
		count = 0;
	}
	
	if (count == 1)
	{
		data = UART_Read();	/* Second character is data */
		count++;		/* Count increment by one */
	}
	
	if (count == 2)
	{
		invdata = UART_Read();/* Third character is inverse data */
		if ((data | invdata) == 0xFF)/* If data OR inverse data is 0xFF */
		{
			switch (data)
			{
				case ('1'):
					PORTB ^= (1 << PB0); 
					break;	
				case ('2'):
					PORTB ^= (1 << PB1);
					break;
				case ('3'):
					PORTB ^= (1 << PB2);
					break;
				case ('4'):
					PORTB ^= (1 << PB3);
					break;
			}
		}
		count = 0;			/* Count set to zero */
	}
}

int main(void)
{
	DDRB = 0x0F;
	DDRC = 0xFF;
	DDRA = 0x00;
	DDRD = 0x00;
	PORTB = 0x00;
	PORTD = 0xF0;
	
	LCD_Init();		/* Initialize the 16x2 LCD */
	LCD_Clear();	/* Clear the LCD */
	UART_Init();	/* Set baud rate 1200 */
	
	sei();			/* Enable the global interrupt */
	
	while(1)
	{
		Ktranutbam();
	}
}

void Ktranutbam()
{
	if(btn1)
	{
		PORTB ^= (1 << PB0);
	}
	while(btn1);
	if(btn2)
	{
		PORTB ^= (1 << PB1);
	}
	while(btn2);
	if(btn3)
	{
		PORTB ^= (1 << PB2);
	}
	while(btn3);
	if(btn4)
	{
		PORTB ^= (1 << PB3);
	}
	while(btn4);
}
