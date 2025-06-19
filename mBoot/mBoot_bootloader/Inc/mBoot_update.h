#ifndef MBOOT_UPDATE_H
#define MBOOT_UPDATE_H

#include <stdint.h>

/**
 * @brief  Firmware Update Error enumeration
 */
typedef enum
{
	BINSIZE_TIMEOUT = -1,	   /**< 固件长命令接收超时 */
    BINSIZE_STRING_ERROR = -2, /**< 固件长命令错误 */
    BINSIZE_VALUE_ERROR = -3,  /**< 固件长命令值错误 */
    PACKET_TIMEOUT = -4,      /**< 固件包接收超时 */
    PACKET_HEADER_ERROR = -5,  /**< 帧头或包序号错误 */
    CRC_CCITT_ERROR = -6       /**< CRC校验错误 */
} update_error_t;

int32_t firmware_update_process(void);

#endif /* MBOOT_UPDATE_H */
