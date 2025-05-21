#include "iap.h"

int main(void)
{
    uint32_t flag = 0;

    IAP_Init();
	
	/* 判断是否已经（通过调试器等）下载了APP程序，有则更新标志 */
    detect_app_flashed();  
	
	/* 监听更新命令一段时间，适用于对刚上电时实时性要求不高的应用，否则应采用引脚检测等其他方案		
		好处：
		1、不用在APP监听更新命令，简化APP开发复杂度（修改IROM1和中断向量偏移即可）
		2、解决因更新错固件或者更新失败后导致的升级后变砖
		3、无需硬件上额外引出引脚
		坏处：
		1、每次上电需要等待一段时间
		2、操作略复杂，需要按下更新固件按钮后快速重新上电（APP无监听返回代码情况下）
		3、引入定时器，增加Bootloader固件大小
	*/
    wait_for_update(1000); 

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
