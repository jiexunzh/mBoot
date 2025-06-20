/**
 ******************************************************************************
 * @file    utils_intstr.h
 * @brief   整数与字符串转换函数头文件
 *
 * @details 定义将整数转字符串、将字符串转整数的函数原型，以及
 *          辅助字符宏判断（十进制/十六进制合法性）与转换宏。
 *          可配合串口命令解析模块使用。
 *
 * @note    默认支持十进制格式的转换，可扩展支持十六进制。
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#ifndef UTILS_INTSTR_H
#define UTILS_INTSTR_H

#include <stdint.h>

#define IS_AF(c)            ((c >= 'A') && (c <= 'F'))
#define IS_af(c)            ((c >= 'a') && (c <= 'f'))
#define IS_09(c)            ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)       IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)       IS_09(c)
#define CONVERTDEC(c)       (c - '0')

#define CONVERTHEX_alpha(c) (IS_AF(c) ? (c - 'A' + 10) : (c - 'a' + 10))
#define CONVERTHEX(c)       (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

void Int2Str(char* str, int32_t intnum);
uint32_t Str2Int(char* inputstr, int32_t* intnum);

#endif /* UTILS_INTSTR_H */
