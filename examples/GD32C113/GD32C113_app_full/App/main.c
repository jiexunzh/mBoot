#include <stdio.h>
#include <string.h>
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_rs485.h"
#include "iap_config.h"
#include "iap_app.h"

uint32_t led_tick = 0;

int main(void)
{
	nvic_vector_table_set(0x08000000, ApplicationAddress - 0x08000000);
	SysTick_init();
	led_init();
	bsp_rs485_dir_init();
	bsp_rs485_uart_init();
	printf("APP running\r\n");
	
    while (1)
	{
		if (nonBlock(led_tick, 500))
		{
			LED_Toggle();
			led_tick = get_tick();
		}
		
		if (newline_flag)
		{
			IAP_Handle(uart_buf);
			memset(uart_buf, 0, sizeof(uart_buf));
			uart_buf_i = 0;
			newline_flag = 0;
		}
    }
}
