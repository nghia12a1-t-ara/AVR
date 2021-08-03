enableIROut(int khz)
{
	TIMER_DISABLE_INTR;	
	DDRD |= (1 << TIMER_PWM_PIN);
	PORTD &= (1 << TIMER_PWM_PIN); // When not sending PWM, we want it low
	TIMER_CONFIG_KHZ(khz);
}

void mark(int time) 
{
	// Sends an IR mark for the specified number of microseconds.
	// The mark output is modulated at the PWM frequency.
	TIMER_ENABLE_PWM; // Enable pin 3 PWM output
	_delay_us(time);
}

void space(int time) {
	// Sends an IR space for the specified number of microseconds.
	// A space is no output, so the PWM output is disabled.
	TIMER_DISABLE_PWM; // Disable pin 3 PWM output
	_delay_us(time);
}

void IRsendNEC(unsigned long data, int nbits)
{
	enableIROut(38);
	mark(NEC_HDR_MARK);
	space(NEC_HDR_SPACE);
	for (int i = 0; i < nbits; i++) {
		if (data & TOPBIT) {
			mark(NEC_BIT_MARK);
			space(NEC_ONE_SPACE);
		}
		else {
			mark(NEC_BIT_MARK);
			space(NEC_ZERO_SPACE);
		}
		data <<= 1;
	}
	mark(NEC_BIT_MARK);
	space(0);
}