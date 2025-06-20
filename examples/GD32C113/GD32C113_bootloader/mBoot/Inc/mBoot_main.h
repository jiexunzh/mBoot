/**
 ******************************************************************************
 * @file    mBoot_main.h
 * @author  ZJX
 * @brief   mBoot_main.c 的头文件。
 *
 * @details 本头文件声明了 mBoot 主逻辑模块中涉及的函数接口，
 *          包括初始化、跳转到应用、固件更新、上传、擦除、
 *          命令监听，以及与 UART 通讯相关的接口。
 *
 * @version 1.0.0
 * @date    2025-06-20
 ******************************************************************************
 */
#ifndef MBOOT_MAIN_H
#define MBOOT_MAIN_H

#include <stdint.h>

void mBoot_init(void);
void detect_app_flashed(void);
void listen_update_command(uint32_t nms);
uint32_t mBoot_read_flag(void);
void mBoot_write_flag(uint32_t flag);
void mBoot_main_menu(void);
uint8_t mBoot_run_app(void);
uint8_t mBoot_update(void);
uint8_t mBoot_upload(void);
uint8_t mBoot_erase(void);
void mBoot_transmit_string(uint8_t* buf);
void mBoot_print(char* s);
uint8_t mBoot_receive_byte(uint8_t* c, uint32_t timeout);
uint8_t mBoot_receive_string(uint8_t* buf, uint32_t timeout);


#endif /* MBOOT_MAIN_H */
