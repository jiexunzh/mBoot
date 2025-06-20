/**
 ******************************************************************************
 * @file    mBoot_main.c
 * @author  ZJX
 * @brief   mBoot 启动加载器的主模块。
 *
 * @details 本文件实现了 mBoot 启动加载器的核心逻辑，
 *          包括初始化、命令监听、应用程序有效性判断、
 *          以及固件更新支持。它通过 UART 串口并结合 RS485
 *          方向控制，实现命令行交互式的启动加载器界面。
 *
 * @note    应用程序的启动与标志位管理功能也在此文件中实现。
 *          启动加载器判断 APP 是否有效的方式是检测其主堆栈指针（MSP）
 *          是否指向 SRAM 区域。
 *
 * @version 1.0.0
 * @date    2025-06-20
 ******************************************************************************
 */
#include "mBoot_main.h"
#include "mBoot_config.h"
#include "mBoot_port.h"
#include "mBoot_update.h"
#include "utils_intstr.h"
#include <string.h>

typedef void (*pFunction)(void);
static uint8_t check_app_valid(void);
static uint8_t is_time_passed(uint32_t timeout);
static void mBoot_clear_uart_received(void);

/**
 * @brief  mBoot初始化(UART, Timer, RS485 DIR)
 */
void mBoot_init(void)
{
    mBoot_uart_init();
    mBoot_rs485_dir_init();
    mBoot_rs485_dir_rx();
    mBoot_timer_init();
}

/**
 * @brief  检查是否已经通过其他方式（调试器等）下载了APP程序
 * @note   通过查看堆栈指针是否位于 SRAM 地址范围来判断是否已经下载了APP程序。
 *         堆栈指针：总是指向栈顶
 */
void detect_app_flashed(void)
{
    uint32_t flag;
    char buf[20] = {0};

    /* 如果是空板状态 */
    if (mBoot_read_flag() == INIT_FLAG_DATA)
    {
        /* 检查堆栈指针是否指向RAM，如果是则说明实际上可能已经通过其他途径刷入了程序 */
        if (check_app_valid())
        {
            mBoot_print("try to jump app.\r\n");
            /* 写入“APP运行标志” */
            mBoot_write_flag(APPRUN_FLAG_DATA);
            /* 防止在某些MCU上可能出现读取异常: read error: 4294967295 */
            mBoot_delay_us(10);
            flag = mBoot_read_flag();
            if (flag != APPRUN_FLAG_DATA)
            {
                /* 写入或读取异常 */
                Int2Str(buf, flag);
                mBoot_transmit_string((uint8_t*)"read error: ");
                mBoot_transmit_string((uint8_t*)buf);
            }
        }
    }
}

/**
 * @brief  bootloader启动时阻塞监听更新命令一段时间
 * @note   采用查询定时器方法判断是否超时，
 * @param  timeout 持续监听时间，单位ms
 * @retval None
 */
void listen_update_command(uint32_t nms)
{
    uint8_t cmdStr[CMD_STRING_SIZE];
    int16_t i;

    /* 上电持续监听更新功能未启用 */
    if (mBoot_get_timer_counter() == 0xFFFFFFFF)
    {
        return;
    }

    /* 如果不是更新状态 */
    if (mBoot_read_flag() != UPDATE_FLAG_DATA)
    {
        memset(cmdStr, 0, sizeof(cmdStr));

        for (i = 0; i < CMD_STRING_SIZE; i++)
        {
            /* 如果接收到字符会追加到cmdStr末尾 */
            if (mBoot_receive_byte(cmdStr + i, 0xFFFF) == 0)
            {
                /* 未接收到字符 */
                i--;
            }

            /* 判断接收的数据里面是否包含“update\r\n” */
            if (strstr((char*)cmdStr, NEW_LINE(CMD_UPDATE_STR)) != NULL)
            {
                mBoot_write_flag(UPDATE_FLAG_DATA);
                /* 防止后面出现读取Flag异常 */
                mBoot_delay_us(10);
                return;
            }

            /* 到达监听时间 */
            if (is_time_passed(nms))
            {
                mBoot_print("listen timeout.\r\n");
                return;
            }
        }
        mBoot_print("Cmd size over.\r\n");
    }
}

/**
 * @brief  轮询检查是否过了 timeout（从TIM2_Init开始计时）
 * @param  timeout 超时时间，单位ms
 * @retval 0 表示超时时间未到，1 表示超时时间已到
 * @internal
 */
static uint8_t is_time_passed(uint32_t timeout)
{
    static uint32_t last_time = 0;
    uint32_t current_time = mBoot_get_timer_counter();

    if ((current_time - last_time) >= timeout)
    {
        /* 更新时间戳 */
        last_time = current_time;
        return 1;
    }
    return 0;
}

uint32_t mBoot_read_flag(void)
{
    uint32_t flag = 0;
    mBoot_read_flash(mBoot_FLAG_ADDR, &flag, 1);
    return flag;
}

void mBoot_write_flag(uint32_t flag)
{
    mBoot_write_flash(mBoot_FLAG_ADDR, &flag, 1);
}

void mBoot_main_menu(void)
{
    uint8_t cmdStr[CMD_STRING_SIZE] = {0};

    while (1)
    {
        mBoot_transmit_string((uint8_t*)"\r\nmBoot Main Menu (V1.0.0)\r\n");
        mBoot_transmit_string((uint8_t*)NEW_LINE(CMD_MENU_STR));
        mBoot_transmit_string((uint8_t*)NEW_LINE(CMD_RUNAPP_STR));
        mBoot_transmit_string((uint8_t*)NEW_LINE(CMD_UPDATE_STR));
        // mBoot_transmit_string((uint8_t *)NEW_LINE(CMD_ERASE_STR));
        // mBoot_transmit_string((uint8_t *)NEW_LINE(CMD_UPLOAD_STR));

        mBoot_receive_string(cmdStr, 0xFFFFFFFF); /* 以换行为结束标识符 */

        if (strcmp((char*)cmdStr, CMD_MENU_STR) == 0)
        {
            mBoot_write_flag(INIT_FLAG_DATA);
        }
        else if (strcmp((char*)cmdStr, CMD_RUNAPP_STR) == 0)
        {
            mBoot_write_flag(APPRUN_FLAG_DATA);
            return;
        }
        else if (strcmp((char*)cmdStr, CMD_UPDATE_STR) == 0)
        {
            mBoot_write_flag(UPDATE_FLAG_DATA);
            return;
        }
        // else if (strcmp((char*)cmdStr, CMD_ERASE_STR) == 0)
        // {
        //     mBoot_write_flag(ERASE_FLAG_DATA);
        //     return;
        // }
        // else if (strcmp((char*)cmdStr, CMD_UPLOAD_STR) == 0)
        // {
        //     mBoot_write_flag(UPLOAD_FLAG_DATA);
        //     return;
        // }
        else
        {
            mBoot_transmit_string((uint8_t*)"Invalid CMD !\r\n");
        }
    }
}

/**
 * @brief  如果有效，跳转到主应用程序
 * @retval 1表示成功，0表示APP无效或跳转失败
 * @note  APP的中断向量表：
 *   ApplicationAddress: APP的堆栈指针的地址，默认的堆栈指针是主堆栈指针 MSP
 *   ApplicationAddress + 4: APP的复位向量地址，指向复位中断向量Reset_Handler
 *   运行Reset_Handler只是软件复位，不会触发真正的硬件复位（如复位所有寄存器、重新启动外设等）
 */
uint8_t mBoot_run_app(void)
{
    pFunction Jump_To_Application;
    uint32_t JumpAddress;

    /* 检查APP的主堆栈指针(MSP)是否指向RAM */
    if (check_app_valid())
    {
        mBoot_print("Jump to app.\r\n");
        /* 当前BOOT并未使用任何中断 */
        // __disable_irq();

        /* 取出APP复位中断向量Reset_Handler的地址为跳转地址 */
        JumpAddress = *(volatile uint32_t*)(ApplicationAddress + 4);
        /* 设置跳转函数指针 -> APP Reset_Handler */
        Jump_To_Application = (pFunction)JumpAddress;   
        /* 设置MSP为APP MSP的初始值 */    
        __set_MSP(*(volatile uint32_t*)ApplicationAddress); 
        /* PC指针正式跳转到 APP复位中断向量的入口，开始运行APP程序 */
        Jump_To_Application();                              
        return 1;
    }
    else /* APP的MSP不指向RAM，不跳转 */
    {
        mBoot_transmit_string((uint8_t*)"Jump to app error.\r\n");
        return 0;
    }
}

/**
 * @brief  判断APP的堆栈指针(SP)是否指向 SRAM 地址范围
 * @retval 0: SP不指向RAM；1: SP指向RAM
 * @internal
 */
static uint8_t check_app_valid(void)
{
    /* 检查 SP 的 bit 17-31, 但这个比较并不准确，可以直接采用准确的范围比较方式 */
    if (((*(volatile uint32_t*)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  执行固件更新步骤
 * @retval 1 更新成功；0 更新失败
 */
uint8_t mBoot_update(void)
{
    char strNumber[10] = "";
    int32_t Size = 0;

    Size = firmware_update_process();
    /* 给上位机一点时间切换为接收回显模式 */
    mBoot_delay_us(10000);
    /* 升级成功 */
    if (Size > 0)           
    {
        mBoot_transmit_string((uint8_t*)"Update finished!\r\n");
        Int2Str(strNumber, Size);
        mBoot_transmit_string((uint8_t*)"APP size: ");
        mBoot_transmit_string((uint8_t*)strNumber);
        mBoot_transmit_string((uint8_t*)"Bytes.\r\n");
        return 1;
    }
    else
    {
        mBoot_transmit_string((uint8_t*)"Update failed!\r\n");
        Size = -Size;
        Int2Str(strNumber, Size);
        mBoot_transmit_string((uint8_t*)"error code: -");
        mBoot_transmit_string((uint8_t*)strNumber);
        return 0;
    }
}

uint8_t mBoot_erase(void)
{
    mBoot_transmit_string((uint8_t*)"\r\ndeveloping...\r\n");
    return 1;
}

uint8_t mBoot_upload(void)
{
    mBoot_transmit_string((uint8_t*)"\r\ndeveloping...\r\n");
    return 1;
}

/**
 * @brief  通过UART发送一个以空结尾的字符串
 * @param  buf 指向字符串的指针 (uint8_t *)
 * @retval None
 */
void mBoot_transmit_string(uint8_t* buf)
{
    mBoot_rs485_dir_tx();
    while (*buf != '\0')
    {
        mBoot_uart_transmit_byte(*buf);
        buf++;
    }

    while (is_mBoot_uart_transmit_complete() == 0)
        ;
    mBoot_rs485_dir_rx();
}

/**
 * @brief  打印调试信息，可通过宏屏蔽
 */
void mBoot_print(char* s)
{
#if (ENABLE_PUTSTR == 1)
    mBoot_transmit_string((uint8_t*)s);
#endif
}

/**
 * @brief  阻塞性的接收字节/符函数
 * @param  c 字节数据指针
 * @param  timeout 阻塞时间，0xFFFFFFFF-永久阻塞
 * @retval 0-接收超时；1-接收成功
 */
uint8_t mBoot_receive_byte(uint8_t* c, uint32_t timeout)
{
    if (timeout == 0xFFFFFFFF)
    {
        while (1)
        {
            /* 串口接收寄存器非空 */
            if (is_mBoot_uart_receive_notEmpty() == 1)
            {
                *c = mBoot_uart_receive_byte();
                return 1;
            }
        }
    }
    else
    {
        while (timeout-- > 0)
        {
            /* 串口接收寄存器非空 */
            if (is_mBoot_uart_receive_notEmpty() == 1)
            {
                *c = mBoot_uart_receive_byte();
                return 1;
            }
        }
        return 0;
    }
}

/**
 * @brief  阻塞接收字符串，直到接收到“\r\n”，但是“\r\n”不会记录到buf中
 * @param  buf 接收缓冲
 * @param  timeout 阻塞时间，0xFFFFFFFF-永久阻塞
 * @retval 0-接收超时；1-接收成功
 */
uint8_t mBoot_receive_string(uint8_t* buf, uint32_t timeout)
{
    uint16_t bytes_read = 0;
    uint8_t c = 0;

    /* 防止第一个字符接收到之前的数据 */
    mBoot_clear_uart_received();

    do
    {
        if (mBoot_receive_byte(&c, timeout) == 0)
        {
            return 0;
        }

        if (c == '\n')
        {
            if (buf[bytes_read - 1] == '\r')
                break;
        }
        if (c == '\b') /* Backspace */
        {
            if (bytes_read > 0)
            {
                bytes_read--;
            }
            continue;
        }
        if (bytes_read >= CMD_STRING_SIZE)
        {
            mBoot_transmit_string((uint8_t*)"Cmd size over.\r\n");
            bytes_read = 0;
            continue;
        }
        if ((c >= 0x20 && c <= 0x7E) || c == '\r')
        {
            buf[bytes_read++] = c;
            // SerialSendChar(c);
        }
    } while (1);
    /* 替换'\r' */
    buf[bytes_read - 1] = '\0';
    return 1;
}

/**
 * @brief  清除串口接收寄存器
 * @internal
 */
static void mBoot_clear_uart_received(void)
{
    if (is_mBoot_uart_receive_notEmpty() == 1)
    {
        /* 读取数据寄存器，丢弃数据 */
        (void)mBoot_uart_receive_byte();
    }
}
