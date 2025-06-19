#include "iap_main.h"

int main(void)
{
    uint32_t flag = 0;

    iap_init();
	
	/* 判断是否已经通过其他方式（调试器等）下载了APP程序，有则更新标志 */
    detect_app_flashed();

    listen_update_command(1000);
	
    while (1)
	{
        flag = IAP_ReadFlag();
        if (flag == APPRUN_FLAG_DATA)
        {
            if (IAP_RunApp() == 0)
            {
                IAP_WriteFlag(APPRUN_ERROR_FLAG_DATA);  // 运行失败
            }
        }
        else if (flag == INIT_FLAG_DATA || flag == APPRUN_ERROR_FLAG_DATA)
        {
            IAP_Main_Menu();
        }
        else if (flag == UPDATE_FLAG_DATA)
        {
            if (IAP_Update())  // 更新成功
            {
                IAP_WriteFlag(APPRUN_FLAG_DATA);
            }
            else
            {
                IAP_WriteFlag(APPRUN_ERROR_FLAG_DATA);
            }
        }
        else if (flag == UPLOAD_FLAG_DATA)
        {
            if (IAP_Upload())
            {
                IAP_WriteFlag(APPRUN_FLAG_DATA);
            }
            else
            {
                IAP_WriteFlag(APPRUN_ERROR_FLAG_DATA);
            }
        }
        else if (flag == ERASE_FLAG_DATA)
        {
            IAP_Erase();
            IAP_WriteFlag(INIT_FLAG_DATA);
        }
        else
        {
            /* code */
        }		
    }
}


