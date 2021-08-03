/*
 * SPI_Slave_Test.c
 *
 * Created: 29/07/2020 10:25:06 AM
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

void SPI_Slave_Init(void);
char SPI_Slave_Receive(void);

int main(void)
{
    uint8_t count;
    char buffer[5];
    
    LCD_Init();
    SPI_Slave_Init();
    
    LCD_String_xy(0, 0, "Slave Device");
    LCD_String_xy(1, 0, "Receive Data:    ");
	
    while (1) 
    {
		count = SPI_Slave_Receive();
		sprintf(buffer, "%d   ", count);
		LCD_String_xy(1, 13, buffer);
    }
}

void SPI_Slave_Init()		
{
	DDRB &= ~((1 << MOSI) | (1 << SCK) | (1 << SS));  /* MOSI, SCK, SS = Input */			
	DDRB |= (1 << MISO);			/* MISO = Output */
	SPCR = (1 << SPE);			/* Enable SPI, slave mode*/
}

char SPI_Slave_Receive()			
{
	while(!(SPSR & (1<<SPIF)));	/* Doi nhan xong */
	return(SPDR);				/* Doc gia tri thanh ghi du lieu */
}
