#define F_CPU 16000000ul
#include "avr/io.h"
#include "util/delay.h"
#include "IRSend.h"

void TIMER_ENABLE_PWM(void)
{
	TCCR1B |= (1 << CS10);
}
void TIMER_DISABLE_PWM(void)
{
	TCCR1B &= ~(1 << CS10);
}

void TIMER_CONFIG_KHZ(unsigned int val)
{
	TCCR1A |= (1 << COM1B1) | (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	// F_clock/1=16mhz
	//F_pwm=16mhz/(ICR1 + 1)
	if (val == 35)
	{
		ICR1 = 457;
		OCR1B = 152;
	}
	if (val == 36)
	{
		ICR1 = 443;
		OCR1B = 148;
	}
	if (val == 38)
	{
		ICR1 = 420;
		OCR1B = 140;
	}
	if (val == 40)
	{
		ICR1 = 399;
		OCR1B = 133;
	}
	
}

void enableIROut(int khz)
{
	DDRD |= (1 << TIMER_PWM_PIN);	// set output
	PORTD &= ~(1 << TIMER_PWM_PIN); // When not sending PWM, we want it low
	TIMER_CONFIG_KHZ(khz);
}

void delayMicroseconds(unsigned int us)
{
	
	if (--us == 0)
	return;
	us <<= 2;
	us -= 2;
	__asm__ __volatile__ (
	"1: sbiw %0,1" "\n\t" // 2 cycles
	"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

void mark(int time)
{
	TIMER_ENABLE_PWM();
	delayMicroseconds(time);
}

void space(int time)
{
	// A space is no output, so the PWM output is disabled.
	TIMER_DISABLE_PWM();
	PORTD &= ~(1 << TIMER_PWM_PIN);
	delayMicroseconds(time);
}

void IRsendNEC(unsigned long data, int nbits)
{
	enableIROut(38);
	mark(NEC_HDR_MARK);
	PORTD &= ~(1 << TIMER_PWM_PIN);
	space(NEC_HDR_SPACE);
	for (int i = 0; i < nbits; i++) {
		if (data & TOPBIT) {
			mark(NEC_BIT_MARK);
			PORTD &= ~(1 << TIMER_PWM_PIN);
			space(NEC_ONE_SPACE);
		}
		else {
			mark(NEC_BIT_MARK);
			PORTD &= ~(1 << TIMER_PWM_PIN);
			space(NEC_ZERO_SPACE);
		}
		data <<= 1;
	}
	mark(NEC_BIT_MARK);
	PORTD &= ~(1 << TIMER_PWM_PIN);
	space(0);
}

void IRsendSony(unsigned long data, int nbits)
{
	enableIROut(40);
	mark(SONY_HDR_MARK);
	space(SONY_HDR_SPACE);
	data = data << (32 - nbits);
	for (int i = 0; i < nbits; i++) {
		if (data & TOPBIT) {
			mark(SONY_ONE_MARK);
			space(SONY_HDR_SPACE);
		}
		else {
			mark(SONY_ZERO_MARK);
			space(SONY_HDR_SPACE);
		}
		data <<= 1;
	}
}

void IRsendRC5(unsigned long data, int nbits)
{
	enableIROut(36);
	data = data << (32 - nbits);
	mark(RC5_T1); // First start bit
	space(RC5_T1); // Second start bit
	mark(RC5_T1); // Second start bit
	for (int i = 0; i < nbits; i++) {
		if (data & TOPBIT) {
			space(RC5_T1); // 1 is space, then mark
			mark(RC5_T1);
		}
		else {
			mark(RC5_T1);
			space(RC5_T1);
		}
		data <<= 1;
	}
	space(0); // Turn off at end
}

void IRsendSAMSUNG(unsigned long data, int nbits)
{
	enableIROut(38);
	mark(SAMSUNG_HDR_MARK);
	space(SAMSUNG_HDR_SPACE);
	for (int i = 0; i < nbits; i++) {
		if (data & TOPBIT) {
			mark(SAMSUNG_BIT_MARK);
			space(SAMSUNG_ONE_SPACE);
		}
		else {
			mark(SAMSUNG_BIT_MARK);
			space(SAMSUNG_ZERO_SPACE);
		}
		data <<= 1;
	}
	mark(SAMSUNG_BIT_MARK);
	space(0);
}