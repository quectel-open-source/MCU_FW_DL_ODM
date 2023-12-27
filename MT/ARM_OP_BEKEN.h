#pragma once
#include "gfc.h"
#include "IniFile.h"

#ifdef FW_MULTI
#ifndef FW_UPGRADE
#include "Power_Supply.h"
#endif
#endif
#define  ERR_DL_RESET_FAIL					0x03002060
#define  ERR_DL_RESET_TIME_OUT				0x03002062
#define  ERR_DL_PROBE_TIME_FAIL				0x03002063
#define  ERR_DL_PROBE_TIME_OUT              0x03002064
#define  ERR_DL_ERASEALL_FAIL               0x03002065
#define  ERR_DL_ERASEALL_TIME_OUT			0x03002066
#define  ERR_DL_BOOTLOADER_SYSTEM_FAIL_EXT  0x03002067
#define  ERR_DL_BOOTLOADER_SYSTEM_FAIL      0x03002068
#define  ERR_DL_BOOTLOADER_SYSTEM_TIME_OUT  0x03002069
#define  ERR_DL_FLEX_FAIL					0x03002070
#define  ERR_DL_FLEX_TIME_OUT				0x03002071
#define  ERR_DL_FIND_PORT					0x03002072

struct BEKEN_FIRE_INFO
{
	CString strAgentBoot;
	CString strBootLoader;
	//CString strSystem;
	CString strRF;
	bool b_Enable_DL;
	BEKEN_FIRE_INFO()
	{
		strAgentBoot = _T("");
		strBootLoader = _T("");
		//strSystem = _T("");
		strRF = _T("");
		b_Enable_DL = false;
	}

	void Clear()
	{
		strAgentBoot = _T("");
		strBootLoader = _T("");
		//strSystem = _T("");
		strRF = _T("");
		b_Enable_DL = false;
	}
};


class CARM_OP_BEKEN
{
public:
	CARM_OP_BEKEN(void);
	~CARM_OP_BEKEN(void);

	HWND m_hWnd_View;
	bool m_bRuning;
	int m_iStop_Flag;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;
	int m_iCom_Port;
	CString m_csCurr_Exe_Path;
	int m_iBaud_Rate;
	BEKEN_FIRE_INFO m_FWInfo;
	CGFC m_cla_GFC;
	CIniFile m_cla_ConfigFile;
	int   m_iResetFlag;//1:reset 0:no reset
	//CString m_strAARFile;
	CString m_strBootloaderFile;
	int m_iProdu_Type;
	//CString m_strSystemFile;


	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);
	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);

	int Do_Execute(void);
	bool DL(const CString& strCmdExe);
	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);
	bool LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path);
	//int m_iBBchip_Curr; 

	int m_iOP_Robot;
	int m_Ts_Num;
	int m_iTimeInterval;

	void Set_PowerOn_SPC(int i_Index);
	void Open_Power_SPC(int iIndex);
	bool Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut);

#ifdef FW_MULTI
#ifndef FW_UPGRADE
	CPower_Supply* m_cla_PS_1;
	CPower_Supply* m_cla_PS_2;
	CPower_Supply* m_cla_PS_3;
	CPower_Supply* m_cla_PS_4;
#endif
#endif
};


