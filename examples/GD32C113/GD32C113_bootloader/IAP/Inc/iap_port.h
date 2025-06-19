#ifndef IAP_PORT_H
#define IAP_PORT_H

#include <stdint.h>
#include "gd32c11x.h"

/**** UART port ****/
void iap_uart_init(void);
void iap_uart_transmit_byte(uint8_t data);
uint8_t is_iap_uart_transmit_complete(void);
uint8_t is_iap_uart_receive_notEmpty(void);
uint8_t iap_uart_receive_byte(void);

/**** flash port ****/
void iap_write_flash(uint32_t WriteAddr, uint32_t* pBuffer, uint32_t NumToWrite);
void iap_read_flash(uint32_t ReadAddr, uint32_t* pBuffer, uint32_t NumToRead);

/**** delay port ****/
void iap_delay_us(uint32_t us);

/**** timer port if use listen update command function when MCU power on ****/
void iap_timer_init(void);
uint32_t iap_get_timer_counter(void);

/**** rs485 port if use rs485 ****/
void iap_rs485_dir_init(void);
void iap_rs485_dir_tx(void);
void iap_rs485_dir_rx(void);

#endif /* IAP_PORT_H */
