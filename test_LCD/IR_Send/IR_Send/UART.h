#ifdef	UART_H_
#define	UART_H_

void UART_Init(long USART_BAUDRATE);
unsigned char UART_RxChar();
void UART_TxChar(char ch);
void UART_SendString(char *str);

#endif