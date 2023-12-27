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





struct FIRE_INFO
{
	CString strAgentBoot;
	CString strBootLoader;
	CString strSystem;
	CString strRF;
	bool b_Enable_DL;
	FIRE_INFO()
	{
		strAgentBoot = _T("");
		strBootLoader = _T("");
		strSystem = _T("");
		strRF = _T("");
		b_Enable_DL = false;
	}

	void Clear()
	{
		strAgentBoot = _T("");
		strBootLoader = _T("");
		strSystem = _T("");
		strRF = _T("");
		b_Enable_DL = false;
	}
};


class CARM_OP_EIGEN
{
public:
	CARM_OP_EIGEN(void);
	~CARM_OP_EIGEN(void);

	HWND m_hWnd_View;
	bool m_bRuning;
	int m_iStop_Flag;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;
	int m_iCom_Port;
	CString m_csCurr_Exe_Path;
	int m_iBaud_Rate;
	FIRE_INFO m_FWInfo;
	CGFC m_cla_GFC;
	CIniFile m_cla_ConfigFile;
	CString m_strBootloaderFile;
	CString m_strSystemFile;


	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);
	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);

	int Do_Execute(void);
	bool DL_Probe(const CString& strCmdExe);
	bool DL_EraseAll(const CString& strCmdExe);
	bool DL_Bootloader_System(const CString& strCmdExe);
	bool DL_FlexFile(int iIndex, const CString& strCmdExe);
	void Save_Config();
	bool LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path);

	bool DL_EraseNV(const CString& strCmdExe);
	bool DL_Reset(const CString& strCmdExe);
	int   m_iResetFlag;//1:reset 0:no reset

	int  m_iProdu_Type;//通过OCP获取的机型，主要是工厂用来判断是升级还是工厂格式化下载

	CString  m_csSrcIni_File;

	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);

	int m_iBBchip_Curr; //EC616和EC617(EC616S)

	int m_iOP_Robot;
	int m_Ts_Num;
	int m_iTimeInterval;

	void Set_PowerOn_SPC(int i_Index);
	void Open_Power_SPC(int iIndex);

#ifdef FW_MULTI
#ifndef FW_UPGRADE
	CPower_Supply* m_cla_PS_1;
	CPower_Supply* m_cla_PS_2;
	CPower_Supply* m_cla_PS_3;
	CPower_Supply* m_cla_PS_4;
#endif
#endif
};


