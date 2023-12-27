#pragma once

#include "IniFile.h"

class UpdateConfig
{
public:
	UpdateConfig();
	~UpdateConfig();

public:
public:
	CIniFile m_iniFile;
	wchar_t m_wcrCurr_Exe_Path[MAX_PATH];//工具当前路径

	void InitConfigPath(CString csFileName);
	void SaveConfig();
	CString	m_csServerName_Fac;//服务器
	bool m_bOCP; //BOOL转bool类型warning
	bool m_bWrite_Check;
	CString m_csSave_Path;
	CString m_csFactory_Info_Name;//工厂名称（HF/JSD/WCL/XT)
};

