#include <avr/io.h>
#include "Sleep.h"

static void setSleepModePowerDown()
{
	MCUCR &= ~(1<<SM2);
	MCUCR |= (1<<SM1);
	MCUCR &= ~(1<<SM0);
}

static void enterSleepMode()
{
	MCUCR |= (1<<SE);
	asm volatile("sleep");
}

static void disableSleepMode()
{
	MCUCR &= ~(1<<SE);
}