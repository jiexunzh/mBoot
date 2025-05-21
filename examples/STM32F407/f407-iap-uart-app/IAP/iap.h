#ifndef __IAP_H_
#define __IAP_H_

#include "stmflash.h"
#include <stdint.h>
#include <string.h>

#define INIT_FLAG_DATA         0xFFFFFFFF  // 默认标志的数据(空片子的情况)
#define UPDATE_FLAG_DATA       0xEEEEEEEE  // 下载标志的数据
#define UPLOAD_FLAG_DATA       0xDDDDDDDD  // 上传标志的数据
#define ERASE_FLAG_DATA        0xCCCCCCCC  // 擦除标志的数据
#define APPRUN_ERROR_FLAG_DATA 0xBBBBBBBB  // 更新或升级失败标志
#define APPRUN_FLAG_DATA       0x5A5A5A5A  // APP不需要做任何处理，直接运行状态

#define CMD_UPDATE_STR      "update"
#define CMD_UPLOAD_STR      "upload"
#define CMD_ERASE_STR       "erase"
#define CMD_MENU_STR        "menu"
#define CMD_RUNAPP_STR      "runapp"

/* IAP command------------------------------------------------ */
#define IAP_FLAG_ADDR          ADDR_FLASH_SECTOR_1                       // App区域和Bootloader区域共享信息的地址(大小为16K)
#define ApplicationAddress     ADDR_FLASH_SECTOR_4                       // 应用程序地址
#define IAP_FLASH_SIZE         (ADDR_FLASH_SECTOR_4 - STM32_FLASH_BASE)  // 中断向偏移

void IAP_WriteFlag(uint32_t flag);
uint32_t IAP_ReadFlag(void);
void IAP_Init(void);
void IAP_Handle(uint8_t* cmd);
#endif
