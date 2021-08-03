/*
 * Test_LCD4.c
 *
 * Created: 21/05/2020 4:37:37 PM
 * Author : Admin
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include "util/delay.h"
#include "LCD_4.h"
#include "ADC.h"

int main(void)
{
	LCD_Init();
	ADC_Init();
	LCD_String("Hello World!");	/* Write string on 1st line of LCD*/
	LCD_Command(0xC0);		/* Go to 2nd line*/
	LCD_String("Goodbye!");
	
	char buffer[5]; 
    /* Replace with your application code */
    while (1) 
    {
		int x = ADC_Read(0);
		itoa(x, buffer, 10);
		LCD_String(buffer);
    }
}

