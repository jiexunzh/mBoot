#include "bsp_systick.h"
#include "bsp_led.h"

int main(void)
{
	nvic_vector_table_set(0x08000000, 16 * 1024);
	SysTick_init();
	led_init();

    while (1)
	{
		LED_Toggle();
		delay_ms(500);
    }
}


