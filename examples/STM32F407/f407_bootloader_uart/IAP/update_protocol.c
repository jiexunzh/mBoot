/**
 ******************************************************************************
 * @file    update_protocol.c
 * @author  ZJX
 * @version V1.0.0
 * @date    2025.03.27
 * @brief   This file provides a firmware update protocol to receive the
 *          firmware from the host computer and response.
 ******************************************************************************
 */

#include "update_protocol.h"
#include "common.h"
#include "delay.h"
#include "iap.h"
#include "iap_config.h"
#include "stmflash.h"
#include <stdio.h>
#include <string.h>

#define PACKET_HEADER      ((uint8_t)0xA5)                           // 包头
#define PACKET_SIZE        (1024)                                    // 有效数据长度
#define PACKET_HEADER_LEN  (2)                                       // 包头长度 = 包头长度 + 包序号长度
#define PACKET_TRAILER_LEN (2)                                       // 包尾长度 = CRC-CCITT
#define PACKET_OVERHEAD    (PACKET_HEADER_LEN + PACKET_TRAILER_LEN)  // 数据包总长度
#define CRC_MAX_ERROR_NUM  (3)  // CRC校验错误重传次数

static uint8_t recv_buf[PACKET_SIZE + PACKET_OVERHEAD];

/**
 * @brief  receive a firmware using the update protocol and write to flash
 * @param  buf: Address of the first byte
 * @retval firmware size (>0) or error code (<0)
 */
int32_t firmware_update(void)
{
    uint8_t crc_l, crc_h;
    uint16_t i, crc, packet_num;
    uint32_t xReturn;
    int32_t bin_size = 0;
    char send_buf[20];
    uint8_t err_num = 0;

    // step0: MCU发送更新命令应答
    SerialPutString(WITH_NEWLINE(RESPONSE_OK_CMD));

    // step1: 等待上位机发送BIN文件大小："BINSIZE=xxxx\r\n"
    memset(recv_buf, 0, sizeof(recv_buf));
    GetInputString(recv_buf);

    /* 接收到的不是以BINSIZE_CMD开头的字符串 */
    if (strncmp((char*)recv_buf, BINSIZE_CMD, strlen(BINSIZE_CMD)) != 0)
    {
        return BINSIZE_STRING_ERROR;  // "BINSIZE=" error
    }
    else
    {
        xReturn = Str2Int((char*)&recv_buf[strlen(BINSIZE_CMD)], &bin_size);  // get the bin file size
        if (xReturn == 0)
        {
            return BINSIZE_STRING_ERROR;  // "BINSIZE=xxxx" error
        }
    }
	/* 250410: 接收到BINSIZE后要延迟一段时间再应答，
		否则在少数配置比较差的电脑上会接收的是乱码，原因未知
	*/	
	delay_ms(20);		
    memset(send_buf, 0, sizeof(send_buf));
    sprintf(send_buf, "%d\r\n", bin_size);
    SerialPutString(send_buf);  // response "xxxx\r\n"

    packet_num = (bin_size % PACKET_SIZE) == 0 ? bin_size / PACKET_SIZE : bin_size / PACKET_SIZE + 1;

    // step2: 接收数据包
    for (int16_t packet_i = 0; packet_i < packet_num; packet_i++)
    {
        memset(recv_buf, 0, sizeof(recv_buf));
        for (i = 0; i < (PACKET_SIZE + PACKET_OVERHEAD); i++)
        {
            if (Receive_Byte(recv_buf + i, NAK_TIMEOUT) == 0)
            {
                return RECEIVE_TIMEOUT;  // 接收超时
            }
        }

        if (recv_buf[0] != PACKET_HEADER || recv_buf[1] != packet_i)
        {
            return PACKET_HEADER_ERROR;  // 帧头或包序号错误
        }

        crc = crc16_ccitt(recv_buf, PACKET_SIZE + PACKET_HEADER_LEN);
        crc_l = (uint8_t)(crc & 0xFF);
        crc_h = (uint8_t)(crc >> 8);

        /* 低位在前，高位在后，同CRC-Modbus */
        if (recv_buf[PACKET_SIZE + PACKET_HEADER_LEN] == crc_l
            && recv_buf[PACKET_SIZE + PACKET_HEADER_LEN + 1] == crc_h)
        {
            const uint32_t WRITE_START_ADDR = ApplicationAddress + (PACKET_SIZE * packet_i);
            MCU_flash_write(WRITE_START_ADDR, (uint32_t*)(&recv_buf[2]), PACKET_SIZE / 4);
            SerialPutString(WITH_NEWLINE(RECEIVED_CMD));
            err_num = 0;
        }
        else
        {
            SerialPutString(WITH_NEWLINE(RECEIVED_ERROR_CMD));
            err_num++;
            if (err_num < CRC_MAX_ERROR_NUM)
            {
                packet_i--;
                continue;  // 未超过CRC最大错误次数，错误重传
            }
            else
            {
                return CRC_CCITT_ERROR;  // 超过CRC最大错误次数，返回CRC校验错误
            }
        }
    }

    return bin_size;
}
