#pragma once
#include "gfc.h"
#include "Com_Port.h"


typedef enum tagBB_CHIP_ASR_TYPE
{
	ASR1802 = 0,
	ASR3601 = 1,
	
}BB_CHIP_ASR_TYPE;


#define  FAIL_SET_PROD_MODE     0x03003001
#define  FAIL_SET_SAW_BAND		0x03003002
#define  FAIL_FIND_VERSION      0x03003003
#define  FAIL_OPEN_PORT			0x03003004
#define  FAIL_FIND_PORT         0x03003005

#define DL_FW_MCU_FAIL1  1096
#define DL_FW_MCU_FAIL2  1097
#define DL_FW_MCU_FAIL3  1098
#define DL_FW_MCU_FAIL4  1099
#define DL_FW_MCU_FAIL5  1100
typedef struct tagZIP_FILE_S
{
	CString cs_Name;
	CString cs_Path;
	bool b_Enable_DL;
}ZIP_FILE_S;


class CARM_OP_ASR
{
public:
	CARM_OP_ASR(void);
	~CARM_OP_ASR(void);

	HWND m_hWnd_View;
	bool m_bRuning;
	int m_iStop_Flag;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;

	bool m_bGet_Debug_Log;


	int m_Ts_Num;
	CGFC m_cla_GFC;
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);
	int Do_Execute(void);
	bool LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path);
	int m_iOP_TYPE;
	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);

	CString m_csCurr_Exe_Path;

	int m_iCom_Port;
	int m_iSec_Port;//第二端口

	int m_iAT_Port;//at port


	void DO_DL_OP(int i_Index);
	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);

	
	int  m_PassNumb;
	int  m_ErrorNumb;

	CCom_Port m_cla_Com_Port;
	CString  mAT_IMEI;
	CString  mAT_FirewareName;
	CString  mDB_FirewareName;

	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);

	CString  mBar_IMEI;//总控提供的IMEI
	CString  mBar_SN;//总控提供的SN

	ZIP_FILE_S m_str_ZIP_FW;

	ZIP_FILE_S m_str_MCU_FW;

	BB_CHIP_ASR_TYPE m_eu_BB_Chip;

	bool Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut);

	bool IsAT_Port(int iPort);

	//AT+QDOWNLOAD=1
	bool SendQD_Mode(int iPort);

	void  AT_SendQD_Mode(int iPort);

	int AT_DO_OP(void);

	bool Read_IMEI(wchar_t * wcp_IMEI);

	//查询是否可以查询到版本号
	int FindVer();

	CString m_strAT_FirewareName;

	void FindAT_Switch_DL_Mode(void);

	bool m_bWaitTestEventStart;

	bool m_b120M_Flag;
	bool Set_Prod_Mode(const int iMode);

	bool Set_Saw_Band(const int Band);

	CString  m_strOC;
	bool FindHexFile(CString lpPath);

	bool CreatQDM512_Jlink_CMD(void);

	int Do_Execute_MCU();

};
