/**
 ******************************************************************************
 * @file    main.c
 * @brief   mBoot app full 的主函数入口
 *
 * @note    具备响应mBoot命令，主动从APP切换回Bootloader完整功能
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include "mBoot_app.h"
#include "bsp_systick.h"
#include "bsp_led.h"
#include "bsp_rs485.h"

uint32_t led_tick = 0;

int main(void)
{
    /* 设置中断向量表偏移 */
    set_nvic_vector_table_offset();
	
	SysTick_init();
	led_init();
	
    /* 初始化串口与RS485等 */
    mBoot_app_hardware_init();

    /* user code begin... */
	printf("APP running\r\n");

    while (1)
    {
        /* 当接收到以"\r\n"结尾的一行命令时，调用 mBoot_app_handle(cmd_buf) */
		if (newline_flag)
		{
			mBoot_app_handle(uart_buf);
			memset(uart_buf, 0, sizeof(uart_buf));
			uart_buf_i = 0;
			newline_flag = 0;
		}		
		
		if (nonBlock(led_tick, 500))
		{
			LED_Toggle();
			led_tick = get_tick();
		}
    }
}

