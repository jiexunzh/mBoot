#ifndef BSP_FLASH_H
#define BSP_FLASH_H

#include <stdint.h>

void flash_write_data(uint32_t addr, uint32_t* data, uint32_t length);
void flash_read_data(uint32_t addr, uint32_t* buffer, uint32_t length);

#endif /* BSP_FLASH_H */
