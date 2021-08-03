#ifdef	UART_H_
#define	UART_H_

void UART_Init(void);
void UART_Write_Chr(unsigned char c);
void UART_Write_Str(char *str);
unsigned char UART_Read(void);

#endif