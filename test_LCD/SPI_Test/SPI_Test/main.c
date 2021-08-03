/*
 * SPI_Test.c
 *
 * Created: 29/07/2020 8:05:53 AM
 * Author : Admin
 */ 
#define F_CPU 8000000UL			
#include <avr/io.h>		
#include <util/delay.h>			
#include <stdio.h>		
#include <string.h>	
#include "LCD_4.h"		

#define MISO	6
#define MOSI	5	
#define SS		4
#define SCK		7

void SPI_Master_Init(void);
void SPI_Master_Write(char);

int main(void)
{
	uint8_t count;
	char buffer[5];
	
	LCD_Init();
	SPI_Master_Init();
	
	LCD_String_xy(0, 0, "Master Device");
	LCD_String_xy(1, 0, "Sending Data:    ");
	
	PORTB &= ~(1 << SS);		//enable slave
	count = 0;
	
    while (1) 
    {
		SPI_Master_Write(count);
		sprintf(buffer, "%d   ", count);
		LCD_String_xy(1, 13, buffer);
		count++;
		_delay_ms(500);
    }
}

void SPI_Master_Init()				
{
	DDRB |= (1 << MOSI) | (1 << SCK) | (1 << SS);		/*MOSI, SCK, SS = Output*/
	DDRB &= ~(1 << MISO);								/* MISO = Input */
	PORTB |= (1 << SS);									/* SS = 1 */
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);		/* Enable SPI master, F = Fosc/16 */
}

void SPI_Master_Write(char data)		
{
	SPDR = data;				/* Ghi du lieu vào thanh ghi du lieu */
	while(!(SPSR & (1<<SPIF)));	/* Gui truyen xong */
}

