#include "bsp_flash.h"
#include "gd32c11x.h"

/**
  * @brief  按字写入数据
  * @param  addr  必须是页的起始地址
  * @param  length  长度最大不能超过256
  * @retval 
  */
void flash_write_data(uint32_t addr, uint32_t* data, uint32_t length)
{
    // 解锁Flash
    fmc_unlock();

    // 擦除Flash页面
    fmc_page_erase(addr);

    for (uint32_t i = 0; i < length; i++)
    {
        fmc_word_program(addr + i * 4, data[i]);
    }

	// 清除数据缓存，避免后续读取到旧的脏数据
	fmc_dbus_reset();	// 先决条件：fmc_unlock
	
    // 锁定Flash
    fmc_lock();
}

/**
 * @brief  按字读取数据
 * @param  addr  必须是页的起始地址
 * @param  length  长度最大不能超过256
 * @retval 
 */
void flash_read_data(uint32_t addr, uint32_t* buffer, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        buffer[i] = *(volatile uint32_t*)(addr + i * 4);  // 直接取对应字的地址
    }
}
