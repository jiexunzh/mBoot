#ifndef BSP_LED_H
#define BSP_LED_H

#define LED1_RCU	RCU_GPIOE
#define LED1_PORT	GPIOE
#define LED1_PIN	GPIO_PIN_4

#define LED1_OFF    gpio_bit_set(LED1_PORT, LED1_PIN)
#define LED1_ON     gpio_bit_reset(LED1_PORT, LED1_PIN)
#define LED1_Toggle  gpio_bit_write(LED1_PORT, LED1_PIN, (bit_status)!gpio_input_bit_get(LED1_PORT, LED1_PIN))

void led_init(void);

#endif /* BSP_LED_H */
