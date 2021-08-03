/*
 * LCD.c
 *
 * Created: 26/03/2020 3:21:11 PM
 * Author : Admin
 */ 

#include <avr/io.h>
#include "util/delay.h"

#define LCD PORTB
#define Data_Dir	DDRB
#define LCD_Control	PORTD
#define LCD_Control_Dir	DDRD
#define LCD_Light	5

void LCD_Check();
void LCD_Peak();
void LCD_Cmd();
void LCD_chr(char*);

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

void LCD_Check()
{
	Data_Dir = 0;
}