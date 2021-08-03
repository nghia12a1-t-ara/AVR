/*
 * 18_6_IRsend.c
 *
 * Created: 18/06/2020 4:44:52 PM
 * Author : Admin
 */ 

#define F_CPU 16000000ul
#include <avr/io.h>
#include "util/delay.h"
#include "avr/interrupt.h"
#include "Keypad.h"
#include "IRSend.h"
#include "Sleep.h"

#define NEC_sec		!(PINC & (1 << 0))
#define SONY_sec	!(PINC & (1 << 1))
#define RC5_sec		!(PINC & (1 << 2))
#define SAMSUNG_sec !(PINC & (1 << 3))

void Mode_sec(void);
void Kiemtra(void);
void Kiemtra_NEC(void);
void Kiemtra_SONY(void);
void Kiemtra_RC5(void);
void Kiemtra_SAMS(void);

int NEC_nbits = 32, SONY_nbits = 12, 
	RC5_nbits = 12, SAMSUNG_nbits = 32;  
	
	 //frame_code: 0 -> 9, +, -, on/off
unsigned long NEC_Code[] = {0xFF6897, 0xFF30CF, 0xFF18E7, 0xFF7A85, 	0xFF10EF, 0xFF38C7, 0xFF5AA5, 0xFF42BD, 0xFF4AB5, 0xFF52AD, 
	0xFFE01F, 0xFFA857};
unsigned long SONY_Code[] = {0x910, 0x10, 0x810, 0x410, 0xC10, 0x210,
	0xA10, 0x610, 0xE10, 0x110, 0x90, 0x890, 0xA90};
unsigned long RC5_Code[] = {0x52, 0x69, 0xf1, 0x49, 0xd4, 0x1b, 0xa0, 0x15, 
	0x24, 0x97, 0x57, 0x4d, 0x7f};
unsigned long SAMSUNG_Code[] = {0xEE110707, 0xFB040707, 0xFA050707, 0xF9060707,
	0xF7080707, 0xF6090707, 0xF50A0707, 0xF30C0707, 0xF20D0707, 0xF10E0707};

char j = 0;
int sec;

int main(void)
{
	DDRC = 0x00;
	PORTC = 0x0F;
	DDRD |= (1 << TIMER_PWM_PIN);
	PORTD &= ~(1 << TIMER_PWM_PIN);
	
	GICR = 1<<INT0;					/* Enable INT0*/
	MCUCR = 1<<ISC01 | 1<<ISC00;	/* Trigger INT0 on rising edge */
	
	sei();							/* Enable Global Interrupt */
    
    while (1) 
    {
		setSleepModePowerDown();
		enterSleepMode();
		disableSleepMode();
		
		j = keyfind();
		Kiemtra();
    }
}

ISR (INT0_vect)
{
	Mode_sec();
}

void Mode_sec(void)
{
	if (NEC_sec)
	{
		sec = 0;
	}
	if (SONY_sec)
	{
		sec = 1;
	}
	if (RC5_sec)
	{
		sec = 2;
	}
	if (SAMSUNG_sec)
	{
		sec = 3;
	}
}

void Kiemtra()
{
	switch (sec)
	{
		case 0:
			Kiemtra_NEC();
			break;
		case 1:
			Kiemtra_SONY();
			break;
		case 2:
			Kiemtra_RC5();
			break;
		case 3:
			Kiemtra_SAMS();
			break;
	}
}

void Kiemtra_NEC()
{
	switch (j)
	{
		case ('0'):
			IRsendNEC(NEC_Code[0], NEC_nbits);
			break;
		case ('1'):
			IRsendNEC(NEC_Code[1], NEC_nbits);
			break;
		case ('2'):
			IRsendNEC(NEC_Code[2], NEC_nbits);
			break;
		case ('3'):
			IRsendNEC(NEC_Code[3], NEC_nbits);
			break;
		case ('4'):
			IRsendNEC(NEC_Code[4], NEC_nbits);
			break;
		case ('5'):
			IRsendNEC(NEC_Code[5], NEC_nbits);
			break;
		case ('6'):
			IRsendNEC(NEC_Code[6], NEC_nbits);
			break;
		case ('7'):
			IRsendNEC(NEC_Code[7], NEC_nbits);
			break;
		case ('8'):
			IRsendNEC(NEC_Code[8], NEC_nbits);
			break;
		case ('9'):
			IRsendNEC(NEC_Code[9], NEC_nbits);
			break;
	}
}

void Kiemtra_SONY()
{
	switch (j)
	{
		case ('0'):
		IRsendSony(SONY_Code[0], SONY_nbits);
		break;
		case ('1'):
		IRsendSony(SONY_Code[1], SONY_nbits);
		break;
		case ('2'):
		IRsendSony(SONY_Code[2], SONY_nbits);
		break;
		case ('3'):
		IRsendSony(SONY_Code[3], SONY_nbits);
		break;
		case ('4'):
		IRsendSony(SONY_Code[4], SONY_nbits);
		break;
		case ('5'):
		IRsendSony(SONY_Code[5], SONY_nbits);
		break;
		case ('6'):
		IRsendSony(SONY_Code[6], SONY_nbits);
		break;
		case ('7'):
		IRsendSony(SONY_Code[7], SONY_nbits);
		break;
		case ('8'):
		IRsendSony(SONY_Code[8], SONY_nbits);
		break;
		case ('9'):
		IRsendSony(SONY_Code[9], SONY_nbits);
		break;
	}
}

void Kiemtra_RC5()
{
	switch (j)
	{
		case ('0'):
		IRsendRC5(RC5_Code[0], SONY_nbits);
		break;
		case ('1'):
		IRsendRC5(RC5_Code[1], SONY_nbits);
		break;
		case ('2'):
		IRsendRC5(RC5_Code[2], RC5_nbits);
		break;
		case ('3'):
		IRsendRC5(RC5_Code[3], RC5_nbits);
		break;
		case ('4'):
		IRsendRC5(RC5_Code[4], RC5_nbits);
		break;
		case ('5'):
		IRsendRC5(RC5_Code[5], RC5_nbits);
		break;
		case ('6'):
		IRsendRC5(RC5_Code[6], RC5_nbits);
		break;
		case ('7'):
		IRsendRC5(RC5_Code[7], RC5_nbits);
		break;
		case ('8'):
		IRsendRC5(RC5_Code[8], RC5_nbits);
		break;
		case ('9'):
		IRsendRC5(RC5_Code[9], RC5_nbits);
		break;
	}
}

void Kiemtra_SAMS()
{
	switch (j)
	{
		case ('0'):
		IRsendSAMSUNG(SAMSUNG_Code[0], SAMSUNG_nbits);
		break;
		case ('1'):
		IRsendSAMSUNG(SAMSUNG_Code[1], SAMSUNG_nbits);
		break;
		case ('2'):
		IRsendSAMSUNG(SAMSUNG_Code[2], SAMSUNG_nbits);
		break;
		case ('3'):
		IRsendSAMSUNG(SAMSUNG_Code[3], SAMSUNG_nbits);
		break;
		case ('4'):
		IRsendSAMSUNG(SAMSUNG_Code[4], SAMSUNG_nbits);
		break;
		case ('5'):
		IRsendSAMSUNG(SAMSUNG_Code[5], SAMSUNG_nbits);
		break;
		case ('6'):
		IRsendSAMSUNG(SAMSUNG_Code[6], SAMSUNG_nbits);
		break;
		case ('7'):
		IRsendSAMSUNG(SAMSUNG_Code[7], SAMSUNG_nbits);
		break;
		case ('8'):
		IRsendSAMSUNG(SAMSUNG_Code[8], SAMSUNG_nbits);
		break;
		case ('9'):
		IRsendSAMSUNG(SAMSUNG_Code[9], SONY_nbits);
		break;
	}
}