/*
 * Test_LCD8.c
 *
 * Created: 14/06/2020 8:27:29 PM
 * Author : Admin
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"
#include "LCD.h"
#include "PORT.h"

int main(void)
{
	LCD_Init();
	LCD_String("Hello World!");
	LCD_Command(0xC0);
	LCD_String("Goodbye");
	
    while (1) 
    {
    }
}

