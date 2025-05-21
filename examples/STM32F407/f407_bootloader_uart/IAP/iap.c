#include "iap.h"
#include "common.h"
#include "delay.h"
#include "iap_config.h"
#include "stmflash.h"
#include "update_protocol.h"
#include "usart.h"
#include <string.h>
#include "timer.h"

typedef void (*pFunction)(void);
static uint8_t Is_time_passed(uint32_t timeout);

void IAP_Init(void)
{
    delay_init(168);  // 有用到延时，应进行延时初始化
    uart_init(115200);
}

/**
 * @brief  打印字符串，用于打印非调试信息
 * @param  s 打印的字符串，结尾必须为“\0”
 * @retval None
 */
void SerialPutString(char* s)
{
    while (*s != '\0')
    {
        // 提供：USARTSendByte，发送一个字节接口
        USARTSendByte(*s);
        s++;
    }

    // 提供：USART_string_send_finish（若不要求跳转前打印完整可去掉）
    while (USART_string_send_finish() == RESET)
        ;
}

/**
 * @brief  打印调试信息，可通过宏屏蔽
 */
void SerialPrint(char* s)
{
#if (ENABLE_PUTSTR == 1)
    SerialPutString(s);
#endif
}

/**
 * @brief  IAP串口接收字符
 * @param  key 字符指针
 * @retval 接收到字符返回1，否则返回0
 */
uint8_t SerialKeyPressed(uint8_t* key)
{
    // 提供：USARTRecvByte，接收一个字节接口
    return USARTRecvByte(key);
}

void clearSerialRXCache(void)
{
    // 提供：clear_USART_RDR，清空串口数据寄存器缓存
    clear_USART_RDR();
}

void MCU_flash_write(uint32_t WriteAddr, uint32_t* pBuffer, uint32_t NumToWrite)
{
    STMFLASH_Write(WriteAddr, pBuffer, NumToWrite);  // 提供：
}

/**
 * @brief  检查是否已经通过下载了APP程序
 *         通过查看堆栈指针是否位于 SRAM 地址范围
 *         来判断是否已经下载了APP程序
 *         堆栈指针: 总是指向栈顶
 * @param
 * @retval
 */
void detect_app_flashed(void)
{
    uint32_t flag;
    char buf[20] = {0};

    // 如果是空板状态
    if (IAP_ReadFlag() == INIT_FLAG_DATA)
    {
        // 检查堆栈指针是否指向RAM
        if (check_app_valid())
        {
            SerialPrint("try to jump app.\r\n");
            IAP_WriteFlag(APPRUN_FLAG_DATA); /* 写入“APP运行标志” */
            delay_us(10);                    // 不加延时可能会出现读取异常: IAP_ReadFlag error: 4294967295
            flag = IAP_ReadFlag();
            if (flag != APPRUN_FLAG_DATA)
            {
                Int2Str(buf, flag);
                SerialPutString("IAP_ReadFlag error: ");
                SerialPutString(buf);
            }
        }
    }
}

/************************************************************************
APP的中断向量表：
ApplicationAddress: APP的堆栈指针的地址，默认的堆栈指针的主堆栈指针 MSP
ApplicationAddress + 4: APP的复位向量地址，指向复位中断向量Reset_Handler
  运行Reset_Handler只是软件复位，不会触发真正的硬件复位（如复位所有寄存器、重新启动外设等）
************************************************************************/
uint8_t IAP_RunApp(void)
{
    pFunction Jump_To_Application;
    uint32_t JumpAddress;

    /* 检查APP的主堆栈指针 MSP 是否指向 RAM */
    if (check_app_valid())
    {
        SerialPutString("Jump to app.\r\n");
        /* 关闭所有中断（当前BOOT并未使用任何中断） */
        // __disable_irq();

        /* 取出APP复位中断向量Reset_Handler的地址为跳转地址 */
        JumpAddress = *(__IO uint32_t*)(ApplicationAddress + 4);
        Jump_To_Application = (pFunction)JumpAddress;   /* 设置跳转函数指针 -> APP Reset_Handler */
        __set_MSP(*(__IO uint32_t*)ApplicationAddress); /* 设置MSP为APP MSP的初始值 */
        Jump_To_Application();                          /* PC指针正式跳转到 APP复位中断向量的入口，开始运行APP程序 */
        return 1;
    }
    else /* APP的MSP不指向RAM，不跳转 */
    {
        SerialPutString("Jump to app error.\r\n");
        return 0;
    }
}

// 判断APP的堆栈指针(SP)是否指向 SRAM 地址范围
uint8_t check_app_valid(void)
{
    // 检查 SP 的 bit 17-31, 但这个比较并不准确，可以直接采用准确的范围比较方式
    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/************************************************************************/
void IAP_Main_Menu(void)
{
    uint8_t cmdStr[CMD_STRING_SIZE] = {0};

    while (1)
    {
        SerialPutString("\r\nIAP Main Menu (V 0.2.0)\r\n");
        SerialPutString(WITH_NEWLINE(CMD_UPDATE_STR));  // update\r\n
        SerialPutString(WITH_NEWLINE(CMD_UPLOAD_STR));  // upload\r\n
        SerialPutString(WITH_NEWLINE(CMD_ERASE_STR));   // erase\r\n
        SerialPutString(WITH_NEWLINE(CMD_MENU_STR));    // menu\r\n
        SerialPutString(WITH_NEWLINE(CMD_RUNAPP_STR));  // runapp\r\n

        GetInputString(cmdStr); /* 以换行为结束标识符 */

        if (strcmp((char*)cmdStr, CMD_UPDATE_STR) == 0)
        {
            IAP_WriteFlag(UPDATE_FLAG_DATA);
            return;
        }
        else if (strcmp((char*)cmdStr, CMD_UPLOAD_STR) == 0)
        {
            IAP_WriteFlag(UPLOAD_FLAG_DATA);
            return;
        }
        else if (strcmp((char*)cmdStr, CMD_ERASE_STR) == 0)
        {
            IAP_WriteFlag(ERASE_FLAG_DATA);
            return;
        }
        else if (strcmp((char*)cmdStr, CMD_MENU_STR) == 0)
        {
            IAP_WriteFlag(INIT_FLAG_DATA);
        }
        else if (strcmp((char*)cmdStr, CMD_RUNAPP_STR) == 0)
        {
            IAP_WriteFlag(APPRUN_FLAG_DATA);
            return;
        }
        else
        {
            SerialPutString(" Invalid CMD !\r\n");
        }
    }
}

/************************************************************************/
uint8_t IAP_Update(void)
{
    char strNumber[10] = "";
    int32_t Size = 0;

    Size = firmware_update();
    delay_ms(5);   // 留给上位机一点时间切换为接收回显模式
    if (Size > 0)  // 升级成功
    {
        SerialPrint("Update finished!\r\n");
        Int2Str(strNumber, Size);
        SerialPrint("APP size: ");
        SerialPrint(strNumber);
        SerialPrint(" Bytes.\r\n");
        return 1;
    }
    else
    {
        Size = -Size;
        Int2Str(strNumber, Size);  // 只支持非负数
        SerialPutString("ERROR CODE: -");
        SerialPutString(strNumber);
        SerialPutString("\r\nUpdate failed!\r\n");
        return 0;
    }
}

uint8_t IAP_Upload(void)
{
    SerialPutString("\r\ndeveloping...\r\n");
    return 1;
}

uint8_t IAP_Erase(void)
{
    SerialPutString("\r\ndeveloping...\r\n");
    return 1;
}

uint32_t IAP_ReadFlag(void)
{
    /* 确保底层访问已添加 volatile */
    return STMFLASH_ReadWord(IAP_FLAG_ADDR);
}

void IAP_WriteFlag(uint32_t flag)
{
    MCU_flash_write(IAP_FLAG_ADDR, &flag, 1);
}

/**
 * @brief  阻塞性的接收字符串函数
 * @param  buffP The input string
 * @retval None
 */
void GetInputString(uint8_t* buffP)
{
    uint16_t bytes_read = 0;
    uint8_t c = 0;

    clearSerialRXCache();  // 清除串口接收寄存器，防止第一个字符接收到之前的数据
    do
    {
        c = GetKey();   // 阻塞
        if (c == '\n')
        {
            // SerialPutChar('n');
            if (buffP[bytes_read - 1] == '\r')
                break;
        }

        if (c == '\b') /* Backspace */
        {
            if (bytes_read > 0)
            {
                // SerialPutString("\b \b");
                bytes_read--;
            }
            continue;
        }
        if (bytes_read >= CMD_STRING_SIZE)
        {
            SerialPutString("Cmd size over.\r\n");
            bytes_read = 0;
            continue;
        }
        if ((c >= 0x20 && c <= 0x7E) || c == '\r')
        {
            buffP[bytes_read++] = c;
            // SerialSendChar(c);
        }
    } while (1);
    buffP[bytes_read - 1] = '\0';  // 替换'\r'
}

/**
 * @brief  阻塞性的接收字节函数
 * @param  None
 * @retval 接收到的字节数据
 */
uint8_t GetKey(void)
{
    uint8_t key = 0;
    /* Waiting for user input */
    while (1)
    {
        if (SerialKeyPressed((uint8_t*)&key))
            break;
    }
    return key;
}

/**
 * @brief  半阻塞性的接收字节函数
 * @param  c: Character
 * @param  timeout: Timeout
 * @retval 1: Byte received
 *         0: Timeout
 */
uint8_t Receive_Byte(uint8_t* c, uint32_t timeout)
{
    while (timeout-- > 0)
    {
        if (SerialKeyPressed(c) == 1)
        {
            return 1;
        }
    }
    return 0;
}

/****************20250402新增：启动时一段时间内持续监听更新命令******************/
/**
 * @brief  bootloader启动时持续监听是否发来更新命令
 * @note   采用查询定时器方法判断是否超时
 * @param  timeout 持续监听时间，单位ms
 * @retval None
 */
void wait_for_update(uint32_t timeout)
{
    uint8_t cmdStr[CMD_STRING_SIZE];
    int16_t i;

    memset(cmdStr, 0, sizeof(cmdStr)); /* 清空接收缓冲 */

    /* 当Flag不处于更新状态 */
    if (IAP_ReadFlag() != UPDATE_FLAG_DATA)
    {
        TIM2_init(); /* 提供：1KHz向上递增计数器接口 */

        for (i = 0; i < CMD_STRING_SIZE; i++)
        {
            /* 如果接收到字符会追加到cmdStr末尾 */
            if (Receive_Byte(cmdStr + i, 0xFFFF) == 0)
            {
                i--;  // 未接收到字符
            }

            /* 判断接收的数据里面是否包含“update\r\n” */
            if (strstr((char*)cmdStr, WITH_NEWLINE(CMD_UPDATE_STR)) != NULL)
            {
                IAP_WriteFlag(UPDATE_FLAG_DATA);
                delay_us(10); /* 加延时防止后面出现读取Flag异常 */
                return;
            }

            /* 到达监听时间 */
            if (Is_time_passed(timeout))
            {
                SerialPrint("timeout.\r\n");
                return;
            }
        }
        SerialPrint("overflow.\r\n");
    }
}

/**
 * @brief  轮询检查是否过了 timeout（从TIM2_Init开始计时）
 * @param  timeout 超时时间，单位ms
 * @retval
 */
static uint8_t Is_time_passed(uint32_t timeout)
{
    static uint32_t last_time = 0;
    uint32_t current_time = get_TIM2Counter(); /* 提供：获取当前定时器计数值接口 */

    if ((current_time - last_time) >= timeout)
    {
        last_time = current_time;  // 更新时间戳
        return 1;                  // 返回 1 表示超时时间已到
    }
    return 0;  // 返回 0 表示超时时间未到
}
