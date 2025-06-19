#include <string.h>
#include <stdio.h>
#include "iap_app.h"
#include "iap_config.h"
#include "bsp_flash.h"
#include "gd32c11x.h"

uint32_t IAP_ReadFlag(void)
{
    uint32_t flag = 0;
    flash_read_data(IAP_FLAG_ADDR, &flag, 1);
    return flag;
}

void IAP_WriteFlag(uint32_t flag)
{
    flash_write_data(IAP_FLAG_ADDR, &flag, 1);
}

/**
 * @brief  �л���Boot����"��λ��"���൱��������������ϵͳ��
           ���׶Ͼ���APP���ֳ������ϵ��һ�лָ�����ʼ״̬��
           ������Boot��ת��APP���������й��жϡ��л�MSPָ��(RTOS)���ָ��ж�������Ȳ���
 * @param
 * @retval
 */
void IAP_Handle(uint8_t* cmd)
{
    if (strcmp((char*)cmd, CMD_UPDATE_STR) == 0)
    {
        IAP_WriteFlag(UPDATE_FLAG_DATA);
        NVIC_SystemReset();	// ������ϵͳ��λ������Ӳ�������衢SRAM���Ĵ����������
    }
    else if (strcmp((char*)cmd, CMD_MENU_STR) == 0)
    {
        IAP_WriteFlag(INIT_FLAG_DATA);
        NVIC_SystemReset();
    }
    else if (strcmp((char*)cmd, CMD_RUNAPP_STR) == 0)
    {
        printf("app already running\r\n");
    }
//    else if (strcmp((char*)cmd, CMD_ERASE_STR) == 0)
//    {
//        IAP_WriteFlag(ERASE_FLAG_DATA);
//        NVIC_SystemReset();
//    }	
    else
    {
        printf("cmd not found\r\n");
    }
}
