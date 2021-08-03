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

void LCD_Command(unsigned char cmnd);
void LCD_Char (unsigned char char_data);
void LCD_String (char *str);

int main(void)
{
	LCD_Data_Dir = 0xFF;
	LCD_Command_Dir |= (1 << RS) | (1 << RW) | (1 << EN);
	
	//khoi tao LCD 
	LCD_Command(0x38);	//mode  8 bit, 2 dong
	LCD_Command(0x0C);	//bat hien thi, tat con tro
	LCD_Command(0x01);
	
	LCD_String("Hello World!");
	LCD_Command(0xC0);
	LCD_String("Goodbye");
	
    while (1) 
    {
    }
}

void LCD_Command(unsigned char cmnd)
{
	LCD_Command_Port &= ~(1 << RW);		//RW = 0
	LCD_Command_Port &= ~(1 << RS);		//RS = 0, ghi lenh
	LCD_Data_Port = cmnd;
	LCD_Command_Port |= (1 << EN);
	_delay_ms(1);
	LCD_Command_Port &= ~(1 << EN);
	_delay_ms(3);
}

void LCD_Char (unsigned char char_data)	/* LCD data write function */
{
	LCD_Command_Port &= ~(1 << RW);		//RW = 0
	LCD_Command_Port |= (1 << RS);		//RS = 1, ghi du lieu
	LCD_Data_Port = char_data;
	LCD_Command_Port |= (1 << EN);
	_delay_ms(1);
	LCD_Command_Port &= ~(1 << EN);
	_delay_ms(1);
}

void LCD_String (char *str)
{
	int i;
	for (i = 0; str[i]!= 0; i++)
	{
		LCD_Char (str[i]);
	}
}
