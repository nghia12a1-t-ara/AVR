/*
 * IR_Decoder_ATMega08.c
 *
 * Created: 26/05/2020 7:05:19 AM
 * Author : Admin
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "util/delay.h"
#include "avr/interrupt.h"
#include "LCD_4.c"

#define DATA_INFR PD6
#define RELAY1 PD4
#define RELAY2 PD5

unsigned char check_start();
unsigned long int infr_sensor();

int main(void)
{
unsigned char Lcd_Buffer[20];          
unsigned long int Data;      // Ma cac kenh cua remote              
// Declare your local variables here

// Input/Output Ports initialization
// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
DDRB = 0x00;
PORTB = 0x10;

// Port C initialization
// Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
// State6=T State5=T State4=T State3=T State2=T State1=T State0=T
DDRC = 0xff;
PORTC = 0xff;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
DDRD = 0x0f;
PORTD = 0x00;

// Alphanumeric LCD initialization
// Connections specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTB Bit 1
// RD - PORTB Bit 2
// EN - PORTB Bit 3
// D4 - PORTC Bit 2
// D5 - PORTC Bit 3
// D6 - PORTC Bit 4
// D7 - PORTC Bit 5
// Characters/line: 16

RELAY1 = RELAY2 = 1;

/*
lcd_init(16);                // Ham khoi tao LCD1602
lcd_clear();                  // Ham xoa LCD1602
lcd_gotoxy(0,0);              // Di chuyen con tro den vi tri (0,0)
*/

while (1)
      {
          Data = infr_sensor();    // Gan ma cua kenh vao bien Data
/*
	//Ham hien thi gia tri ma cua cac kenh len LCD1602
    if(Data!=0)
    {
          lcd_gotoxy(0,1)
          sprintf(Lcd_Buffer,"Ma Kenh: %lu",Data);
          lcd_puts(Lcd_Buffer);
    }
*/


// Ham dieu khien RELAY ========================================================  
          if(Data!=0)              // Neu co kenh tren remote duoc an  
          {
              if(Data == OF_QUAT) // Thuc hien lenh dieu khien ung voi moi phim
              {  
                    RELAY1 =~RELAY1;      
              }
                if((Data == QUAY_QUAT)|(Data == QUAY_QUAT1))
              {        
                  RELAY2 = ~RELAY2;
              }
            
          }
        
      }
}

// Ham check xung START
/* Xung START là xung co:
- tin hieu o muc thap (0) có do dai 9ms
- tin hieu o muc cao (1) có do dai 4ms
Chi kiem tra tin hieu o muc thap dung 9ms la dung xung START
*/
unsigned char check_start()
{
	unsigned char i;
	if(DATA_INFR==0)              // Neu co canh xuong cua tin hieu
	{
		delay_us(200);          // Delay de chong nhieu
		if(DATA_INFR == 0)        // Neu dung la co canh xuong cua tin hieu
		{
			for(i = 0; i < 40; i++)  // Chia nho xung START thanh 40 doan, moi doan dai 200us
			{
				delay_us(200);
				if(DATA_INFR == 1) return 0;    // Neu tin hieu len muc cao (1) --> khong phai xung START
			}
			while(DATA_INFR == 1);
			return 1;          // Dung la xung start
		}

	}
}

// ===============================================================
//  Ham doc ma cua cac kenh
unsigned long int infr_sensor()
{
	unsigned char i;
	unsigned int Data_Infr;
	Data_Infr = 0;
	if(check_start())                  // Neu dung xung START
	{
		while(DATA_INFR == 1);          // Cho het bit 1 tiep theo
		while(DATA_INFR == 0);          // Cho het bit 0 tiep theo
		for(i = 0; i < 24; i++)            // Lay 24 bit du lieu (bao gom 8 bit 0 va 8 bit 1 dau tien sau xung start)
		{
			//while(DATA_INFR==0);
			delay_us(750);          // Delay 750us de kiem tra bit
			if(DATA_INFR == 0) Data_Infr = Data_Infr*2 + 1;
			if(DATA_INFR == 1) Data_Infr = Data_Infr*2;
			while(DATA_INFR == 1);    // cho canh len
			while(DATA_INFR == 0);    // cho canh len
		}
		delay_ms(600);                // Cho het tin hieu (chong nhieu)
	}
	return Data_Infr;                  // Gia tri ma kenh tra ve
}