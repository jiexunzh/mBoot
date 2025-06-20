/**
 * *********************************************************************************
 * @file bsp_systick.h
 * @author ZJX
 * @version V1.0
 * @date 2024.6.21
 * @brief 采用滴答定时器中断的方法实现毫秒延时和非阻塞性延时
 *
 * **********************************************************************************
 */

#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

#define SYS_MAX_DELAY      		0xFFFFFFFFU

void SysTick_init(void);
uint32_t get_tick(void);
void delay_ms(uint32_t nms);
void delay_us(uint16_t count);
uint8_t nonBlock(uint32_t start_tick, uint32_t delay_time);

#endif


