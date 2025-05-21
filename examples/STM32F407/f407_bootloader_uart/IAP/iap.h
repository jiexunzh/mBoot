#ifndef IAP_H
#define IAP_H

#include <stdint.h>

/* 以字为单位 */
#define INIT_FLAG_DATA         ((uint32_t)(0xFFFFFFFF))  // 默认标志的数据(空片子的情况)
#define UPDATE_FLAG_DATA       ((uint32_t)(0xEEEEEEEE))  // 下载标志的数据
#define UPLOAD_FLAG_DATA       ((uint32_t)(0xDDDDDDDD))  // 上传标志的数据
#define ERASE_FLAG_DATA        ((uint32_t)(0xCCCCCCCC))  // 擦除标志的数据
#define APPRUN_ERROR_FLAG_DATA ((uint32_t)(0xBBBBBBBB))  // 更新或升级失败标志
#define APPRUN_FLAG_DATA       ((uint32_t)(0x5A5A5A5A))  // APP不需要做任何处理，直接运行状态

#define CMD_STRING_SIZE        ((uint16_t)128)       // 命令字符串的最大长度
#define NAK_TIMEOUT            ((uint32_t)0x300000)  // 根据不同MCU设定的一个等待设备响应的最大时间

void SerialPutString(char* s);
void SerialPrint(char* s);
uint8_t SerialKeyPressed(uint8_t* key);
void clearSerialRXCache(void);
void IAP_Init(void);
void detect_app_flashed(void);
uint8_t IAP_RunApp(void);
void IAP_Main_Menu(void);
uint8_t IAP_Update(void);
uint8_t IAP_Upload(void);
uint8_t IAP_Erase(void);
uint32_t IAP_ReadFlag(void);
void IAP_WriteFlag(uint32_t flag);
void MCU_flash_write(uint32_t WriteAddr, uint32_t* pBuffer, uint32_t NumToWrite);
void GetInputString(uint8_t* buffP);
uint8_t GetKey(void);
uint8_t check_app_valid(void);
uint8_t Receive_Byte(uint8_t* c, uint32_t timeout);
void wait_for_update(uint32_t time_ms);

#endif /* IAP_H */
