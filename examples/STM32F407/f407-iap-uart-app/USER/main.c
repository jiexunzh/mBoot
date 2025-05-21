#include "delay.h"
#include "iap.h"
#include "led.h"
#include "sys.h"
#include "usart.h"

int main(void)
{
    u16 i = 0;

    IAP_Init();

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 设置系统中断优先级分组2
    delay_init(168);                                 // 初始化延时函数
    LED_Init();                                      // 初始化LED
    uart_init(115200);                               // 初始化串口波特率为115200
    printf("APP start...\r\n");

    while (1)
    {
        i++;
        if (i == 100)
        {
            LED0 = !LED0;  // 提示系统正在运行
            i = 0;
        }
        delay_ms(10);

        if (USART_RX_STA & 0x80)
        {
            USART_RX_STA = 0;
            IAP_Handle(USART_RX_BUF);
        }
    }
}
