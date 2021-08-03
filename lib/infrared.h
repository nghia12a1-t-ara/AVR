#define MARK  0
#define SPACE 1

#define TOPBIT 0x80000000

#define NEC_HDR_MARK	9000
#define NEC_HDR_SPACE	4500
#define NEC_BIT_MARK	560
#define NEC_ONE_SPACE	1600
#define NEC_ZERO_SPACE	560
#define NEC_RPT_SPACE	2250

#define SONY_HDR_MARK	2400
#define SONY_HDR_SPACE	600
#define SONY_ONE_MARK	1200
#define SONY_ZERO_MARK	600
#define SONY_RPT_LENGTH 45000
#define SONY_DOUBLE_SPACE_USECS  500

#define TIMER_ENABLE_PWM     (TCCR1A |= (1 << COM1B1) | (1 << COM1A1))
#define TIMER_DISABLE_PWM    (TCCR1A &= ~((1 << COM1B1) & (1 << COM1A1))

#define TIMER_DISABLE_INTR   (TIMSK = 0)

#define TIMER_PWM_PIN 4    //pind4

#define TIMER_CONFIG_KHZ(int val) ({ 
	const uint8_t pwmval = F_CPU / 2000 / (val); 
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10);
	OCR1A = pwmval; 
	OCR1B = pwmval / 3; 
})

