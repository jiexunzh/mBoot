#include "bsp_rs485.h"
#include "gd32c11x.h"
#include <stdio.h>
#include <string.h>

uint8_t uart_buf[UART_BUF_SIZE] = {0};
uint16_t uart_buf_i = 0;
uint8_t newline_flag = 0;

void bsp_rs485_uart_init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
	
	nvic_irq_enable(USART1_IRQn, 0, 0);
	usart_interrupt_enable(USART1, USART_INT_RBNE);
	usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);	// ·ÀÖ¹Îó´¥·¢
    usart_enable(USART1);
}

void bsp_rs485_dir_init(void)
{
    /* enable the LED1 GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure LED1 GPIO port */ 
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
	RS485_DIR_RX_EN;
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
	RS485_DIR_TX_EN;
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
	RS485_DIR_RX_EN;
    return ch;
}

void USART1_IRQHandler(void)
{
	uint8_t data;
	
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE)) 
	{
		data = usart_data_receive(USART1);
		
		// ·ÀÖ¹ÓëIAP_Handle()³åÍ»
		if (newline_flag == 0)
		{
			if (uart_buf_i >= UART_BUF_SIZE)
			{
				memset(uart_buf, 0, sizeof(uart_buf));
				uart_buf_i = 0;
			}		
			uart_buf[uart_buf_i] = data;
			
			if (uart_buf_i > 0 && uart_buf[uart_buf_i] == 0x0A)
			{
				if (uart_buf[uart_buf_i - 1] == 0x0D)
				{
					// ÒÆ³ý»»ÐÐ·û
					uart_buf[uart_buf_i - 1] = 0;
					uart_buf[uart_buf_i] = 0;
					newline_flag = 1;
				}
			}
			uart_buf_i++;		
		}
    }
}
