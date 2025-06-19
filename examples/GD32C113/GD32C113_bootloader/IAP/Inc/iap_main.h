#ifndef IAP_MAIN_H
#define IAP_MAIN_H

#include <stdint.h>
#include "iap_config.h"

void iap_transmit_string(uint8_t* buf);
uint8_t iap_receive_byte(uint8_t* c, uint32_t timeout);
uint8_t iap_receive_string(uint8_t* buf, uint32_t timeout);
void iap_init(void);
void detect_app_flashed(void);
uint32_t IAP_ReadFlag(void);
void IAP_WriteFlag(uint32_t flag);
void IAP_Main_Menu(void);
uint8_t IAP_RunApp(void);
uint8_t IAP_Update(void);
uint8_t IAP_Upload(void);
uint8_t IAP_Erase(void);
void listen_update_command(uint32_t nms);

#endif /* IAP_MAIN_H */
