/**
 ******************************************************************************
 * @file    main.c
 * @brief   mBoot app lite 的主函数入口
 *
 * @note    只需修改IROM1和中断向量偏移，与“mBoot_config.h”对应即可
 *          更新操作可通过上位机与MCU上电配合完成
 *          前提条件：Bootloader必须开启监听更新功能！
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#include "mBoot_config.h"
// #include "gd32c11x.h"

/**
 * @brief  set the NVIC vector table offset 
 *         and make sure it's consistent with uBoot_config.h
 */
void set_nvic_vector_table_offset(void)
{
    // nvic_vector_table_set(0x08000000, ApplicationAddress - 0x08000000);
}

int main(void)
{
    set_nvic_vector_table_offset();
    
    /* user code begin... */

    while (1)
    {
    }
}
