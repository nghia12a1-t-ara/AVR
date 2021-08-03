/*
 * bai1.c
 *
 * Created: 26/03/2020 3:46:35 PM
 * Author : Admin
 */ 
#define	F_CPU 16000000UL 

#include <avr/io.h>
#include "util/delay.h"

#define	LCD	PORTC
#define LCD_RS	0
#define LCD_EN	1

#define LCD_D7 7
#define LCD_D6 6
#define LCD_D5 5
#define LCD_D4 4

void LCD_Write_High_Data(char b)
{
	LCD &= 0x0F;
	LCD |= (b & 0xF0);
}

void LCD_Write_Low_Data(char b)
{
	b = b << 4;
	LCD &= 0x0F;
	LCD |= (b & 0xF0); 
}

void LCD_Enable()
{
	LCD |= (1 << LCD_EN);
	LCD &= ~(1 << LCD_EN);
}

void LCD_Cmd(char cmd)
{
	LCD &= ~(1 << LCD_RS);
	LCD_Write_High_Data(cmd);
	LCD_Enable();
	LCD_Write_Low_Data(cmd);
	LCD_Enable();
	LCD &= ~(1 << LCD_EN);
	LCD |= (1 << LCD_EN);
	_delay_ms(2);
}

void LCD_Init()
{
	LCD &= ~(1 << LCD_RS) & ~(1 << LCD_EN);
	#ifdef	USE_LCD_RW
	LCD &= ~(1 << LCD_RW);
	#endif

	LCD_Cmd(0x30);
	_delay_ms(5);
	LCD_Cmd(0x30);
	_delay_ms(1);
	LCD_Cmd(0x30);
	LCD_Cmd(0x38);
	
	LCD_Cmd(0x01);
	LCD_Cmd(0x0C);
}

void LCD_Chr_Cp(char c)
{
	LCD |= (1 << LCD_RS);
	LCD_Write_High_Data(c);
	LCD_Enable();
	LCD_Write_Low_Data(c);
	LCD_Enable();
	_delay_ms(1);
}

void LCD_String(char *str)
{	
	unsigned char i = 0;
	while (str[i] != 0)
	{
		LCD_Chr_Cp(str[i]);
		i++;
	}
}

int main(void)
{	
	DDRD |= 0xFF;
	DDRC |= 0xFF;
	PORTA |= (1<<DDA1);
	LCD_Init();
	LCD_Write_High_Data(0x31);
	_delay_ms(2000);
	LCD_Write_Low_Data(0x31);
	unsigned char i = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		if ((PINA&(1<<PINA1)) == 0)
		{	
			i++;
			LCD_Chr_Cp(i);
			while((PINA&(1<<PINA1)) == 0);
		}
    }
}

