#ifndef UPDATE_PROTOCOL_H
#define UPDATE_PROTOCOL_H

#include <stdint.h>

/**
 * @brief  Firmware Update Error enumeration
 */
typedef enum
{
    BINSIZE_STRING_ERROR = -1, /*!< �̼���������� */
    BINSIZE_VALUE_ERROR = -2,  /*!< �̼�������ֵ���� */
    RECEIVE_TIMEOUT = -3,      /*!< ���ճ�ʱ */
    PACKET_HEADER_ERROR = -4,  /*!< ֡ͷ�����Ŵ��� */
    CRC_CCITT_ERROR = -5       /*!< CRC_CCITT���� */
} UpdateError_TypeDef;

int32_t firmware_update(void);

#endif /* UPDATE_PROTOCOL_H */
