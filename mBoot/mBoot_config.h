/**
 * @file mBoot_config.h
 * @brief 本文件声明了mBoot部署使用的硬件接口、功能配置、区域地址划分以及相关命令字段标志等，
          "mBoot_bootloader"和"mBoot_app_full"共享。
 * @author ZJX
 * @date 2025-06-19
 * @version 1.0
 */
#ifndef MBOOT_CONFIG_H
#define MBOOT_CONFIG_H

/* "USE_LISTEN_UPDATE"说明：
 * 启用该宏则会在MCU上电时监听更新命令一段时间，适用于对上电时实时性要求不高的场景。
 * 如果希望上电时MCU能快速响应，则应关闭或采用GPIO检测等其他方案。
 * 优点：
 * 1、降低APP开发的复杂度，不用在APP监听更新命令并响应，只需修改IROM1和中断向量偏移即可
 * 2、当APP波特率与mBoot波特率不一致时，上位机不用做波特率对应切换
 * 3、通过上位机和MCU上电的配合，能解决更新错固件等其他意外情况导致的变砖
 * 4、基于MCU内部定时器，不依赖板级硬件，相比上电GPIO检测方案更通用
 * 缺点：
 * 1、每次上电都需要等待一段时间才会跳转到APP
 * 2、增加了升级操作的复杂度，点击上位机的“更新固件”按钮后，必须在规定时间内重新上电
 * 3、基于MCU内部定时器，增加Bootloader大小
 */
#define USE_LISTEN_UPDATE  1    /**< 使用上电监听更新命令功能 */
#define LISTEN_UPDATE_TIMEOUT 1000 /**< 上电监听功能时间，单位ms */
#define USE_RS485                  1    /**< 使用RS485 */

/* App区域，根据不同芯片进行修改 */
#define ApplicationAddress         (0x08000000 + 16 * 1024)
/* App和Bootloader共享信息区域，根据不同芯片进行修改 */
#define mBoot_FLAG_ADDR            (ApplicationAddress - 2 * 1024)

/* 调试信息打印开关 */
#define ENABLE_PUTSTR              0

/* 功能命令字段 */
#define CMD_MENU_STR               "menu"          /**< bootloader主菜单命令 */
#define CMD_RUNAPP_STR             "runapp"        /**< 运行APP命令 */
#define CMD_UPDATE_STR             "update"        /**< 更新命令 */
#define CMD_ERASE_STR              "erase"         /**< 擦除Flash命令 */
#define CMD_UPLOAD_STR             "upload"        /**< 上传Flash 命令*/
#define CMD_DISWP_STR              "diswp"         /**< 取消读写保护命令 */
#define CMD_STRING_SIZE            ((uint16_t)128) /**< 命令的最大长度 */

/* 固件更新功能字段 */
#define RESPONSE_OK_CMD            "OK"       /**< 上位机发送更新命令后，MCU应答，可以正式开始固件更新了 */
#define BINSIZE_CMD                "BINSIZE=" /**< 上位机发送BIN文件大小的前置字符串 */
#define RECEIVED_CMD               "RECV"     /**< 上位机发送一个固件数据包后，MCU应答，接收到上位机的更新数据包了且校验通过 */
#define RECEIVED_ERROR_CMD         "ERROR"    /**< MCU应答，数据校验未通过或发生其他错误 */

/* 预处理阶段拼接字符串 */
#define NEW_LINE(cmd)              cmd "\r\n"

/* 状态标志，存储在共享信息区，以字为单位（有些芯片不支持半字访问） */
#define INIT_FLAG_DATA             ((uint32_t)(0xFFFFFFFF)) /**< 初始默认标志（空板子的情况） */
#define UPDATE_FLAG_DATA           ((uint32_t)(0xEEEEEEEE)) /**< 更新固件标志 */
#define ERASE_FLAG_DATA            ((uint32_t)(0xDDDDDDDD)) /**< 擦除固件标志 */
#define UPLOAD_FLAG_DATA           ((uint32_t)(0xCCCCCCCC)) /**< 上传固件标志 */
#define APPRUN_ERROR_FLAG_DATA     ((uint32_t)(0xBBBBBBBB)) /**< 更新失败标志 */
#define APPRUN_FLAG_DATA           ((uint32_t)(0x5A5A5A5A)) /**< APP运行标志 */

#endif /* MBOOT_CONFIG_H */
