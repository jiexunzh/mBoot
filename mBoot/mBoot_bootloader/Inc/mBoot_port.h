#ifndef MBOOT_PORT_H
#define MBOOT_PORT_H

#include <stdint.h>
// #include "gd32c11x.h"

/**** UART port ****/
void mBoot_uart_init(void);
void mBoot_uart_transmit_byte(uint8_t data);
uint8_t is_mBoot_uart_transmit_complete(void);
uint8_t is_mBoot_uart_receive_notEmpty(void);
uint8_t mBoot_uart_receive_byte(void);

/**** flash port ****/
void mBoot_write_flash(uint32_t WriteAddr, uint32_t* pBuffer, uint32_t NumToWrite);
void mBoot_read_flash(uint32_t ReadAddr, uint32_t* pBuffer, uint32_t NumToRead);

/**** delay port ****/
void mBoot_delay_us(uint32_t us);

/**** timer port if mBoot use listen update command function when MCU power on ****/
void mBoot_timer_init(void);
uint32_t mBoot_get_timer_counter(void);

/**** rs485 port if mBoot use rs485 ****/
void mBoot_rs485_dir_init(void);
void mBoot_rs485_dir_tx(void);
void mBoot_rs485_dir_rx(void);

#endif /* MBOOT_PORT_H */
