#include "SPI.h"

void SPI_MasterInit(void)
{
	SPI |= (1 << SCK) | (1 << MOSI) | (1 << SS);
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);		//Master, F_CPU/4
	sbi(SPI, SS);
}

void SPI_Transmit(uint8_t data)
{
	SPDR = data;
	while(bit_is_clear(SPSR, SPIF));
}