#ifdef	UART_H_
#define	UART_H_

void UART_Init(void);
void UART_Send_Char(unsigned char c);
void UART_Send_Str(char *str);
char UART_Receive(void);

#endif