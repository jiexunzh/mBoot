#include "usart.h"
#include "stm32f4xx.h" 

/**
 * @brief  ��ʼ������
 * @param  bound ���ڵĲ�����
 * @retval 
 */
void uart_init(uint32_t bound)
{
    // GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   // ʹ��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // ʹ��USART1ʱ��

    // ����1��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // GPIOA9����ΪUSART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // GPIOA10����ΪUSART1

    // USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  // GPIOA9��GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             // ���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // �ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           // ���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             // ����
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   // ��ʼ��PA9��PA10

    // USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;                                      // ����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      // �ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                           // һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                              // ����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // ��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                  // �շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);                                        // ��ʼ������1

    USART_Cmd(USART1, ENABLE);  // ʹ�ܴ���1
}

/**
 * @brief  ���ڷ���һ���ֽ�
 * @param  c Ҫ��ӡ���ֽ�
 * @retval None
 */
void USARTSendByte(uint8_t c)
{
    USART_SendData(USART1, c);
    /* �������ݼĴ����е�1�ֽ�������ȫ���ƶ���������λ�Ĵ��� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)    
    {
    }
}

/**
 * @brief  ������λ�Ĵ����е�1�ֽ������Ѿ�ͨ��TX��һλһλȫ���Ƴ�ȥ�ˣ�
 *         ȷ����תǰ�ַ�����ȫͨ��TX����ȥ��
 * @retval 1: ��ȫͨ��TX����ȥ��
 *         0: ��û��ͨ��TX����ȥ
 */
uint8_t USART_string_send_finish(void)
{
    return USART_GetFlagStatus(USART1, USART_FLAG_TC);
}

/**
 * @brief  ��⴮���Ƿ���յ�һ���ֽڵ�����
 * @param  c �洢�����ֽ����ݱ����򻺳��ָ��
 * @retval 1: ���յ�һ���ֽ�����
 *         0: û�н��յ�һ���ֽ�����
 */
uint8_t USARTRecvByte(uint8_t* c)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
    {
        // *c = (uint8_t)USART1->DR;
        *c = (uint8_t)USART_ReceiveData(USART1);
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  ��մ������ݼĴ���
 * @note   ��ѭ�������ַ���֮ǰ���ã��Է�����յ�һ���޹��ַ�
 * @retval None
 */
void clear_USART_RDR(void)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
    {
        (void)USART_ReceiveData(USART1);  // ��ȡ���ݼĴ�������������
    }
}
