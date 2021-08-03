/*
 * NOKIA5110.c
 *
 * Created: 22/05/2020 1:00:06 PM
 * Author : Admin
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "SPI_Master_C_file.c"
#include "NOKIA5110.h"
#include "Font.h"

#define MOSI 5
#define MISO 6
#define SCK 7
#define SS 4
#define DC 1
#define RST 0

int main()
{
	SPI_Init();
	N5110_init();
	N5110_clear();
	lcd_setXY(0x40,0x80);
	N5110_Data("ElectronicWings");
	while(1)
	return 0;
}