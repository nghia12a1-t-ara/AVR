#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED1_PIN PORTB0
#define LED2_PIN PORTB2
#define LED3_PIN PORTB3
#define LED4_PIN PORTB4

#define IR_IN_PIN PORTB1
#define IR_IN_PORT PORTB
#define IR_OCR0A (122)

#define LOW (0)
#define HIGH (1)

#define IR_SUCCESS (0)
#define IR_ERROR (1)

#define IR_EVENT_IDLE (0)
#define IR_EVENT_INIT (1)
#define IR_EVENT_FINI (2)
#define IR_EVENT_PROC (3)

#define IR_PROTO_EVENT_INIT (0)
#define IR_PROTO_EVENT_DATA (1)
#define IR_PROTO_EVENT_FINI (2)
#define IR_PROTO_EVENT_HOOK (3)

volatile uint16_t IR_timeout = 0;
volatile uint16_t IR_counter = 0;
volatile uint32_t IR_rawdata = 0;

uint8_t IR_event = 0;
uint8_t IR_proto_event = 0;
uint8_t IR_index = 0;
uint32_t IR_data = 0;

static void IR_init()
{
	DDRB &= ~(1 << IR_IN_PIN); // set IR IN pin as INPUT
	PORTB &= ~(1 << IR_IN_PIN); // set LOW level to IR IN pin
	/*TCCR1A |= (1 << WGM01); // set timer counter mode to CTC*/
	/*TCCR1B |= (1 << CS00); // set prescaler to 1*/
	TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10);
	TIMSK |= (1 << OCIE0); // enable Timer COMPA interrupt
	OCR0 = IR_OCR0A; // set OCR0n to get ~38.222kHz timer frequency
	GICR = 1<<INT0;	 // enable INT0 interrupt handler
	MCUCR &= ~(1 << ISC01); // trigger INTO interrupt on raising
	MCUCR |= (1 << ISC00); // and falling edge
	sei(); // enable global interrupts
}

static int8_t IR_NEC_process(uint16_t counter, uint8_t value)
{
	int8_t retval = IR_ERROR;

	switch(IR_proto_event) 
	{
		case IR_PROTO_EVENT_INIT:
		/* expecting a space */
			if (value == HIGH) 
			{
				if (counter > 330 && counter < 360) 
				{
				/* a 4.5ms space for regular transmition of NEC Code;
				counter => 0.0045/(1.0/38222.0) * 2 = 344 (+/- 15) */
					IR_proto_event = IR_PROTO_EVENT_DATA;
					IR_data = IR_index = 0;
					retval = IR_SUCCESS;
				} 
				else if (counter > 155 && counter < 185) 
				{
					/* a 2.25ms space for NEC Code repeat;
					counter => 0.00225/(1.0/38222.0) * 2 = 172 (+/- 15) */
					IR_proto_event = IR_PROTO_EVENT_FINI;
					retval = IR_SUCCESS;
				}
			}
			break;
		case IR_PROTO_EVENT_DATA:
			/* Reading 4 octets (32bits) of data:
			1) the 8-bit address for the receiving device
			2) the 8-bit logical inverse of the address
			3) the 8-bit command
			4) the 8-bit logical inverse of the command
			Logical '0' ? a 562.5?s pulse burst followed by a 562.5?s
			(<90 IR counter cycles) space, with a total transmit time of 1.125ms
			Logical '1' ? a 562.5?s pulse burst followed by a 1.6875ms
			(>=90 IR counter cycles) space, with a total transmit time of 2.25ms */
			if (IR_index < 32) 
			{
				if (value == HIGH) 
				{
					IR_data |= ((uint32_t)((counter < 90) ? 0 : 1) << IR_index++);
					if (IR_index == 32) 
					{
						IR_proto_event = IR_PROTO_EVENT_HOOK;
					}
				}
				retval = IR_SUCCESS;
			}
			break;
		case IR_PROTO_EVENT_HOOK:
		/* expecting a final 562.5?s pulse burst to
		signify the end of message transmission */
			if (value == LOW) 
			{
				IR_proto_event = IR_PROTO_EVENT_FINI;
				retval = IR_SUCCESS;
			}
			break;
		case IR_PROTO_EVENT_FINI:
			/* copying data to volatile variable; raw data is ready */
			IR_rawdata = IR_data;
			break;
		default:
			break;
	}
	return retval;
}

static void IR_process()
{
	uint8_t value;
	uint16_t counter;

	/* load IR counter value to local variable, then reset counter */
	counter = IR_counter;
	IR_counter = 0;

	/* read IR_IN_PIN digital value
	(NOTE: logical inverse value = value ^ 1 due to sensor used) */
	value = (PINB & (1 << IR_IN_PIN)) > 0 ? LOW : HIGH;

	switch(IR_event) 
	{
		case IR_EVENT_IDLE:
			/* awaiting for an initial signal */
			if (value == HIGH) 
			{
				IR_event = IR_EVENT_INIT;
			}
			break;
		case IR_EVENT_INIT:
			/* consume leading pulse burst */
			if (value == LOW) 
			{
				if (counter > 655 && counter < 815) 
				{
					/* a 9ms leading pulse burst, NEC Infrared Transmission Protocol detected,
					counter = 0.009/(1.0/38222.) * 2 = 343.998 * 2 = 686 (+/- 30) */
					IR_event = IR_EVENT_PROC;
					IR_proto_event = IR_PROTO_EVENT_INIT;
					IR_timeout = 7400;
				}
				else 
				{
					IR_event = IR_EVENT_FINI;
				}
			}
			else 
			{
				IR_event = IR_EVENT_FINI;
			}
			break;
		case IR_EVENT_PROC:
			/* read and decode NEC Protocol data */
			if (IR_NEC_process(counter, value))
			{
				IR_event = IR_EVENT_FINI;
			}
			break;
		case IR_EVENT_FINI:
			/* clear timeout and set idle mode */
			IR_event = IR_EVENT_IDLE;
			IR_timeout = 0;
			break;
		default:
			break;
	}

}

static int8_t IR_read(uint8_t *address, uint8_t *command)
{
	if (!IR_rawdata)
	return IR_ERROR;

	*address = IR_rawdata;
	*command = IR_rawdata >> 16;
	IR_rawdata = 0;

	return IR_SUCCESS;
}

ISR(INT0_vect)
{
	IR_process();
}

ISR(TIMER1_COMPA_vect)
{
	/* When transmitting or receiving remote control codes using
	the NEC IR transmission protocol, the communications performs
	optimally when the carrier frequency (used for modulation/demodulation)
	is set to 38.222kHz. */
	if (IR_counter++ > 10000)
	IR_event = IR_EVENT_IDLE;
	if (IR_timeout && --IR_timeout == 0)
	IR_event = IR_EVENT_IDLE;
}

int main(void)
{
	uint8_t addr, cmd;

	/* setup */
	DDRB |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN) | (1 << LED4_PIN);
	
	IR_init();

	/* loop */
	while (1) 
	{
		if (IR_read(&addr, &cmd) == IR_SUCCESS) 
		{
			if (addr != 0x01)
			continue;
			switch(cmd) 
			{
				case 0x01:
				/* turn all LEDs off */
				PORTB &= ~((1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN) | (1 << LED4_PIN));
				break;
				case 0x00:
				PORTB ^= (1 << LED1_PIN); // toggle LED1
				break;
				case 0x07:
				PORTB ^= (1 << LED2_PIN); // toggle LED2
				break;
				case 0x06:
				PORTB ^= (1 << LED3_PIN); // toggle LED3
				break;
				case 0x04:
				PORTB ^= _BV(LED4_PIN); // toggle LED4
				break;
				default:
				break;
			};
		}
	}
}