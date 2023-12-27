#pragma once
#include <locale>	//区域需要，设定新的区域设定（为了能保存中文）
#include <afxdb.h>	//本地数据库需要



//返回结果定义
#define RTN_SUCCESS					0
#define RTN_ERROR					-1
#define RTN_FAIL					1
#define RTN_INIT					2

//
#define OP_CHOOSE					0
#define OP_ADD_NEW					1
#define OP_EDT_CUR					2
#define OP_DEL_CUR					3

#define PROCESS_BARCODE_17				0//有SN
#define PROCESS_BARCODE_2				1//无SN只写标志位

//3个共用的显示信息控件
#define WM_MSGBOX_MSG						WM_USER + 1000
//#define WM_REDIT_INFO_MSG					WM_USER + 1001
#define WM_STATE_MSG						WM_USER + 1002
#define WM_REDIT_TRACE_MSG					WM_USER + 1003
#define WM_CLEAR_ALL_INFO_MSG				WM_USER + 1004
#define WM_UPDATE_LOC_MO_MSG				WM_USER + 1005
#define WM_UPDATE_SQL_MO_STD_MSG			WM_USER + 1006
#define WM_SEARCH_SQL_MO_STD_MSG			WM_USER + 1007
#define WM_UPDATE_MO_CONFIG_MSG				WM_USER + 1008
#define WM_EDIT_SQL_MO_LITE_CONFIG_MSG		WM_USER + 1009

#define	SHOW_INFO			0
#define	SHOW_PASS			1
#define	SHOW_FAIL			2

#define	PWR_ON_NORMAL				0
#define	PWR_ON_23_ZBX_EVADE			1
#define	PWR_ON_COM_3_EVADE			2
#define	PWR_ON_52_ZBX_EVADE			3

//直播星规避初始化时间指令
#define START_CMD_1				0xA0
#define START_CMD_ECHO_1		0x5F
#define START_CMD_2				0x0A
#define START_CMD_ECHO_2		0xF5
#define START_CMD_3				0x50
#define START_CMD_ECHO_3		0xAF
#define START_CMD_4				0x05
#define START_CMD_ECHO_4		0xFA

#define WRITE_CMD				0xA1
#define JUMP_CMD				0xA8

//串口3规避初始化时间指令
#define SYNC_WORD				0xC5
#define SYNC_WORD_RSP			0x5C
#define SYNC_WORD2				0xB9
#define SYNC_WORD_SUCCESS		0x9B

//数据库来源分类
#define DB_LOC					0
#define DB_SQL_ALL				1
#define DB_SQL_SEARCH			2


//GPIB卡传输协议
#define GPIB_NI					0
#define GPIB_VISA				1
#define GPIB_VISA_TCPIP			2



