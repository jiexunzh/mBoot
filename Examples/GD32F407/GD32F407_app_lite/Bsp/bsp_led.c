#include "bsp_led.h"
#include "gd32f4xx.h"

void led_init(void)
{
    /* enable the LED GPIO clock */
    rcu_periph_clock_enable(LED1_RCU);

    /* configure LED GPIO port */
    gpio_mode_set(LED1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_PIN);
    gpio_output_options_set(LED1_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_PIN);
	
    /* reset LED GPIO pin */
    LED1_OFF;
}
