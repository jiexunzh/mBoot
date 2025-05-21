#ifndef IAP_CONFIG_H
#define IAP_CONFIG_H

#include "stmflash.h"

// #define MCU_SRAM_SIZE	(192 * 1024)		// F407-192KB

/* App区域地址 */
#define ApplicationAddress ADDR_FLASH_SECTOR_4
/* App区域和Bootloader区域共享信息的地址 */
#define IAP_FLAG_ADDR      ADDR_FLASH_SECTOR_1

/* 调试信息打印开关 */
#define ENABLE_PUTSTR      0

/* Bootloader IAP命令 -----------------------------------------*/
#define CMD_UPDATE_STR     "update" /* 更新 */
#define CMD_UPLOAD_STR     "upload" /* 上传Flash */
#define CMD_ERASE_STR      "erase"  /* 擦除Flash */
#define CMD_MENU_STR       "menu"   /* bootloader主菜单界面 */
#define CMD_RUNAPP_STR     "runapp" /* 运行APP */
#define CMD_DISWP_STR      "diswp"  /* 取消读写保护 */

/* 固件更新协议字段 -----------------------------------------*/
#define RESPONSE_OK_CMD    "OK"       /* 上位机发送CMD_UPDATE_STR后，MCU应答，可以正式开始固件更新了 */
#define BINSIZE_CMD        "BINSIZE=" /* 上位机发送BIN文件大小的前置字符串 */
#define RECEIVED_CMD       "RECV"     /* 上位机发送一个固件数据包后，MCU应答，接收到上位机的更新数据包了且校验通过 */
#define RECEIVED_ERROR_CMD "ERROR"    /* MCU应答，数据校验未通过或发生其他错误 */

/* 字符串字面量在预处理阶段进行拼接 */
#define WITH_NEWLINE(cmd)  cmd "\r\n"

#endif /* IAP_CONFIG_H */
