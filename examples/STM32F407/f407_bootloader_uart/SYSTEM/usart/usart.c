#include "usart.h"
#include "stm32f4xx.h" 

/**
 * @brief  初始化串口
 * @param  bound 串口的波特率
 * @retval 
 */
void uart_init(uint32_t bound)
{
    // GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // 使能USART1时钟

    // 串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   // GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  // GPIOA10复用为USART1

    // USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  // GPIOA9与GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             // 复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        // 速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           // 推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   // 初始化PA9，PA10

    // USART1 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                                      // 波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      // 字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                           // 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                              // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                  // 收发模式
    USART_Init(USART1, &USART_InitStructure);                                        // 初始化串口1

    USART_Cmd(USART1, ENABLE);  // 使能串口1
}

/**
 * @brief  串口发送一个字节
 * @param  c 要打印的字节
 * @retval None
 */
void USARTSendByte(uint8_t c)
{
    USART_SendData(USART1, c);
    /* 发送数据寄存器中的1字节数据已全部移动到发送移位寄存器 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)    
    {
    }
}

/**
 * @brief  发送移位寄存器中的1字节数据已经通过TX脚一位一位全部移出去了，
 *         确保跳转前字符串完全通过TX发出去了
 * @retval 1: 完全通过TX发出去了
 *         0: 还没有通过TX发出去
 */
uint8_t USART_string_send_finish(void)
{
    return USART_GetFlagStatus(USART1, USART_FLAG_TC);
}

/**
 * @brief  检测串口是否接收到一个字节的数据
 * @param  c 存储接收字节数据变量或缓冲的指针
 * @retval 1: 接收到一个字节数据
 *         0: 没有接收到一个字节数据
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
 * @brief  清空串口数据寄存器
 * @note   在循环接收字符串之前调用，以防多接收到一个无关字符
 * @retval None
 */
void clear_USART_RDR(void)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
    {
        (void)USART_ReceiveData(USART1);  // 读取数据寄存器，丢弃数据
    }
}
