#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "gd32c11x.h"

#define LED_RCU      RCU_GPIOB
#define LED_PROT  		GPIOB
#define LED_PIN  		GPIO_PIN_7
#define LED_ON			gpio_bit_reset(LED_PROT, LED_PIN);
#define LED_OFF		gpio_bit_set(LED_PROT, LED_PIN);
#define LED_Toggle     gpio_bit_write(LED_PROT, LED_PIN, (bit_status)!gpio_input_bit_get(LED_PROT, LED_PIN))

void led_init(void);

#endif /* __BSP_LED_H__ */
