/**
 * @file iap_config.h
 * @brief 本文件声明了功能接口配置、声明区域地址、IAP相关的命令字段等
 * @author ZJX
 * @date 2025-06-18
 * @version 1.0
 */
#ifndef IAP_CONFIG_H
#define IAP_CONFIG_H

/*  "USE_LISTEN_UPDATE_COMMAND"说明： 
	监听更新命令一段时间，适用于对刚上电时实时性要求不高的应用，否则应采用引脚检测等其他方案		
	优点：
	1、降低APP复杂度，不用在APP监听和响应更新命令（修改IROM1和中断向量偏移即可），上位机也不用切换波特率
	2、防止因更新错固件等其他意外情况导致的升级后变砖
	3、无需硬件上额外引出引脚
	缺点：
	1、每次上电需要等待一段时间
	2、操作略复杂，需要在上位机点击“更新固件按钮”后迅速重新上电（APP无监听返回代码情况下）
	3、引入定时器，增加Bootloader固件大小
*/    
#define USE_LISTEN_UPDATE_COMMAND 1 /**< 使用上电监听更新命令功能 */
#define USE_RS485                 1 /**< 使用RS485 */

/* App区域地址，根据不同芯片进行修改 */
#define ApplicationAddress (0x08000000 + 16 * 1024)
/* App和Bootloader共享信息的地址，根据不同芯片进行修改 */
#define IAP_FLAG_ADDR      (ApplicationAddress - 2 * 1024)

/* 调试信息打印开关 */
#define ENABLE_PUTSTR             0

/* Bootloader IAP命令 -----------------------------------------*/
#define CMD_STRING_SIZE        ((uint16_t)128)       /* 命令字符串的最大长度 */
#define CMD_MENU_STR              "menu"   /* bootloader主菜单界面 */
#define CMD_RUNAPP_STR            "runapp" /* 运行APP */
#define CMD_UPDATE_STR            "update" /* 更新 */
#define CMD_ERASE_STR             "erase"  /* 擦除Flash */
#define CMD_UPLOAD_STR            "upload" /* 上传Flash */
#define CMD_DISWP_STR             "diswp"  /* 取消读写保护 */

/* Umodem固件更新协议字段 -----------------------------------------*/
#define RESPONSE_OK_CMD           "OK"       /* 上位机发送CMD_UPDATE_STR后，MCU应答，可以正式开始固件更新了 */
#define BINSIZE_CMD               "BINSIZE=" /* 上位机发送BIN文件大小的前置字符串 */
#define RECEIVED_CMD              "RECV"     /* 上位机发送一个固件数据包后，MCU应答，接收到上位机的更新数据包了且校验通过 */
#define RECEIVED_ERROR_CMD        "ERROR"    /* MCU应答，数据校验未通过或发生其他错误 */

/* 字符串字面量在预处理阶段进行拼接 */
#define NEW_LINE(cmd)         cmd "\r\n"

/* 以字为单位 */
#define INIT_FLAG_DATA         ((uint32_t)(0xFFFFFFFF))  // 默认标志的数据(空片子的情况)
#define UPDATE_FLAG_DATA       ((uint32_t)(0xEEEEEEEE))  // 下载标志的数据
#define ERASE_FLAG_DATA        ((uint32_t)(0xDDDDDDDD))  // 擦除标志的数据
#define UPLOAD_FLAG_DATA       ((uint32_t)(0xCCCCCCCC))  // 上传标志的数据
#define APPRUN_ERROR_FLAG_DATA ((uint32_t)(0xBBBBBBBB))  // 更新或升级失败标志
#define APPRUN_FLAG_DATA       ((uint32_t)(0x5A5A5A5A))  // APP不需要做任何处理，直接运行状态

#endif /* IAP_CONFIG_H */
