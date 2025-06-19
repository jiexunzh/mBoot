/**
 * *********************************************************************************
 * @file bsp_systick.c
 * @author ZJX
 * @version V1.1
 * @date 2024.8.10
 * @brief 采用滴答定时器中断方法实现毫秒延时和非阻塞性延时，使用空指令实现微秒延时
 *
 * **********************************************************************************
 */

#include "bsp_systick.h"
#include "gd32c11x.h"

/* 系统时基 */
static volatile uint32_t uwTick = 0;

/**
 * @brief  滴答定时器配置，1ms产生1次中断
 * @param  None
 * @retval None
 */
void SysTick_init(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        /* capture error */
        while (1)
        {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);

    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
}

/**
 * @brief  非阻塞性延时函数
 * @param  start_tick 起始tick值
 * @param  delay_time 需要等待的时间（毫秒）
 * @retval 1 - 延时完成
 *         0 - 延时未完成
 */
uint8_t nonBlock(uint32_t start_tick, uint32_t delay_time)
{
    /* 当前时间 */
    uint32_t current_tick = get_tick();
    /* 已经过去的时间 */
    uint32_t elapsed_time = current_tick - start_tick;

    /* 无需显式处理溢出 */
    if (elapsed_time >= delay_time)
    {
        return 1;  // 延时完成
    }
    else
    {
        return 0;  // 延时未完成
    }
}

/**
 * @brief 毫秒延时函数，在 SysTick_init() 之后调用
 * @param nms 延时毫秒数
 * @retval None
 */
void delay_ms(uint32_t nms)
{
    uint32_t tickstart = get_tick();
    uint32_t wait = nms;

    /* 处理延时为0的特殊情况 */
    if (wait < SYS_MAX_DELAY)
    {
        wait++;
    }

    /* C语言中，无符号整数的溢出行为是标准化的（超过最大值会重新从0开始）
        因此无需考虑tick溢出问题 */
    while ((get_tick() - tickstart) < wait)
    {
        /* 可以执行低功耗模式或者其他处理 */
        // __WFI();
    }
}

/**
  * @brief  使用“空指令”实现粗糙的微秒延时。在比Level0更高Level可能会失效可能会变得不精确。
  *         适用于主频为120Mhz的MCU。
            经验证，延时20us时，实际延时为20.015us。10us为10.180us。（Level0）
  * @param  count 延时微秒数
  * @retval
  */
void delay_us(uint16_t count)
{
    uint16_t i;

    for (i = 0; i < count; i++)
    {
        /* 113 = 120 - 7，每次for循环占用七个指令周期 */
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();

        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();        
    }
}

/**
 * @brief  获取系统节拍
 * @param  None
 * @retval None
 */
uint32_t get_tick(void)
{
    return uwTick;
}

/**
 * @brief  SysTick中断服务函数，每1ms 系统时基 自增1次
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    /* 基准时基自增 */
    uwTick++;
}
