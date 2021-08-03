
#define F_CPU 16000000UL	
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "stdlib.h"
#include "UART.h"
#include "LCD_4.h"

char check,data,invdata,count=0;
char bufferdata[10];

ISR(USART_RXC_vect)
{
	if(count == 0)
	{
		if (UART_RxChar() == '$')/* Check special character '$' */
		count++;		/* If yes, increment count by 1 */
		
		else			/* Else count set to 0 */
		count=0;
	}
	
	if (count == 1)
	{
		data = UART_RxChar();	/* Second character is data */
		count++;		/* Count increment by one */
	}
	
	if (count == 2)
	{
		invdata = UART_RxChar();/* Third character is inverse data */
		if ((data | invdata) == 0xFF)/* If data OR inverse data is 0xFF */
		{
			itoa(data, bufferdata, 0);
			LCD_String(bufferdata);	/* Display the data on LCD */
		}
		count = 0;			/* Count set to zero */
	}
}

int main(void)
{
	LCD_Init();		/* Initialize the 16x2 LCD */
	UART_Init(1200);	/* Set baud rate 1200 */
	LCD_Clear();		/* Clear the LCD */
	sei();			/* Enable the global interrupt */
	while(1);
}