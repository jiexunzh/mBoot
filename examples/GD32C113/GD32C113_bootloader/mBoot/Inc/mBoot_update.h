/**
 ******************************************************************************
 * @file    mBoot_update.h
 * @brief   mBoot 固件更新模块头文件
 *
 * @details 声明了固件更新过程的错误码枚举类型与主处理函数接口。
 *          该模块实现了基于串口的 IAP 升级机制。
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#ifndef MBOOT_UPDATE_H
#define MBOOT_UPDATE_H

#include <stdint.h>

/**
 * @brief 固件更新过程中的错误码定义
 */
typedef enum
{
    BINSIZE_TIMEOUT = -1,      /**< 固件大小字段接收超时 */
    BINSIZE_STRING_ERROR = -2, /**< 固件大小字段格式错误（未以 BINSIZE= 开头） */
    BINSIZE_VALUE_ERROR = -3,  /**< 固件大小字段值无法解析 */
    PACKET_TIMEOUT = -4,       /**< 固件数据包接收超时 */
    PACKET_HEADER_ERROR = -5,  /**< 包头或包序号错误 */
    CRC_CCITT_ERROR = -6       /**< CRC 校验错误，超过最大允许次数 */
} update_error_t;

int32_t firmware_update_process(void);

#endif /* MBOOT_UPDATE_H */
