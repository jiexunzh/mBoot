/**
 ******************************************************************************
 * @file    main.c
 * @brief   mBoot Bootloader 的主函数入口
 *
 * @details 启动后初始化串口、定时器和方向控制引脚，
 *          检查是否为首次上电或调试器刷入 APP 的情况，
 *          并根据标志判断跳转 APP、进入主菜单或执行固件更新等操作。
 *
 * @note    该主循环是整个 Bootloader 的控制核心逻辑。
 *          注意：本示例使用MCU内部晶振！
 *
 * @author  ZJX
 * @date    2025-06-20
 * @version 1.0
 ******************************************************************************
 */
#include "mBoot_main.h"
#include "mBoot_config.h"

int main(void)
{
    uint32_t flag;

    mBoot_init();

    detect_app_flashed();

    listen_update_command(LISTEN_UPDATE_TIMEOUT);

    while (1)
    {
        flag = mBoot_read_flag();
        if (flag == APPRUN_FLAG_DATA)
        {
            /* 运行失败 */
            if (mBoot_run_app() == 0)
            {                
                mBoot_write_flag(APPRUN_ERROR_FLAG_DATA);
            }
        }
        else if (flag == INIT_FLAG_DATA || flag == APPRUN_ERROR_FLAG_DATA)
        {
            /* 主菜单界面 */
            mBoot_main_menu();
        }
        else if (flag == UPDATE_FLAG_DATA)
        {
            /* 更新成功 */
            if (mBoot_update())
            {                
                mBoot_write_flag(APPRUN_FLAG_DATA);
            }
            else
            {
                mBoot_write_flag(APPRUN_ERROR_FLAG_DATA);
            }
        }
        //    else if (flag == ERASE_FLAG_DATA)
        //    {
        //        if (mBoot_erase())
        //        {
        //            mBoot_write_flag(INIT_FLAG_DATA);
        //        }
        //        else
        //        {
        //            mBoot_write_flag(APPRUN_ERROR_FLAG_DATA);
        //        }
        //    }
        //    else if (flag == UPLOAD_FLAG_DATA)
        //    {
        //        if (mBoot_upload())
        //        {
        //            mBoot_write_flag(APPRUN_FLAG_DATA);
        //        }
        //        else
        //        {
        //            mBoot_write_flag(APPRUN_ERROR_FLAG_DATA);
        //        }
        //    }
        else
        {
        }
    }
}
