#include "iap_main.h"

int main(void)
{
    uint32_t flag = 0;

    iap_init();
	
	/* �ж��Ƿ��Ѿ�ͨ��������ʽ���������ȣ�������APP����������±�־ */
    detect_app_flashed();

    listen_update_command(1000);
	
    while (1)
	{
        flag = IAP_ReadFlag();
        if (flag == APPRUN_FLAG_DATA)
        {
            if (IAP_RunApp() == 0)
            {
                IAP_WriteFlag(APPRUN_ERROR_FLAG_DATA);  // ����ʧ��
            }
        }
        else if (flag == INIT_FLAG_DATA || flag == APPRUN_ERROR_FLAG_DATA)
        {
            IAP_Main_Menu();
        }
        else if (flag == UPDATE_FLAG_DATA)
        {
            if (IAP_Update())  // ���³ɹ�
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


