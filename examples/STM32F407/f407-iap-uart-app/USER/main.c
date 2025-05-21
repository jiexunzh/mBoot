#include "delay.h"
#include "iap.h"
#include "led.h"
#include "sys.h"
#include "usart.h"

int main(void)
{
    u16 i = 0;

    IAP_Init();

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // ����ϵͳ�ж����ȼ�����2
    delay_init(168);                                 // ��ʼ����ʱ����
    LED_Init();                                      // ��ʼ��LED
    uart_init(115200);                               // ��ʼ�����ڲ�����Ϊ115200
    printf("APP start...\r\n");

    while (1)
    {
        i++;
        if (i == 100)
        {
            LED0 = !LED0;  // ��ʾϵͳ��������
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
