#include "mBoot_main.h"

int main(void)
{
    uint32_t flag = 0;

    mBoot_init();

    /* �ж��Ƿ��Ѿ�ͨ��������ʽ���������ȣ�������APP����������±�־ */
    detect_app_flashed();

    listen_update_command(LISTEN_UPDATE_TIMEOUT);

    while (1)
    {
        flag = mBoot_read_flag();
        if (flag == APPRUN_FLAG_DATA)
        {
            if (mBoot_run_app() == 0)
            {
                mBoot_write_flag(APPRUN_ERROR_FLAG_DATA);  // ����ʧ��
            }
        }
        else if (flag == INIT_FLAG_DATA || flag == APPRUN_ERROR_FLAG_DATA)
        {
            mBoot_main_menu();
        }
        else if (flag == UPDATE_FLAG_DATA)
        {
            if (mBoot_update())  // ���³ɹ�
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
