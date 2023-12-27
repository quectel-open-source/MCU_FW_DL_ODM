#pragma once
#include "gfc.h"
#include "Com_Port.h"

#define DL_FW_MCU_FAIL1  1096
#define DL_FW_MCU_FAIL2  1097
#define DL_FW_MCU_FAIL3  1098
#define DL_FW_MCU_FAIL4  1099
#define DL_FW_MCU_FAIL5  1100
struct SILABS_FIRE_INFO
{
	//CString strBootLoader;
	CString strHexFile;
	bool b_Enable_DL;
	CString cs_Path;
	CString cs_Name;
	SILABS_FIRE_INFO()
	{
		cs_Name = _T("");
		cs_Path = _T("");
		strHexFile = _T("");
		b_Enable_DL = false;
	}

	void Clear()
	{
		cs_Name = _T("");
		cs_Path = _T("");
		strHexFile = _T("");
		b_Enable_DL = false;
	}
};


class CARM_OP_SILABS
{
public:
	CARM_OP_SILABS(void);
	~CARM_OP_SILABS(void);

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
	bool LOAD_FW_FILE_V1(const wchar_t * wcp_FW_File_Path);
	int m_iOP_TYPE;
	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);
	SILABS_FIRE_INFO m_FWInfo;
	CString m_csCurr_Exe_Path;

	int m_iCom_Port;
	int m_iSec_Port;

	int m_iAT_Port;

	void DO_DL_OP(int i_Index);
	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);
	int  m_PassNumb;
	int  m_ErrorNumb;

	CCom_Port m_cla_Com_Port;
	CString  mAT_IMEI;
	CString  mAT_FirewareName;
	CString  mDB_FirewareName;
	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	bool Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut);

	bool FindHexFile(CString lpPath);
	bool CreatZG100S_Jlink_CMD(void);
	int Do_Execute_MCU();
};


