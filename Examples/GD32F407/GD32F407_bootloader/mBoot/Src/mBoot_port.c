/**
 ******************************************************************************
 * @file    mBoot_port.c
 * @brief   本文件定义了 mBoot 所需的硬件适配接口函数，使用者需根据具体硬件平台进行实现。
 * 
 * @details 包括串口、Flash、定时器、延时、RS485 等功能的接口抽象。
 *          所有函数为移植层接口，默认调用外部 BSP 层函数（需用户实现或解注释）。
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#include "mBoot_port.h"
#include "mBoot_config.h"
#include "fmc_operation.h"
#include "bsp_rs485_debug.h"
#include "bsp_timer.h"

/**** UART port ****/
/**
 * @brief  初始化 mBoot 串口（无需配置中断）
 * @retval 无
 */
void mBoot_uart_init(void)
{
    usart_debug_init(115200);
}

/**
 * @brief  向串口发送一个字节
 * @param  data 要发送的数据字节
 * @retval 无
 */
void mBoot_uart_transmit_byte(uint8_t data)
{
    usart_data_transmit(USART0, data);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
}

/**
 * @brief  查询串口发送是否完成（TC 标志）
 * @retval 1: 发送完成；0: 未完成
 */
uint8_t is_mBoot_uart_transmit_complete(void)
{
    return usart_flag_get(USART0, USART_FLAG_TC);
}

/**
 * @brief  查询串口是否接收到数据（接收寄存器非空）
 * @retval 1: 接收到数据；0: 尚未接收
 */
uint8_t is_mBoot_uart_receive_notEmpty(void)
{
    return usart_flag_get(USART0, USART_FLAG_RBNE);
}

/**
 * @brief  读取接收到的串口数据
 * @retval 接收到的一个字节
 */
uint8_t mBoot_uart_receive_byte(void)
{
    return usart_data_receive(USART0);
}

/**** flash port ****/
/**
 * @brief  写 Flash 数据
 * @param  WriteAddr 写入地址
 * @param  pBuffer 数据指针
 * @param  NumToWrite 要写入的字数（单位：uint32_t）
 * @retval 无
 */
void mBoot_write_flash(uint32_t WriteAddr, uint32_t* pBuffer, uint32_t NumToWrite)
{
	fmc_write_32bit_data(WriteAddr, NumToWrite, (int32_t*)pBuffer);
}

/**
 * @brief  读 Flash 数据
 * @param  ReadAddr 读取地址
 * @param  pBuffer 数据缓冲区指针
 * @param  NumToRead 要读取的字数（单位：uint32_t）
 * @retval 无
 */
void mBoot_read_flash(uint32_t ReadAddr, uint32_t* pBuffer, uint32_t NumToRead)
{
	fmc_read_32bit_data(ReadAddr, NumToRead, (int32_t*)pBuffer);
}

/**** delay port ****/
/**
 * @brief  延时函数（单位：微秒）
 * @param  us 延时时长（微秒）
 * @retval 无
 */
void mBoot_delay_us(uint32_t us)
{
    volatile int i, j;
    for (i = 0; i != us; i++)
		for (j = 0; j < 13; j++)
			;
}

/**** timer port if use listen update command function when MCU power on ****/
/**
 * @brief  启动 1KHz 向上计数的定时器（用于监听超时功能）
 * @retval 无
 */
void mBoot_timer_init(void)
{
#if (USE_LISTEN_UPDATE == 1)
    bsp_timer_init();
#endif
}

/**
 * @brief  获取定时器当前计数值（单位：ms）
 * @retval 当前计数值
 */
uint32_t mBoot_get_timer_counter(void)
{
#if (USE_LISTEN_UPDATE == 1)
    return timer_counter_read(TIMER1);
#else
    return 0xFFFFFFFF;
#endif
}

/**** rs485 port if use rs485 ****/
/**
 * @brief  初始化 RS485 方向控制引脚
 * @retval 无
 */
void mBoot_rs485_dir_init(void)
{
#if (USE_RS485 == 1)
    RS485_debug_dir_init();
#endif
}

/**
 * @brief  设置 RS485 为发送模式
 * @retval 无
 */
void mBoot_rs485_dir_tx(void)
{
#if (USE_RS485 == 1)
    RS485_DEBUG_TX_EN;
#endif
}

/**
 * @brief  设置 RS485 为接收模式
 * @retval 无
 */
void mBoot_rs485_dir_rx(void)
{
#if (USE_RS485 == 1)
    RS485_DEBUG_RX_EN;
#endif
}
