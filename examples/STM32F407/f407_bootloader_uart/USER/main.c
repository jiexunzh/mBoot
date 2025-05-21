#include "iap.h"

int main(void)
{
    uint32_t flag = 0;

    IAP_Init();
	
	/* �ж��Ƿ��Ѿ���ͨ���������ȣ�������APP����������±�־ */
    detect_app_flashed();  
	
	/* ������������һ��ʱ�䣬�����ڶԸ��ϵ�ʱʵʱ��Ҫ�󲻸ߵ�Ӧ�ã�����Ӧ�������ż�����������		
		�ô���
		1��������APP�������������APP�������Ӷȣ��޸�IROM1���ж�����ƫ�Ƽ��ɣ�
		2���������´�̼����߸���ʧ�ܺ��µ��������ש
		3������Ӳ���϶�����������
		������
		1��ÿ���ϵ���Ҫ�ȴ�һ��ʱ��
		2�������Ը��ӣ���Ҫ���¸��¹̼���ť����������ϵ磨APP�޼������ش�������£�
		3�����붨ʱ��������Bootloader�̼���С
	*/
    wait_for_update(1000); 

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
