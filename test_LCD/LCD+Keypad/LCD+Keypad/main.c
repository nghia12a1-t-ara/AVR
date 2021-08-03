/*
 * LCD+Keypad.c
 *
 * Created: 18/06/2020 10:35:36 AM
 * Author : Nghia
 */ 

#define  F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"
#include "LCD_4.h"
#include "PORT_4.h"
#include "Keypad.h"


int main(void)
{
	LCD_Init();
	LCD_Clear();
	
    while (1) 
    {
		LCD_Char(keyfind());
		if (' ' == keyfind())
		{
			LCD_Clear();
		}
    }
}

