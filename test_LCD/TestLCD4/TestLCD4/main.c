/*
 * TestLCD4.c
 *
 * Created: 20/06/2020 9:27:38 PM
 * Author : Admin
 */ 

#define F_CPU 16000000ul
#include <avr/io.h>
#include "util/delay.h"
#include "stdlib.h"
#include "avr/interrupt.h"

#define LCD_Dir  DDRC
#define LCD_Port PORTC			

#define RS 0				
#define EN 1			
/*#define IR_Out_Pin 4*/

void LCD_Init(void);
void LCD_Command (unsigned char);
void LCD_Char (unsigned char);
void LCD_String(char*);
void LCD_String_xy(char, char, char*);
void LCD_Clear();
void PWM_init(void);

// int count = 0, a = 0;
// char buffer[10];

// ISR (INT0_vect)
// {
// 	_delay_ms(30);
// 	a = count / 2;
// 	itoa(a, buffer, 10);
// 	LCD_String_xy(2, 0, buffer);
// 	count++;
// }

int main(void)
{
	/*DDRD |= (1 << IR_Out_Pin);*/
	DDRD &= ~(1 << 2);
	DDRD &= ~(1 << 3);
	PORTD |= (1 << 2);
	PORTD |= (1 << 3);
	LCD_Init();
	LCD_Clear();
// 	PWM_init();
// 	OCR1B = 210;
// 	
// 	GICR |= (1 << INT0);		/* Enable INT0*/
// 	MCUCR |= (1 << ISC01) | (1 << ISC00);  /* Trigger INT0 on rising edge */
// 				
// 	sei();
	
	LCD_String_xy(1, 0, "Tong san pham:");
	LCD_String_xy(2, 0, "0");
	
    while (1) 
    {
    }
}

void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);	
	LCD_Port &= ~ (1<<RS);							
	LCD_Port |= (1<<EN);							
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);		
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);	
	LCD_Port |= (1<<RS);							
	LCD_Port|= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (data << 4);		
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_String (char *str)		
{
	int i;
	for(i=0;str[i]!=0;i++)		
	{
		LCD_Char (str[i]);
	}
}

void LCD_Init (void)		
{
	LCD_Dir = 0xFF;			
	_delay_ms(20);			
	
	LCD_Command(0x02);	
	LCD_Command(0x28);           
	LCD_Command(0x0c);            
	LCD_Command(0x06);
	LCD_Command(0x01);  
	_delay_ms(2);
}

void LCD_String_xy(char row, char pos, char *str)
{
	if (row == 1 && pos < 16)
	LCD_Command((pos & 0x0F) | 0x80);
	else if (row == 2 && pos < 16)
	LCD_Command((pos & 0x0F) | 0xC0);
	LCD_String(str);
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD_Command (0x80);		/* Cursor at home position */
}

void PWM_init()
{
	TCCR1A = 0;
	TCCR1B = 0;
	// RESET l?i 2 thanh ghi
	// ??u ra PB2 là OUTPUT ( pin 10)
	
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	// ch?n Fast PWM ch? ?? ch?n TOP_value t? do  ICR1
	TCCR1A |= (1 << COM1B1);
	// So sánh th??ng( none-inverting)
	TCCR1B |= (1 << CS10);
	// F_clock/1=16mhz
	//F_pwm=16mhz/(ICR1 + 1)
	ICR1 = 421;
	// xung r?ng c?a tràn sau 30000 P_clock
}

