#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#define  F_CPU 16000000UL
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

#define SPI		PORTB
#define	SCK		7
#define MISO	6
#define MOSI	5
#define SS		4

#define cbi(port, bit)	(port) &= ~(1 << (bit))
#define sbi(port, bit)	(port) |= (1 << (bit))

void SPI_MasterInit(void);
void SPI_Transmit(uint8_t data);

#endif