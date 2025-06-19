#ifndef UTILS_CRC16_CCITT_H
#define UTILS_CRC16_CCITT_H

#include <stdint.h>

uint16_t crc16_ccitt_cal(uint8_t* ptr, uint32_t len);

#endif /* UTILS_CRC16-CCITT_H */
