/**
 * @file iap_port.c
 * @brief 本文件定义了实现IAP所需的接口函数，使用者需根据实际硬件提供
 * @author ZJX
 * @date 2025-06-18
 * @version 1.0
 */
#include "iap_port.h"
#include "iap_config.h"
#include "bsp_rs485.h"
#include "bsp_flash.h"
#include "bsp_timer.h"

/**** UART port ****/
/**
 * @brief  IAP串口初始化，无需配置中断
 */
void iap_uart_init(void)
{
	bsp_rs485_uart_init();
}

/**
 * @brief  IAP串口发送一个字节数据
 * @param  data 发送的数据
 */
void iap_uart_transmit_byte(uint8_t data)
{
    usart_data_transmit(USART1, data);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));    
}

/**
 * @brief  IAP串口是否已经发送完成（TC标志）
 * @retval 1: 发送完成
 *         0: 发送未完成
 */
uint8_t is_iap_uart_transmit_complete(void)
{
    return usart_flag_get(USART1, USART_FLAG_TC);
}

/**
 * @brief  IAP串口是否已经接收到了数据（接收寄存器非空）
 * @retval 1: 已接收到数据
 *         0: 未接收到数据
 */
uint8_t is_iap_uart_receive_notEmpty(void)
{
    return usart_flag_get(USART1, USART_FLAG_RBNE);
}

/**
 * @brief  获取IAP串口接收到的数据
 * @retval 接收到的数据
 */
uint8_t iap_uart_receive_byte(void)
{
    return usart_data_receive(USART1);
}

/**** flash port ****/
void iap_write_flash(uint32_t WriteAddr, uint32_t* pBuffer, uint32_t NumToWrite)
{
	flash_write_data(WriteAddr, pBuffer, NumToWrite);
}

void iap_read_flash(uint32_t ReadAddr, uint32_t* pBuffer, uint32_t NumToRead)
{
    flash_read_data(ReadAddr, pBuffer, NumToRead);
}

/**** delay port ****/
void iap_delay_us(uint32_t us)
{
    volatile int i, j;
    for (i = 0; i != us; i++)
        for (j = 0; j < 10; j++)
            ;	
}

/**** timer port if use listen update command function when MCU power on ****/
/**
 * @brief  1KHz向上递增计数器初始化
 */
void iap_timer_init(void)
{
#if (USE_LISTEN_UPDATE_COMMAND == 1)
    bsp_timer_init();
#endif
}

uint32_t iap_get_timer_counter(void)
{
#if (USE_LISTEN_UPDATE_COMMAND == 1)
	return timer_counter_read(TIMER1);
#else
    return 0xFFFFFFFF;
#endif
}

/**** rs485 port if use rs485 ****/
void iap_rs485_dir_init(void)
{
#if (USE_RS485 == 1)
    bsp_rs485_dir_init();
#endif
}

void iap_rs485_dir_tx(void)
{
#if (USE_RS485 == 1)
    RS485_DIR_TX_EN;
#endif
}

void iap_rs485_dir_rx(void)
{
#if (USE_RS485 == 1)
    RS485_DIR_RX_EN;
#endif
}
