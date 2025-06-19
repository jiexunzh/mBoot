#include "bsp_led.h"

void led_init(void)
{
    rcu_periph_clock_enable(LED_RCU);
    gpio_init(LED_PROT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_PIN);
    LED_OFF();
}
