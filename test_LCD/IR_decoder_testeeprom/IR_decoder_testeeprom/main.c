/*
 * IR_decoder_testeeprom.c
 *
 * Created: 04/06/2020 2:46:38 PM
 * Author : Taarabt
 */ 

// Define your IR port and IR pin
#define IR_PIN PIND
#define IR_PIN_NUM PIND2
#define IR_PORT PORTD
#define IR_ENABLE_PULLUP 1

// Define pin to start computer - pull-down
#define START_PORT	PORTB
#define START_DDR	DDRB
#define START_PIN	PINB1

// Define repeat rate of long-hold buttons
// This is actually prescale value
#define NEC_REPEAT_RATE 5
#define RC5_REPEAT_RATE 5
#define SIRP_REPEAT_RATE 5

// UART Baudrate
#define USART_BAUD 9600
#define USART_UBBR_VALUE ((F_CPU/(USART_BAUD<<4))-1)

// only 8 for now
#define TIMER_PRESCALER 8


#if TIMER_PRESCALER == 8
#define TIMER1_PRESCALER (1 << CS11)
#endif

// Maximal time is about 47407us = 47,407ms !
// 47407 => 65535 cycles, the timer is 16bit so maximum
// value is 65535
#define F_CPU 16000000UL
#define CONV(x) ((F_CPU/TIMER_PRESCALER)/(1000000/x))


#define IR_HIGH  (IR_PIN & (1 << IR_PIN_NUM))
#define IR_LOW (!(IR_PIN & (1 << IR_PIN_NUM)))
#define IR_VAL (IR_PIN & (1 << IR_PIN_NUM))


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "avr/eeprom.h"
#include "util/delay.h"

// NEC code for LG remote and unused SimpLink button
uint16_t EEMEM eeSavedCode = 0x207E;

unsigned char repeatCount = 0;

void uartInit(void)
{
	// Set baud rate
	UBRRH = (uint8_t)(USART_UBBR_VALUE>>8);
	UBRRL = (uint8_t)USART_UBBR_VALUE;

	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSRC = (1 << URSEL) | (1 << USBS) | (1 << UCSZ0);

	// Enable receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
}

void uartSend(uint8_t u8Data)
{
	while((UCSRA & (1 << UDRE)) == 0);
	UDR = u8Data;
}

void uartSendNibble(unsigned char nibble)
{
	if(nibble < 10)
	uartSend(nibble + '0');
	else
	uartSend(nibble - 10 + 'A');
}
 
void uartSendCode(unsigned int code)
{
	// print "0x"
	uartSend('0');
	uartSend('x');
	// print 4 HEX digits
	uartSendNibble(code >> 12);
	uartSendNibble((code >> 8) & 0x0f);
	uartSendNibble((code >> 4) & 0x0f);
	uartSendNibble(code  & 0x0f);

	// CR
	uartSend(13);
	// LF
	uartSend(10);
}

// The input IR code is matching
// the learned code
void codeMatch(unsigned int code)
{
	// pull-down the pin connected to the
	// computer's power on button
	START_DDR |= (1 << START_PIN);

	// Debug LED on
	PORTC = 255;

	// Wait 40ms
	TCNT1 = 0;
	while(TCNT1 < 40000);

	// Hi-Z again
	START_DDR &= ~(1 << START_PIN);

	// Debug LED off
	PORTC = 0;
}

void init()
{
	// Initialize timer's prescaler
	TCCR1B = TIMER1_PRESCALER;

	// Enable pull-up on IR pin
	if(IR_ENABLE_PULLUP)
	IR_PORT |= (1 << IR_PIN_NUM);

	// Jumper pull-up
	PORTB |= (1 << PORTB0);

	// Debug LED output - all pins on PORTC
	DDRC = 255;
	uartInit();
}

unsigned int protocolSIRP(unsigned int code)
{
	unsigned char bitVal;
	unsigned int time;
	unsigned char i;

	static unsigned int lastCode = 0;

	code = 0;

	// Read 32 data bits
	for(i = 0; i < 12; i++)
	{
		
		while(IR_LOW);
		while(IR_HIGH);

		time = TCNT1;
		TCNT1 = 0;
		
		// 1440 us
		if(time > CONV(1460))
		{
			bitVal = 1;
		} 
		else 
		{
			bitVal = 0;
		}
		
		code = code << 1;
		code |= bitVal;
	}
	if(code == lastCode)
	{
		if((repeatCount++) == SIRP_REPEAT_RATE)
		{
			repeatCount = 0;
			return code;
		} 
		else 
		{
			return 0;
		}
	}
	lastCode = code;
	return code;
}

unsigned int protocolRC5(unsigned int code)
{
	unsigned char repeatBit;
	unsigned char i;

	static unsigned int lastCode = 0;
	static unsigned char lastRepeatBit = 0;

	code = 0;

	// 3160 us offset from the first pulse
	while(TCNT1 < CONV(3160));
	TCNT1 = 0;

	// Read "repeat" bit
	repeatBit = IR_VAL;

	// Move 1760us to the first data bit
	while(TCNT1 < CONV(1760));
	TCNT1 = 0;

	// Read 12 data bits (5 address & 7 command)
	for(i = 0; i < 12; i++)
	{

		code = code << 1;

		if(IR_VAL)
		code |= 0x0001;

		while(TCNT1 < CONV(1760));
		TCNT1 = 0;

	}

	if(code == lastCode && repeatBit == lastRepeatBit)
	{
		if(++repeatCount == RC5_REPEAT_RATE)
		{
			repeatCount = 0;
			return code;
		} 
		else 
		{
			return 0;
		}
	}
	if(repeatBit != lastRepeatBit)
	{
		repeatCount = 0;
	}
	lastCode = code;
	lastRepeatBit = repeatBit;
	return code;
}

unsigned int protocolNEC(unsigned int code)
{

	unsigned char bitVal;
	unsigned int time;
	unsigned char i;

	static unsigned int lastCode = 0;

	//TCNT1 = 0;
	while(IR_HIGH);

	time = TCNT1;
	TCNT1 = 0;

	// 4200 us
	if(time > CONV(4200))
	{
		// regular button press
		repeatCount = 0;
	} 
	else 
	{
		// hold button press
		// send last keycode
		if(++repeatCount == NEC_REPEAT_RATE)
		{
			repeatCount = 0;
			return lastCode;
		}
		return 0;
	}
	code = 0;
	
	// Read 32 data bits
	for(i = 0; i < 32; i++)
	{
		while(IR_LOW);
		while(IR_HIGH);

		time = TCNT1;
		TCNT1 = 0;
		
		// 1650 us
		if(time > CONV(1650))
		{
			bitVal = 1;
		} 
		else 
		{
			bitVal = 0;
		}
		if((i < 8) || (i >= 16 && i < 24))
		{
			code = code << 1;
			code |= bitVal;
		}
	}
	lastCode = code;
	return code;
}

int main(void)
{

	unsigned int savedCode;
	unsigned int code = 0;
	unsigned int time;

	unsigned char protocolLetter;
	
	init();
	

	savedCode = eeprom_read_word(&eeSavedCode);

	while(1)
	{	
		PORTC = 0x00;
		_delay_ms(500);
		PORTC = 0xFF;
		_delay_ms(500);
		while(IR_HIGH);
		TCNT1 = 0;
		while(IR_LOW);
		
		time = TCNT1;
		
		TCNT1 = 0;

		// Initial pulse length between 9ms and 9.3ms => NEC protocol
		if(time > CONV(9000) && time < CONV(9300)) {
			protocolLetter = 'N';
			code = protocolNEC(code);

			// Initial pulse length between 0.8ms and 1.2ms => RC5 protocol
			} else if(time > CONV(800) && time < CONV(1200)) {
			protocolLetter = 'R';
			code = protocolRC5(code);

			// Initial pulse length between 2ms and 2.8ms => SIRP protocol
			} else if(time > CONV(2000) && time < CONV(2800)) {
			protocolLetter = 'S';
			code = protocolSIRP(code);

			// Unkwnown initial pulse length
			} else {
			continue;
		}

		// If code is wrong, we get 0
		if(code == 0)
		{
			continue;
		}
		// Learning mode
		if((PINB & (1 << PINB0)) == 0)
		{
			savedCode = code;
			eeprom_write_word(&eeSavedCode, savedCode);
			continue;
		}

		// Code match
		if(code == savedCode)
		{
			codeMatch(code);
		}

		// Send protocol letter and code over UART
		uartSend(protocolLetter);
		uartSendCode(code);
	}
	return 0;
}