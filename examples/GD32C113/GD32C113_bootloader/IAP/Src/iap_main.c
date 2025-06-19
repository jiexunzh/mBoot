#include <string.h>
#include "iap_main.h"
#include "iap_update.h"
#include "iap_port.h"
#include "iap_config.h"
#include "utils_intstr.h"

typedef void (*pFunction)(void);

void iap_init(void)
{
	iap_uart_init();
	iap_rs485_dir_init();
	iap_rs485_dir_rx();
	iap_timer_init();
}

void iap_transmit_string(uint8_t* buf)
{
	iap_rs485_dir_tx();
    while (*buf != '\0')
    {
        iap_uart_transmit_byte(*buf);
        buf++;
    }

    while (is_iap_uart_transmit_complete() == 0)
        ;
	iap_rs485_dir_rx();
}

/**
 * @brief  打印调试信息，可通过宏屏蔽
 */
void iap_print(char* s)
{
#if (ENABLE_PUTSTR == 1)
    iap_transmit_string((uint8_t *)s);
#endif
}

void clear_iap_uart_received(void)
{
    if (is_iap_uart_receive_notEmpty() == 1)
    {
        (void)iap_uart_receive_byte();  // 读取数据寄存器，丢弃数据
    }    
}

/**
 * @brief  阻塞性的接收字节/符函数
 * @param  c 字节数据指针
 * @param  timeout 阻塞时间，0xFFFFFFFF-永久阻塞
 * @retval 0-接收超时；1-接收成功
 */
uint8_t iap_receive_byte(uint8_t* c, uint32_t timeout)
{
    if (timeout == 0xFFFFFFFF)
    {
        while(1)
        {
            /* 串口接收寄存器非空 */
            if (is_iap_uart_receive_notEmpty() == 1)
            {
                *c = iap_uart_receive_byte();
                return 1;
            }
        }
    }
    else
    {
		while (timeout-- > 0)
		{
            /* 串口接收寄存器非空 */
            if (is_iap_uart_receive_notEmpty() == 1)
            {
                *c = iap_uart_receive_byte();
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
 * @retval 
 */
uint8_t iap_receive_string(uint8_t* buf, uint32_t timeout)
{
    uint16_t bytes_read = 0;
    uint8_t c = 0;

    clear_iap_uart_received();  // 清除串口接收寄存器，防止第一个字符接收到之前的数据
    do
    {
        if (iap_receive_byte(&c, timeout) == 0)
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
            iap_transmit_string((uint8_t* )"Cmd size over.\r\n");
            bytes_read = 0;
            continue;
        }
        if ((c >= 0x20 && c <= 0x7E) || c == '\r')
        {
            buf[bytes_read++] = c;
            // SerialSendChar(c);
        }
    } while (1);
    buf[bytes_read - 1] = '\0';  // 替换'\r'
	return 1;
}

// 判断APP的堆栈指针(SP)是否指向 SRAM 地址范围
uint8_t check_app_valid(void)
{
    // 检查 SP 的 bit 17-31, 但这个比较并不准确，可以直接采用准确的范围比较方式
    if (((*(volatile uint32_t*)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint32_t IAP_ReadFlag(void)
{
    uint32_t flag = 0;
    iap_read_flash(IAP_FLAG_ADDR, &flag, 1);
    return flag;
}

void IAP_WriteFlag(uint32_t flag)
{
    iap_write_flash(IAP_FLAG_ADDR, &flag, 1);
}

/**
 * @brief  检查是否已经通过下载了APP程序
 *         通过查看堆栈指针是否位于 SRAM 地址范围
 *         来判断是否已经下载了APP程序
 *         堆栈指针: 总是指向栈顶
 */
void detect_app_flashed(void)
{
    uint32_t flag;
    char buf[20] = {0};

    // 如果是空板状态
    if (IAP_ReadFlag() == INIT_FLAG_DATA)
    {
        /* 检查堆栈指针是否指向RAM，如果是则说明实际上可能已经通过其他途径刷入了程序 */
        if (check_app_valid())
        {
            iap_print("try to jump app.\r\n");
            IAP_WriteFlag(APPRUN_FLAG_DATA); /* 写入“APP运行标志” */
            iap_delay_us(10);                /* 不加延时可能会出现读取异常: IAP_ReadFlag error: 4294967295 */
            flag = IAP_ReadFlag();
            if (flag != APPRUN_FLAG_DATA)
            {
                /* 写入或读取异常 */
                Int2Str(buf, flag);
                iap_transmit_string((uint8_t *)"read error: ");
                iap_transmit_string((uint8_t *)buf);
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
        iap_print("Jump to app.\r\n");
        /* 关闭所有中断（当前BOOT并未使用任何中断） */
        // __disable_irq();

        /* 取出APP复位中断向量Reset_Handler的地址为跳转地址 */
        JumpAddress = *(volatile uint32_t*)(ApplicationAddress + 4);
        Jump_To_Application = (pFunction)JumpAddress;   /* 设置跳转函数指针 -> APP Reset_Handler */
        __set_MSP(*(volatile uint32_t*)ApplicationAddress); /* 设置MSP为APP MSP的初始值 */
        Jump_To_Application();                          /* PC指针正式跳转到 APP复位中断向量的入口，开始运行APP程序 */
        return 1;
    }
    else /* APP的MSP不指向RAM，不跳转 */
    {
        iap_transmit_string((uint8_t *)"Jump to app error.\r\n");
        return 0;
    }
}

/************************************************************************/
void IAP_Main_Menu(void)
{
    uint8_t cmdStr[CMD_STRING_SIZE] = {0};

    while (1)
    {
        iap_transmit_string((uint8_t *)"\r\nIAP Main Menu (V 1.0.0)\r\n");
        iap_transmit_string((uint8_t *)NEW_LINE(CMD_UPDATE_STR));  // update\r\n
        iap_transmit_string((uint8_t *)NEW_LINE(CMD_UPLOAD_STR));  // upload\r\n
        iap_transmit_string((uint8_t *)NEW_LINE(CMD_ERASE_STR));   // erase\r\n
        iap_transmit_string((uint8_t *)NEW_LINE(CMD_MENU_STR));    // menu\r\n
        iap_transmit_string((uint8_t *)NEW_LINE(CMD_RUNAPP_STR));  // runapp\r\n

        iap_receive_string(cmdStr, 0xFFFFFFFF); /* 以换行为结束标识符 */

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
            iap_transmit_string((uint8_t *)" Invalid CMD !\r\n");
        }
    }
}

/************************************************************************/
uint8_t IAP_Update(void)
{
    char strNumber[10] = "";
    int32_t Size = 0;

    Size = firmware_update_process();
    iap_delay_us(10000);   // 留给上位机一点时间切换为接收回显模式
    if (Size > 0)  // 升级成功
    {
        iap_transmit_string((uint8_t *)"Update finished!\r\n");
        Int2Str(strNumber, Size);
        iap_transmit_string((uint8_t *)"APP size: ");
        iap_transmit_string((uint8_t *)strNumber);
        iap_transmit_string((uint8_t *)" Bytes.\r\n");
        return 1;
    }
    else
    {
        iap_transmit_string((uint8_t *)"Update failed!\r\n");		
        Size = -Size;
        Int2Str(strNumber, Size);  // 只支持非负数
        iap_transmit_string((uint8_t *)"error code: -");
        iap_transmit_string((uint8_t *)strNumber);
        return 0;
    }
}

uint8_t IAP_Upload(void)
{
    iap_transmit_string((uint8_t *)"\r\ndeveloping...\r\n");
    return 1;
}

uint8_t IAP_Erase(void)
{
    iap_transmit_string((uint8_t *)"\r\ndeveloping...\r\n");
    return 1;
}

/**
 * @brief  轮询检查是否过了 timeout（从TIM2_Init开始计时）
 * @param  timeout 超时时间，单位ms
 * @retval
 */
uint8_t is_time_passed(uint32_t timeout)
{
    static uint32_t last_time = 0;
    uint32_t current_time = iap_get_timer_counter();

    if ((current_time - last_time) >= timeout)
    {
        last_time = current_time;  // 更新时间戳
        return 1;                  // 返回 1 表示超时时间已到
    }
    return 0;  // 返回 0 表示超时时间未到
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
	if (iap_get_timer_counter() == 0xFFFFFFFF)
	{
		return;
	}    

    /* 如果不处于更新状态 */
    if (IAP_ReadFlag() != UPDATE_FLAG_DATA)
    {
		memset(cmdStr, 0, sizeof(cmdStr)); /* 清空接收缓冲 */
		
        for (i = 0; i < CMD_STRING_SIZE; i++)
        {
            /* 如果接收到字符会追加到cmdStr末尾 */
            if (iap_receive_byte(cmdStr + i, 0xFFFF) == 0)
            {
                i--;  // 未接收到字符
            }

            /* 判断接收的数据里面是否包含“update\r\n” */
            if (strstr((char*)cmdStr, NEW_LINE(CMD_UPDATE_STR)) != NULL)
            {
                IAP_WriteFlag(UPDATE_FLAG_DATA);
                iap_delay_us(10); /* 加延时防止后面出现读取Flag异常 */
                return;
            }

            /* 到达监听时间 */
            if (is_time_passed(nms))
            {
                iap_print("listen timeout.\r\n");
                return;
            }
        }
        iap_print("Cmd size over.\r\n");
    }
}
