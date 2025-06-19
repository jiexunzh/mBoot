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
