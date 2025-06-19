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
