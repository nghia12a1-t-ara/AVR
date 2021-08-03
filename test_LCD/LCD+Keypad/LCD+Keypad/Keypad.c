#include <avr/io.h>
#include <util/delay.h>
#include "Keypad.h"

#define KEY_PRT 	PORTA
#define KEY_DDR		DDRA
#define KEY_PIN		PINA

unsigned char keypad[4][4] = {	{'A','8','9','/'},
								{'4','5','6','*'},
								{'1','2','3','-'},
								{' ','0','=','+'}};

unsigned char colloc, rowloc;

char keyfind()
{
	while(1)
	{
		KEY_DDR = 0xF0;           /* set port direction as input-output */
		KEY_PRT = 0xFF;

		do
		{
			KEY_PRT &= 0x0F;      /* mask PORT for column read only */
			asm("NOP");
			colloc = (KEY_PIN & 0x0F); /* read status of column */
		}while(colloc != 0x0F);
		
		/* debounce if a button is pressed */
		do
		{
			do
			{
				_delay_ms(20);             /* 20ms key debounce time */
				colloc = (KEY_PIN & 0x0F); /* read status of column */
			}while(colloc == 0x0F);        /* check for any key press */
			_delay_ms (40);	            /* 20 ms key debounce time */
			colloc = (KEY_PIN & 0x0F);
		}while(colloc == 0x0F);

		/* now check for rows */
		
		/* check for pressed key in 1st row */
		KEY_PRT = 0xEF;   // 0xEF = 1110.1111         
		asm("NOP");
		colloc = (KEY_PIN & 0x0F);
		if(colloc != 0x0F)
		{
			rowloc = 0;
			break;
		}
		
		/* check for pressed key in 2nd row */
		KEY_PRT = 0xDF;		// OxDF = 0b1101.1111
		asm("NOP");
		colloc = (KEY_PIN & 0x0F);
		if(colloc != 0x0F)
		{
			rowloc = 1;
			break;
		}
		
		/* check for pressed key in 3rd row */
		KEY_PRT = 0xBF;		
		asm("NOP");
		colloc = (KEY_PIN & 0x0F);
		if(colloc != 0x0F)
		{
			rowloc = 2;
			break;
		}

		/* check for pressed key in 4th row */
		KEY_PRT = 0x7F;		
		asm("NOP");
		colloc = (KEY_PIN & 0x0F);
		if(colloc != 0x0F)
		{
			rowloc = 3;
			break;
		}
	}
	
	/* now check for collum */
	if(colloc == 0x0E)    //0b0000.1110
		{
			return(keypad[rowloc][0]);
		}
	else if(colloc == 0x0D)
		{
			return(keypad[rowloc][1]);
		}
	else if(colloc == 0x0B)
		{
			return(keypad[rowloc][2]);
		}
	else
		{
			return(keypad[rowloc][3]);
		}
}

