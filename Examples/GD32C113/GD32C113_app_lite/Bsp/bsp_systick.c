/**
 * *********************************************************************************
 * @file bsp_systick.c
 * @author ZJX
 * @version V1.1
 * @date 2024.8.10
 * @brief ���õδ�ʱ���жϷ���ʵ�ֺ�����ʱ�ͷ���������ʱ��ʹ�ÿ�ָ��ʵ��΢����ʱ
 *
 * **********************************************************************************
 */

#include "bsp_systick.h"
#include "gd32c11x.h"

/* ϵͳʱ�� */
static volatile uint32_t uwTick = 0;

/**
 * @brief  �δ�ʱ�����ã�1ms����1���ж�
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
 * @brief  ����������ʱ����
 * @param  start_tick ��ʼtickֵ
 * @param  delay_time ��Ҫ�ȴ���ʱ�䣨���룩
 * @retval 1 - ��ʱ���
 *         0 - ��ʱδ���
 */
uint8_t nonBlock(uint32_t start_tick, uint32_t delay_time)
{
    /* ��ǰʱ�� */
    uint32_t current_tick = get_tick();
    /* �Ѿ���ȥ��ʱ�� */
    uint32_t elapsed_time = current_tick - start_tick;

    /* ������ʽ������� */
    if (elapsed_time >= delay_time)
    {
        return 1;  // ��ʱ���
    }
    else
    {
        return 0;  // ��ʱδ���
    }
}

/**
 * @brief ������ʱ�������� SysTick_init() ֮�����
 * @param nms ��ʱ������
 * @retval None
 */
void delay_ms(uint32_t nms)
{
    uint32_t tickstart = get_tick();
    uint32_t wait = nms;

    /* ������ʱΪ0��������� */
    if (wait < SYS_MAX_DELAY)
    {
        wait++;
    }

    /* C�����У��޷��������������Ϊ�Ǳ�׼���ģ��������ֵ�����´�0��ʼ��
        ������迼��tick������� */
    while ((get_tick() - tickstart) < wait)
    {
        /* ����ִ�е͹���ģʽ������������ */
        // __WFI();
    }
}

/**
  * @brief  ʹ�á���ָ�ʵ�ֲִڵ�΢����ʱ���ڱ�Level0����Level���ܻ�ʧЧ���ܻ��ò���ȷ��
  *         ��������ƵΪ120Mhz��MCU��
            ����֤����ʱ20usʱ��ʵ����ʱΪ20.015us��10usΪ10.180us����Level0��
  * @param  count ��ʱ΢����
  * @retval
  */
void delay_us(uint16_t count)
{
    uint16_t i;

    for (i = 0; i < count; i++)
    {
        /* 113 = 120 - 7��ÿ��forѭ��ռ���߸�ָ������ */
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
 * @brief  ��ȡϵͳ����
 * @param  None
 * @retval None
 */
uint32_t get_tick(void)
{
    return uwTick;
}

/**
 * @brief  SysTick�жϷ�������ÿ1ms ϵͳʱ�� ����1��
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    /* ��׼ʱ������ */
    uwTick++;
}
