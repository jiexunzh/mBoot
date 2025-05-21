#ifndef UPDATE_PROTOCOL_H
#define UPDATE_PROTOCOL_H

#include <stdint.h>

/**
 * @brief  Firmware Update Error enumeration
 */
typedef enum
{
    BINSIZE_STRING_ERROR = -1, /*!< ¹Ì¼þ³¤ÃüÁî´íÎó */
    BINSIZE_VALUE_ERROR = -2,  /*!< ¹Ì¼þ³¤ÃüÁîÖµ´íÎó */
    RECEIVE_TIMEOUT = -3,      /*!< ½ÓÊÕ³¬Ê± */
    PACKET_HEADER_ERROR = -4,  /*!< Ö¡Í·»ò°üÐòºÅ´íÎó */
    CRC_CCITT_ERROR = -5       /*!< CRC_CCITT´íÎó */
} UpdateError_TypeDef;

int32_t firmware_update(void);

#endif /* UPDATE_PROTOCOL_H */
