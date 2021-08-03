/*
 * Bai1_LCD.c
 *
 * Created: 14/06/2020 9:58:55 PM
 * Author : Admin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"

#define LCD_Data_Dir DDRD		/* Define LCD data port direction */
#define LCD_Command_Dir DDRC		/* Define LCD command port direction register */
#define LCD_Data_Port PORTD		/* Define LCD data port */
#define LCD_Command_Port PORTC		/* Define LCD data port */

#define RS 0				/* Define Register Select (data/command reg.)pin */
#define RW 1				/* Define Read/Write signal pin */
#define EN 2				/* Define Enable signal pin */

void LCD_Command(unsigned char);
void LCD_Char (unsigned char);
void LCD_String (char*)

int main(void)
{
	LCD_String("Hello World!");
	LCD_Command(0xC0);
	LCD_String("Goodbye!")
	
    while (1) 
    {
    }
}

void LCD_Command(unsigned char cmnd)
{
	LCD_Data_Port= cmnd;
	LCD_Command_Port &= ~(1<<RS);	/* RS=0 command reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 Write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(3);
}

void LCD_Char (unsigned char char_data)	/* LCD data write function */
{
	LCD_Data_Port = char_data;
	LCD_Command_Port |= (1<<RS);	/* RS=1 Data reg. */
	LCD_Command_Port &= ~(1<<RW);	/* RW=0 write operation */
	LCD_Command_Port |= (1<<EN);	/* Enable Pulse */
	_delay_us(1);
	LCD_Command_Port &= ~(1<<EN);
	_delay_ms(1);
}

void LCD_String (char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)  /* send each char of string till the NULL */
	{
		LCD_Char (str[i]);  /* call LCD data write */
	}
}