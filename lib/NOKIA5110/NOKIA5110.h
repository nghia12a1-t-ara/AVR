#ifndef NOKIA5110_H_
#define NOKIA5110_H_

void N5110_Cmnd(char DATA);
void N5110_Data(char *DATA);
void N5110_Reset();
void N5110_init();
void lcd_setXY(char x, char y);
void N5110_clear();
void N5110_image(const unsigned char *image_data)			

#endif