#ifndef MBOOT_MAIN_H
#define MBOOT_MAIN_H

#include <stdint.h>
#include "mBoot_config.h"

void mBoot_init(void);
void mBoot_transmit_string(uint8_t* buf);
uint8_t mBoot_receive_byte(uint8_t* c, uint32_t timeout);
uint8_t mBoot_receive_string(uint8_t* buf, uint32_t timeout);
void detect_app_flashed(void);
uint32_t mBoot_read_flag(void);
void mBoot_write_flag(uint32_t flag);
void mBoot_main_menu(void);
uint8_t mBoot_run_app(void);
uint8_t mBoot_update(void);
uint8_t mBoot_upload(void);
uint8_t mBoot_erase(void);
void listen_update_command(uint32_t nms);

#endif /* MBOOT_MAIN_H */
