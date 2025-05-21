#include "iap.h"

/************************************************************************/
void IAP_WriteFlag(uint32_t flag)
{
    STMFLASH_Write(IAP_FLAG_ADDR, &flag, 1);
}

/************************************************************************/
uint32_t IAP_ReadFlag(void)
{
    return STMFLASH_ReadWord(IAP_FLAG_ADDR);
}

void IAP_Init(void)
{
    /* �����ж�������Ļ���ַΪAPP������ */
    NVIC_SetVectorTable(STM32_FLASH_BASE, IAP_FLASH_SIZE);
    /* �������ж�������Ļ���ַ�󣬷�������ȫ���ж� */
    // __enable_irq();
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
    else if (strcmp((char*)cmd, CMD_ERASE_STR) == 0)
    {
        IAP_WriteFlag(ERASE_FLAG_DATA);
        NVIC_SystemReset();
    }
    else if (strcmp((char*)cmd, CMD_MENU_STR) == 0)
    {
        IAP_WriteFlag(INIT_FLAG_DATA);
        NVIC_SystemReset();
    }
    else if (strcmp((char*)cmd, CMD_RUNAPP_STR) == 0)
    {
        // printf("�Ѿ�������APP\r\n");
    }
    else
    {
        // printf("ָ������\r\n");
    }
}
