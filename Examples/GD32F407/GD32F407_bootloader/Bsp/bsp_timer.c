#include "bsp_timer.h"
#include "gd32f4xx.h"

/**
 * @brief  最大计时65.5秒，时钟源 = 2 × APB1_CLK （预分频器 ≠ 1）
 * @param  
 * @retval 
 */
void bsp_timer_init(void)
{
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = (SystemCoreClock / 2000) - 1;    // 1KHz
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 0xFFFF;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);
    
    timer_enable(TIMER1);
}
