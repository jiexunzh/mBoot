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
    /* 设置中断向量表的基地址为APP代码区 */
    NVIC_SetVectorTable(STM32_FLASH_BASE, IAP_FLASH_SIZE);
    /* 设置完中断向量表的基地址后，方可启用全局中断 */
    // __enable_irq();
}

/**
 * @brief  切换回Boot采用"复位大法"，相当于重新启动整个系统，
           彻底断绝和APP部分程序的联系，一切恢复到初始状态，
           不用像Boot跳转到APP那样，进行关中断、切换MSP指针(RTOS)、恢复中断向量表等操作
 * @param
 * @retval
 */
void IAP_Handle(uint8_t* cmd)
{
    if (strcmp((char*)cmd, CMD_UPDATE_STR) == 0)
    {
        IAP_WriteFlag(UPDATE_FLAG_DATA);
        NVIC_SystemReset();	// 完整的系统复位，包括硬件（外设、SRAM、寄存器）和软件
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
        // printf("已经在运行APP\r\n");
    }
    else
    {
        // printf("指令有误\r\n");
    }
}
