#ifndef __IAP_H_
#define __IAP_H_

#include "stmflash.h"
#include <stdint.h>
#include <string.h>

#define INIT_FLAG_DATA         0xFFFFFFFF  // Ĭ�ϱ�־������(��Ƭ�ӵ����)
#define UPDATE_FLAG_DATA       0xEEEEEEEE  // ���ر�־������
#define UPLOAD_FLAG_DATA       0xDDDDDDDD  // �ϴ���־������
#define ERASE_FLAG_DATA        0xCCCCCCCC  // ������־������
#define APPRUN_ERROR_FLAG_DATA 0xBBBBBBBB  // ���»�����ʧ�ܱ�־
#define APPRUN_FLAG_DATA       0x5A5A5A5A  // APP����Ҫ���κδ���ֱ������״̬

#define CMD_UPDATE_STR      "update"
#define CMD_UPLOAD_STR      "upload"
#define CMD_ERASE_STR       "erase"
#define CMD_MENU_STR        "menu"
#define CMD_RUNAPP_STR      "runapp"

/* IAP command------------------------------------------------ */
#define IAP_FLAG_ADDR          ADDR_FLASH_SECTOR_1                       // App�����Bootloader��������Ϣ�ĵ�ַ(��СΪ16K)
#define ApplicationAddress     ADDR_FLASH_SECTOR_4                       // Ӧ�ó����ַ
#define IAP_FLASH_SIZE         (ADDR_FLASH_SECTOR_4 - STM32_FLASH_BASE)  // �ж���ƫ��

void IAP_WriteFlag(uint32_t flag);
uint32_t IAP_ReadFlag(void);
void IAP_Init(void);
void IAP_Handle(uint8_t* cmd);
#endif
