#ifndef BSP_USART_DEBUG_H
#define BSP_USART_DEBUG_H

#include <stdint.h>

#define RS485_DEBUG_TX_EN   gpio_bit_set(GPIOA, GPIO_PIN_8)
#define RS485_DEBUG_RX_EN   gpio_bit_reset(GPIOA, GPIO_PIN_8)

void RS485_debug_dir_init(void);
void usart_debug_init(uint32_t bound);

#endif /* BSP_USART_DEBUG_H */
