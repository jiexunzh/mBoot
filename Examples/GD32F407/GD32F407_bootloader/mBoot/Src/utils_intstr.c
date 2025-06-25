/**
 ******************************************************************************
 * @file    utils_intstr.c
 * @brief   整数与字符串转换函数实现
 *
 * @details 提供将整数转换为字符串（十进制）、将字符串解析为整数的工具函数。
 *          常用于串口接收文本命令时的数值提取与响应。
 *          不支持负数转换，需由上层调用者保证输入合法性。
 *
 * @note    本文件配套 utils_intstr.h 使用，常用于 Bootloader 命令解析阶段。
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#include "utils_intstr.h"

/**
 * @brief   将整数转换为字符串（十进制形式）
 * @param   str 输出字符串缓冲区（保证足够大）
 * @param   intnum 要转换的整数值（不支持负数）
 */
void Int2Str(char* str, int32_t intnum)
{
    uint32_t i, Div = 1000000000, j = 0, Status = 0;

    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + 48;

        intnum = intnum % Div;
        Div /= 10;
        if ((str[j - 1] == '0') & (Status == 0))
        {
            j = 0;
        }
        else
        {
            Status++;
        }
    }
}

/**
 * @brief   将字符串转换为整数（十进制）
 * @param   inputstr 输入的字符串指针
 * @param   intnum 转换结果指针，转换成功则输出整数值
 * @retval  1 表示转换成功，0 表示格式错误或超出范围
 */
uint32_t Str2Int(char* inputstr, int32_t* intnum)
{
    uint32_t i = 0, res = 0;
    uint32_t val = 0;

    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
    {
        if (inputstr[2] == '\0')
        {
            return 0;
        }
        for (i = 2; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1; */
                res = 1;
                break;
            }
            if (ISVALIDHEX(inputstr[i]))
            {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* over 8 digit hex --invalid */
        if (i >= 11)
        {
            res = 0;
        }
    }
    else /* max 10-digit decimal input */
    {
        for (i = 0; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1 */
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
            {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
            {
                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            }
            else if (ISVALIDDEC(inputstr[i]))
            {
                val = val * 10 + CONVERTDEC(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* Over 10 digit decimal --invalid */
        if (i >= 11)
        {
            res = 0;
        }
    }

    return res;
}
