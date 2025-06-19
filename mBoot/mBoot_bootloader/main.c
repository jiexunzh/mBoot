#include "mBoot_main.h"

int main(void)
{
    uint32_t flag = 0;

    mBoot_init();

    /* 判断是否已经通过其他方式（调试器等）下载了APP程序，有则更新标志 */
    detect_app_flashed();

    listen_update_command(LISTEN_UPDATE_TIMEOUT);

    while (1)
    {
        flag = mBoot_read_flag();
        if (flag == APPRUN_FLAG_DATA)
        {
            if (mBoot_run_app() == 0)
            {
                mBoot_write_flag(APPRUN_ERROR_FLAG_DATA);  // 运行失败
            }
        }
        else if (flag == INIT_FLAG_DATA || flag == APPRUN_ERROR_FLAG_DATA)
        {
            mBoot_main_menu();
        }
        else if (flag == UPDATE_FLAG_DATA)
        {
            if (mBoot_update())  // 更新成功
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
