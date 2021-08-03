#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "LCD_4.h"
#include "PORT_4.h"

#define degree_sysmbol 0xdf

void ADC_Init(){
	DDRA &= ~(1 << 0);	     /*ADC0 is input */
	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);  /* Enable ADC, with freq/128  */
	ADMUX |= (1 << REFS0);           /* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)		//channel = 1
{
	ADMUX = 0x40 | (channel & 0x07);   /* set input channel to read */
	ADCSRA |= (1<<ADSC);               /* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));     /* Wait until end of conversion by polling ADC interrupt flag */
	ADCSRA |= (1<<ADIF);               /* Clear interrupt flag */
	_delay_ms(1);                      /* Wait a little bit */
	return ADCW;                       /* Return ADC word */
}


int main()
{
	char Temperature[10];
	float celsius;

	LCD_Init();                 /* initialize 16x2 LCD*/
	LCD_Clear();
	ADC_Init();                 /* initialize ADC*/
	
	while(1)
	{
		LCD_String_xy(1, 0, "Temperature");
		celsius = (ADC_Read(1)*4.88);
		celsius = (celsius/10.00);
		sprintf(Temperature, "%d%cC  ", (int)celsius, degree_sysmbol);/* convert integer value to ASCII string */
		LCD_String_xy(2, 0, Temperature);/* send string data for printing */
		_delay_ms(1000);
	}
}