/**
 ******************************************************************************
 * @file    mBoot_app.c
 * @brief   本文件定义了用于处理 mBoot 与 App 区之间交互的相关函数
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#include <string.h>
#include <stdio.h>
#include "mBoot_app.h"
#include "mBoot_config.h"
// #include "bsp_rs485.h"
// #include "bsp_flash.h"
// #include "gd32c11x.h"

/**
 * @brief  set the NVIC vector table offset 
 *         and make sure it's consistent with uBoot_config.h
 */
void set_nvic_vector_table_offset(void)
{
    // nvic_vector_table_set(0x08000000, ApplicationAddress - 0x08000000);
}

void mBoot_app_hardware_init(void)
{
	// bsp_rs485_uart_init();
    // bsp_rs485_dir_init();
    // RS485_DIR_RX_EN;
}

void mBoot_write_flag(uint32_t flag)
{
    // flash_write_data(mBoot_FLAG_ADDR, &flag, 1);
}

/**
 * @brief 响应来自 App 的 Bootloader 命令
 * @param cmd 接收到的命令字符串，默认不包括"\r\n"
 * @note  采用软件复位的方法回到Bootloader，
           相当于重新启动整个系统，一切恢复到初始状态，彻底断绝和APP部分程序的联系
           如果采用App跳转到Boot的方法，则需进行关中断、切换MSP指针(RTOS)、恢复中断向量表等操作
 */
void mBoot_app_handle(uint8_t* cmd)
{
    if (strcmp((char*)cmd, CMD_UPDATE_STR) == 0)
    {
        mBoot_write_flag(UPDATE_FLAG_DATA);
        /* 完整的系统复位，包括硬件（外设、SRAM、寄存器）和软件 */
        NVIC_SystemReset();
    }
    else if (strcmp((char*)cmd, CMD_MENU_STR) == 0)
    {
        mBoot_write_flag(INIT_FLAG_DATA);
        /* 回到Bootloader主菜单界面（空板状态） */
        NVIC_SystemReset();
    }
    else if (strcmp((char*)cmd, CMD_RUNAPP_STR) == 0)
    {
        printf("app already running\r\n");
    }
    else
    {
        printf("cmd not found\r\n");
    }
}
