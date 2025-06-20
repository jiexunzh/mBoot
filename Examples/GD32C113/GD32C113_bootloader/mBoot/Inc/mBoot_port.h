/**
 ******************************************************************************
 * @file    mBoot_port.h
 * @brief   mBoot 硬件抽象层接口头文件
 *
 * @details 声明 mBoot 所依赖的所有硬件相关接口函数，包含：
 *          - UART 初始化与收发
 *          - Flash 读写
 *          - 微秒级延时
 *          - 1KHz 定时器
 *          - RS485 控制
 *          
 *          所有接口需要用户根据目标平台实现，便于移植。
 * 
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#ifndef MBOOT_PORT_H
#define MBOOT_PORT_H

#include <stdint.h>
#include "gd32c11x.h"

/**** mBoot UART port ****/
void mBoot_uart_init(void);
void mBoot_uart_transmit_byte(uint8_t data);
uint8_t is_mBoot_uart_transmit_complete(void);
uint8_t is_mBoot_uart_receive_notEmpty(void);
uint8_t mBoot_uart_receive_byte(void);

/**** mBoot flash port ****/
void mBoot_write_flash(uint32_t WriteAddr, uint32_t* pBuffer, uint32_t NumToWrite);
void mBoot_read_flash(uint32_t ReadAddr, uint32_t* pBuffer, uint32_t NumToRead);

/**** mBoot delay port ****/
void mBoot_delay_us(uint32_t us);

/**** timer port if mBoot use listen update command function when MCU power on ****/
void mBoot_timer_init(void);
uint32_t mBoot_get_timer_counter(void);

/**** rs485 port if mBoot use rs485 ****/
void mBoot_rs485_dir_init(void);
void mBoot_rs485_dir_tx(void);
void mBoot_rs485_dir_rx(void);

#endif /* MBOOT_PORT_H */
