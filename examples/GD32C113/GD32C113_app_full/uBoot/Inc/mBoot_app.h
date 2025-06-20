/**
 ******************************************************************************
 * @file    mBoot_app.h
 * @brief   本文件声明了 mBoot 与 App 交互的相关接口函数
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#ifndef MBOOT_APP_H
#define MBOOT_APP_H

#include <stdint.h>

void set_nvic_vector_table_offset(void);
void mBoot_app_hardware_init(void);
void mBoot_app_handle(uint8_t* cmd);

#endif /* MBOOT_APP_H */
