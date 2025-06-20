/**
 ******************************************************************************
 * @file    main.c
 * @brief   mBoot app full 的主函数入口
 *
 * @note    具备响应mBoot命令，主动从APP切换回Bootloader完整功能
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#include "mBoot_app.h"

int main(void)
{
    /* 设置中断向量表偏移 */
    set_nvic_vector_table_offset();
    /* 初始化串口与RS485等 */
    mBoot_app_hardware_init();

    /* user code begin... */

    while (1)
    {
        /* 当接收到以"\r\n"结尾的一行命令时，调用 mBoot_app_handle(cmd_buf) */
    }
}

