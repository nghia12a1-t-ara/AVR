/*
 * I2C_Master_test.c
 *
 * Created: 05/08/2020 10:59:40 PM
 * Author : Admin
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>		/* Include inbuilt defined Delay header file */
#include <stdio.h>		/* Include standard I/O header file */
#include <string.h>		/* Include string header file */
#include "LCD_4.h"
#include "PORT_4.h"

#define SCL_CLK 100000L
/* Dinh nghia tan so SCL theo cong thuc */
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))
#define Slave_Write_Address		0x20
#define Slave_Read_Address		0x21
#define	count					10

void I2C_Init(void);
uint8_t I2C_Start(char write_address);
uint8_t I2C_Repeated_Start(char read_address);
void I2C_Start_Wait(char write_address);
uint8_t I2C_Write(char data);
char I2C_Read_Ack(void);
char I2C_Read_Nack(void);
void I2C_Stop(void);

int main()
{
	char buffer[10];
	
	LCD_Init();			/* Initialize LCD */
	I2C_Init();			/* Initialize I2C */
	
	LCD_String_xy(0, 0, "Master Device");
	
	while (1)
	{
		LCD_String_xy(1, 0, "Sending :       ");
		I2C_Start_Wait(Slave_Write_Address);/* Start I2C with SLA+W */
		_delay_ms(5);
		for (uint8_t i = 0; i < count ; i++)
		{
			sprintf(buffer, "%d    ",i);
			LCD_String_xy(1, 13, buffer);
			I2C_Write(i);	/* Send Incrementing count */
			_delay_ms(500);
		}
		LCD_String_xy(1, 0, "Receiving :       ");
		I2C_Repeated_Start(Slave_Read_Address);/* Repeated Start with SLA+R */
		_delay_ms(5);
		for (uint8_t i = 0; i < count; i++)
		{
			if(i < count - 1)
			sprintf(buffer, "%d    ", I2C_Read_Ack());/* Read & Ack of data */
			else
			sprintf(buffer, "%d    ", I2C_Read_Nack());/* Read & Nack to data */
			LCD_String_xy(1, 13, buffer);
			_delay_ms(500);
		}
		I2C_Stop();		/* Stop I2C */
	}
}

void I2C_Init()	
{
	TWBR = BITRATE(TWSR=0x00);
}

uint8_t I2C_Start(char write_address)
{
	uint8_t status;		
	/* Enable TWI, generate START */
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); 
	while(!(TWCR&(1<<TWINT)));
	/* Read TWI status register */
	status = TWSR & 0xF8;		
	/* Check weather START transmitted or not? */
	if(status != 0x08)		
	return 0;			/* Return 0 to indicate start condition fail */
	/* Write SLA+W in TWI data register */
	TWDR = write_address;		
	TWCR = (1 << TWEN) | (1 << TWINT);	/* Enable TWI & clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status = TWSR & 0xF8;		
	/* Check for SLA+W transmitted &ack received */
	if(status == 0x18)		
	return 1;			/* Return 1 to indicate ack received */
	/* Check for SLA+W transmitted &nack received */
	if(status == 0x20)		
	return 2;			/* Return 2 to indicate nack received */
	else
	return 3;			/* Else return 3 to indicate SLA+W failed */
}

uint8_t I2C_Repeated_Start(char read_address)
{
	uint8_t status;		
	/* Enable TWI, generate start */
	TWCR = (1<<TWSTA) | (1 << TWEN) | (1 << TWINT);
	while(!(TWCR&(1<<TWINT)));
	/* Read TWI status register */
	status = TWSR & 0xF8;		
	/* Check for repeated start transmitted */
	if(status != 0x10)		
	return 0;			/* Return 0 for repeated start condition fail */
	/* Write SLA+R in TWI data register */
	TWDR = read_address;		
	TWCR = (1 << TWEN) | (1 << TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	
	/* Read TWI status register */
	status = TWSR & 0xF8;		
	/* Check for SLA+R transmitted &ack received */
	if(status == 0x40)		
	return 1;			/* Return 1 to indicate ack received */
	/* Check for SLA+R transmitted &nack received */
	if(status == 0x48)		
	return 2;			/* Return 2 to indicate nack received */
	else
	return 3;			/* Else return 3 to indicate SLA+W failed */
}

void I2C_Start_Wait(char write_address)						/* I2C start wait function */
{
	uint8_t status;											/* Declare variable */
	while (1)
	{
		TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				/* Enable TWI, generate start condition and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));						/* Wait until TWI finish its current job (start condition) */
		status = TWSR & 0xF8;								/* Read TWI status register with masking lower three bits */
		if (status != 0x08)									/* Check weather start condition transmitted successfully or not? */
		continue;											/* If no then continue with start loop again */
		TWDR = write_address;								/* If yes then write SLA+W in TWI data register */
		TWCR = (1<<TWEN)|(1<<TWINT);						/* Enable TWI and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));						/* Wait until TWI finish its current job (Write operation) */
		status = TWSR & 0xF8;								/* Read TWI status register with masking lower three bits */
		if (status != 0x18 )								/* Check weather SLA+W transmitted & ack received or not? */
		{
			I2C_Stop();										/* If not then generate stop condition */
			continue;										/* continue with start loop again */
		}
		break;												/* If yes then break loop */
	}
}

uint8_t I2C_Write(char data)
{
	uint8_t status;	
	TWDR = data;
	/* Enable TWI and clear interrupt flag */
	TWCR = (1 << TWEN) | (1 << TWINT);	
	while(!(TWCR&(1<<TWINT)));	
	/* Read TWI status register */
	status = TWSR & 0xF8;	
	/* Check for data transmitted &ack received */	
	if(status == 0x28)		
	return 0;			/* Return 0 to indicate ack received */
	/* Check for data transmitted &nack received */
	if(status==0x30)		
	return 1;			/* Return 1 to indicate nack received */
	else
	return 2;			/* Else return 2 for data transmission failure */
}

char I2C_Read_Ack()
{
	/* Enable TWI, generation of ack */
	TWCR |= (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	while(!(TWCR&(1<<TWINT)));	
	return TWDR;			
}

char I2C_Read_Nack()
{
	/* Enable TWI and clear interrupt flag */
	TWCR |= (1 << TWEN) | (1 << TWINT);	
	while(!(TWCR&(1<<TWINT)));
	return TWDR;
}

void I2C_Stop()
{
	/* Enable TWI, generate stop */
	TWCR |= (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
	while(TWCR&(1<<TWSTO));
}

