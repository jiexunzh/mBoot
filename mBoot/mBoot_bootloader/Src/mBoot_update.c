#include "mBoot_update.h"
#include "mBoot_config.h"
#include "mBoot_main.h"
#include "mBoot_port.h"
#include "utils_crc16-ccitt.h"
#include "utils_intstr.h"
#include <stdio.h>
#include <string.h>

#define NAK_TIMEOUT         ((uint32_t)0xA00000)  // 根据不同MCU设定的一个等待设备响应的最大时间

#define PACKET_HEADER       ((uint8_t)0xA5)                                                 // 包头
#define PACKET_VALID_SIZE   (1024)                                                          // 数据包有效数据大小
#define PACKET_HEADER_SIZE  (2)                                                             // 数据包包头大小 = 包头 + 包序号
#define PACKET_TRAILER_SIZE (2)                                                             // 数据包包尾大小 = CRC16-CCITT
#define PACKET_SIZE         (PACKET_HEADER_SIZE + PACKET_VALID_SIZE + PACKET_TRAILER_SIZE)  // 数据包总大小
#define CRC_MAX_ERROR_NUM   (3)                                                             // 最大错误重传次数

int32_t firmware_update_process(void)
{
    static uint8_t recv_buf[PACKET_SIZE]; /* 接收缓冲 */
    static uint8_t send_buf[20];          /* 发送缓冲 */
    uint16_t i, crc16, packet_num;
    uint8_t crc_l, crc_h;
    int32_t binFile_size = 0; /* bin文件大小 */
    uint8_t err_num = 0;      /* 数据包校验错误次数 */
    uint32_t ret;             /* 接收返回 */

    /* step1: 发送固件更新命令应答“OK\r\n” */
    mBoot_transmit_string((uint8_t*)NEW_LINE(RESPONSE_OK_CMD));

    /* step2: 等待上位机发来的BIN文件大小字段“BINSIZE=xxxx\r\n”，解析并应答 */
    memset(recv_buf, 0, sizeof(recv_buf));
    if (mBoot_receive_string(recv_buf, NAK_TIMEOUT) == 0)
    {
        /* “BINSIZE=xxxx\r\n”字段接收超时 */
        return BINSIZE_TIMEOUT;
    }

    /* 判断"BINSIZE=xxxx" */
    if (strncmp((char*)recv_buf, BINSIZE_CMD, strlen(BINSIZE_CMD)) != 0)
    {
        /* 发来的不是以“BINSIZE=”开头的字符串 */
        return BINSIZE_STRING_ERROR;  // "BINSIZE=" error
    }
    else
    {
        /* 解析bin文件大小 */
        ret = Str2Int((char*)&recv_buf[strlen(BINSIZE_CMD)], &binFile_size);
        if (ret == 0)
        {
            return BINSIZE_VALUE_ERROR;  // "xxxx" error
        }
    }

    /* 250410: 延迟一段时间再应答，否则在极少数配置差的电脑上，接收到的会是乱码，原因未知 */
    mBoot_delay_us(10000);
    memset(send_buf, 0, sizeof(send_buf));
    sprintf((char*)send_buf, "%d\r\n", binFile_size);
    /* 响应bin文件大小"xxxx\r\n" */
    mBoot_transmit_string(send_buf);

    packet_num = (binFile_size % PACKET_VALID_SIZE) == 0 ? binFile_size / PACKET_VALID_SIZE : binFile_size / PACKET_VALID_SIZE + 1;

    /* step3: 接收数据包 */
    for (int16_t packet_i = 0; packet_i < packet_num; packet_i++)
    {
        memset(recv_buf, 0, sizeof(recv_buf));
        for (i = 0; i < PACKET_SIZE; i++)
        {
            if (mBoot_receive_byte(recv_buf + i, NAK_TIMEOUT) == 0)
            {
                return PACKET_TIMEOUT;  // 固件包接收超时
            }
        }

        if (recv_buf[0] != PACKET_HEADER || recv_buf[1] != packet_i)
        {
            return PACKET_HEADER_ERROR;  // 包头或包序号错误
        }

        crc16 = crc16_ccitt_cal(recv_buf, PACKET_SIZE - 2);
        crc_l = (uint8_t)(crc16 & 0xFF);
        crc_h = (uint8_t)(crc16 >> 8);

        /* 低位在前，高位在后，同CRC16-Modbus */
        if (recv_buf[PACKET_SIZE - 2] == crc_l
            && recv_buf[PACKET_SIZE - 1] == crc_h)
        {
            const uint32_t WRITE_START_ADDR = ApplicationAddress + (PACKET_VALID_SIZE * packet_i);
            mBoot_write_flash(WRITE_START_ADDR, (uint32_t*)(&recv_buf[2]), PACKET_VALID_SIZE / 4);
            mBoot_transmit_string((uint8_t*)NEW_LINE(RECEIVED_CMD));
            err_num = 0;
        }
        else
        {
            mBoot_transmit_string((uint8_t*)NEW_LINE(RECEIVED_ERROR_CMD));
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

    return binFile_size;
}
