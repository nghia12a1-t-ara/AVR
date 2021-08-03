/*
 * Nhung_CBquangdien.c
 *
 * Created: 22/06/2020 11:51:05 PM
 * Author : Admin
 */ 
#define F_CPU 16000000ul
#include <avr/io.h>
#include "util/delay.h"
#include "avr/interrupt.h"
#include "stdlib.h"
#include "LCD_4.h"
#include "PORT_4.h"

#define IR_Out_Pin 4

void Timer_pwm_init(void);

int count = -1;
char buffer[10];

int main(void)
{
	DDRD |= (1 << IR_Out_Pin);
	DDRD &= ~(1 << 2);
	PORTD &= ~(1 << 2);
	
 	LCD_Init();
	Timer_pwm_init();
	GICR |= (1 << INT0);		/* Enable INT0*/
	MCUCR |= (1 << ISC01) | (1 << ISC00);  /* Trigger INT0 on rising edge */
	
	sei();
	
	LCD_String_xy(0, 0, "Tong San Pham:");
    /* Replace with your application code */
    while (1) 
    {
    }
}

// ISR (INT0_vect)
// {
// 	count++;
// 	itoa(count, buffer, 10);
// 	LCD_String_xy(1, 0, buffer);
// }

void Timer_pwm_init(void)
{
	TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10);  // chia 1
	ICR1 = 420;
	OCR1B = 140;
}
