/*
 * ADC.c
 *
 * Created: 21/05/2020 8:54:37 PM
 * Author : Admin
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"
#include "LCD_4.c"
#include "ADC.c"
#include "stdlib.h"

#define LCD_Dir  DDRB			/* Define LCD data port direction */
#define LCD_Port PORTB			/* Define LCD data port */
#define RS 0				/* Define Register Select pin */
#define EN 1 				/* Define Enable signal pin */

int main()
{
	char String[5];
	int value;

	ADC_Init();
	LCD_Init();			/* Initialization of LCD */
	LCD_String("ADC value");	/* Write string on 1st line of LCD */

	while(1)
	{
		LCD_Command(0xc4);	/* LCD16x2 cursor position */
		value = ADC_Read(0);	/* Read ADC channel 0 */
		itoa(value,String,10);	/* Integer to string conversion */
		LCD_String(String);
		LCD_String("  ");
	}
	return 0;
}
