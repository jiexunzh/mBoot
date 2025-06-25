/**
 ******************************************************************************
 * @file    main.c
 * @brief   mBoot Bootloader �����������
 *
 * @details �������ʼ�����ڡ���ʱ���ͷ���������ţ�
 *          ����Ƿ�Ϊ�״��ϵ�������ˢ�� APP �������
 *          �����ݱ�־�ж���ת APP���������˵���ִ�й̼����µȲ�����
 *
 * @note    ����ѭ�������� Bootloader �Ŀ��ƺ����߼���
 *          ע�⣺��ʾ��ʹ��MCU�ڲ�����
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
            /* ����ʧ�� */
            if (mBoot_run_app() == 0)
            {                
                mBoot_write_flag(APPRUN_ERROR_FLAG_DATA);
            }
        }
        else if (flag == INIT_FLAG_DATA || flag == APPRUN_ERROR_FLAG_DATA)
        {
            /* ���˵����� */
            mBoot_main_menu();
        }
        else if (flag == UPDATE_FLAG_DATA)
        {
            /* ���³ɹ� */
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
