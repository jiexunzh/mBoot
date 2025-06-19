#ifndef BSP_RS485_H
#define BSP_RS485_H

#include <stdint.h>

#define RS485_DIR_TX_EN     gpio_bit_set(GPIOA,GPIO_PIN_1)
#define RS485_DIR_RX_EN     gpio_bit_reset(GPIOA,GPIO_PIN_1)

void bsp_rs485_uart_init(void);
void bsp_rs485_dir_init(void);

#endif /* BSP_RS485_H */
