#include "Keypad.h"

#include <mega16.h>
#include <delay.h>
#include <alcd.h>
#include <eeprom.h>
#include <string.h>          

#ifndef S_CHANGE_PASSWORD
#define     S_CHANGE_PASSWORD       2
#endif


uint8_t keypad[4][3] = {'1','2','3','4','5','6','7','8','9','*','0','#'};
uint8_t pass_md[6]={'1','1','1','1','1','1'};
uint8_t pass[6];
uint8_t pass1[6];
uint8_t pass2[6];
uint8_t mk[6];
uint8_t oldpass[6];
extern volatile uint8_t status;

void Assign_Pass(uint8_t nguon[6], uint8_t dich[6])
{
	int i;
	for(i = 0; i < 6;i++)
    {
		nguon[i] = dich[i];
	}
}

uint8_t keypass(void)
{
    unsigned char c, r;
    if(PIN_KEYPAD != 0x0F)
    {
        for(c=0; c<3; c++)
        {
            PORT_KEYPAD = ~(0x01<<(4+c));
            delay_ms(1);
            for(r=0; r<4; r++)
            {
                if((PIN_KEYPAD & (0x01 << r)) == 0)
                {
                    while( (PIN_KEYPAD & (0x01<<r)) == 0 );
                    return keypad[r][c]; 
                }
            }
        }
    }
    return 0;
}

uint8_t check(uint8_t a[], uint8_t b[])
{
	unsigned char i;
	for(i=0; i<6; i++){
		if(a[i] != b[i])
        {
            return 0;
        }
	}
	return 1;
}

void Enter_Pass(void)
{
    uint8_t i = 0, key = 0;
    // P2_7 = 0; LED
    PORTA.2 = 0;
    
    lcd_gotoxy(0,0);
    lcd_puts("ENTER PASSWORD:");
    lcd_gotoxy(0,1);
  
    i=0;
    while( i < 6 )
    {
        key = keypass();
        if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0')
        {
            lcd_gotoxy(i, 1);
            lcd_putchar(key);
            mk[i] = key;
            key = 0;
            i++;
        }
        if(key == '*' && i > 0)
        {
            lcd_gotoxy(i-1, 1);
            lcd_putchar(' ');
            i--;
        }
        if (status == S_CHANGE_PASSWORD)
        {
            break;
        }
    }
}

void Change_Password(void)
{
    uint8_t key = 0, j = 0, k = 0, h = 0;
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_puts("Change pass!");
    delay_ms(500);
    do
    {
        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_puts("Old pass?");
        lcd_gotoxy(0,1);

        j = 0;
        while(j < 6)
        {
            key = keypass();
            if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0')
            {
                lcd_gotoxy(j, 1);
                lcd_putchar(key);
                oldpass[j] = key;
                key = 0;
                j++;
            }
            if(key == '*' && j > 0){
                lcd_gotoxy(j-1, 1);
                lcd_putchar(' ');
                j--;
            }
        }
        // Read_PASS_From_EEP(pass); 
        eeprom_read_block(pass, 0, strlen(pass));
    }while( !check(oldpass, pass) );

    do{
        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_puts("New pass?");
        lcd_gotoxy(0, 1);
        k = 0;
        while(k < 6)
        {
            key = keypass();
            if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0'){
                lcd_gotoxy(k, 1);
                lcd_putchar(key);
                pass1[k] = key;
                key = 0;
                k++;
            }
            if(key == '*' && k > 0){
                lcd_gotoxy(k-1, 1);
                lcd_putchar(' ');
                k--;
            }
        }

        lcd_clear();
        lcd_gotoxy(0,0);
        lcd_puts("Confirm Pass?");
        lcd_gotoxy(0, 1);
        h = 0;
        while (h < 6)
        {
            key = keypass();
            if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0')
            {
                lcd_gotoxy(h, 1);
                lcd_putchar(key);
                pass2[h] = key;
                key=0;
                h++;
            }
            if(key == '*' && h > 0){
                lcd_gotoxy(h-1, 1);
                lcd_putchar(' ');
                h--;
            }
        }
    } while( !check(pass1, pass2) );

    // Write_PASS_To_EEP(pass1);
    eeprom_write_block(pass1, 0, strlen(pass1));
                
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_puts("Pass changed!");
}                
                

