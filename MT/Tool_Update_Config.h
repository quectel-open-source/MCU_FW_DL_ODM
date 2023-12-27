#pragma once

#include "IniFile.h"

class Tool_Update_Config
{
public:
	Tool_Update_Config ();
	~Tool_Update_Config ();


public:
	CIniFile m_iniFile;

	CString GetExePath ( void );
	void InitConfigPath ( CString csFileName );
	void SaveConfig ();
	CString	m_csServerName_Fac;//服务器
	int m_iPort;//端口
	CString m_csFactory_Info_Name;//工厂名称（HF/JSD/WCL/XT/KD)

	CString m_csExe_Path;//   /* D:\Run\ASR_RF_CAL_V1.10.1\ */
	CString m_csExe_Name;//      ASR_RF_CAL_V1.10.1
	CString m_csSetting;// 需要从老工具copy到新工具的文件或文件夹,以分号隔离  Config.ini;Param

	CString m_csTool_Name;// ASR_RF_CAL

};

