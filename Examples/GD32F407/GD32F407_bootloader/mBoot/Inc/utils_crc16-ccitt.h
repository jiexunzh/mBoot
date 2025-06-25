/**
 ******************************************************************************
 * @file    utils_crc16-ccitt.h
 * @brief   CRC16-CCITT 校验函数头文件
 *
 * @details 声明对指定数据进行 CRC 校验的公共接口，
 *          以支持 Bootloader 中固件包完整性验证。
 *
 * @note    多项式为 0x1021，初始值为 0xFFFF，低位在前。
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#ifndef UTILS_CRC16_CCITT_H
#define UTILS_CRC16_CCITT_H

#include <stdint.h>

uint16_t crc16_ccitt_cal(uint8_t* ptr, uint32_t len);

#endif /* UTILS_CRC16-CCITT_H */
