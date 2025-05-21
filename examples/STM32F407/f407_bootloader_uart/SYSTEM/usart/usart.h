#ifndef __USART_H
#define __USART_H

#include <stdint.h>

void uart_init(uint32_t bound);
void USARTSendByte(uint8_t c);;
uint8_t USART_string_send_finish(void);
uint8_t USARTRecvByte(uint8_t* c);
void clear_USART_RDR(void);


#endif
