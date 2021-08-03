#ifndef IRSend_H_
#define IRSend_H_

#define TOPBIT 0x80000000

#define NEC_HDR_MARK	9000
#define NEC_HDR_SPACE	4500
#define NEC_BIT_MARK	560
#define NEC_ONE_SPACE	1600
#define NEC_ZERO_SPACE	560
/*#define NEC_RPT_SPACE	2250*/

#define SONY_HDR_MARK	2400
#define SONY_HDR_SPACE	600
#define SONY_ONE_MARK	1200
#define SONY_ZERO_MARK	600
#define SONY_RPT_LENGTH 	45000
#define SONY_DOUBLE_SPACE_USECS  500

#define RC5_T1		889
#define RC5_RPT_LENGTH	46000

#define SAMSUNG_HDR_MARK  5000
#define SAMSUNG_HDR_SPACE 5000
#define SAMSUNG_BIT_MARK  560
#define SAMSUNG_ONE_SPACE 1600
#define SAMSUNG_ZERO_SPACE  560
/*#define SAMSUNG_RPT_SPACE 2250*/


#define TIMER_PWM_PIN	4    //pind4

void TIMER_ENABLE_PWM(void);
void TIMER_DISABLE_PWM(void);
void TIMER_CONFIG_KHZ(unsigned int);
void enableIROut(int);
void delayMicroseconds(unsigned int);
void mark(int);
void space(int);
void IRsendNEC(unsigned long, int);
void IRsendSony(unsigned long, int);
void IRsendRC5(unsigned long, int);
void IRsendSAMSUNG(unsigned long, int);

#endif