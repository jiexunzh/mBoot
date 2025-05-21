#include "timer.h"
#include "stm32f4xx_tim.h"

/**
 * @brief  初始化 TIM2
 * @note   向上递增，1KHz 计数频率
 * @retval None
 */
void TIM2_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 使能 TIM2 时钟

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000) - 1;  // 1KHz 计数频率
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;          // 向上递增
    TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;                       // 最大周期（32位），1KHz下溢出时间49.7天
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_Cmd(TIM2, ENABLE);  // 启动定时器
}

/**
 * @brief  获取TIM2计数器值
 * @retval TIM2计数器值
 */
uint32_t get_TIM2Counter(void)
{
    return TIM_GetCounter(TIM2);
}
