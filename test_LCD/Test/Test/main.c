#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "stdlib.h"
#include "LCD.h"

#define degree_sysmbol 0xdf

void ADC_Init(){
	DDRA = 0x00;	        /* Make ADC port as input */
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0) ;          /* Enable ADC, with freq/128  */
	ADMUX |= (1 << REFS0);           /* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)
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
	uint16_t adc_result0;
	char buffer[10];
	int temp;

	LCD_Init();                 /* initialize 16x2 LCD*/
	ADC_Init();                 /* initialize ADC*/
	
	while(1)
	{
		adc_result0 = ADC_Read(0); // read adc value at PA0
		temp = adc_result0/2.01; // finding the temperature
		itoa(temp,buffer,10);
		LCD_String("Temp="); //display temperature
		LCD_String(buffer);
// 		LCD_String_xy(1,0,"Temperature");
// 		celsius = (ADC_Read(0)*9.4);
// 		celsius = (celsius/10.00);
// 		sprintf(Temperature,"%d%cC  ", (int)celsius, degree_sysmbol);/* convert integer value to ASCII string */
// 		LCD_String_xy(2,0,Temperature);/* send string data for printing */
// 		_delay_ms(1000);
// 		memset(Temperature,0,10);
	}
}