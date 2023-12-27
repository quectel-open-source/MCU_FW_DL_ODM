#include "stdafx.h"
#include "UpdateConfig.h"


UpdateConfig::UpdateConfig()
{
}


UpdateConfig::~UpdateConfig()
{
}

void UpdateConfig::InitConfigPath(CString csFileName)
{
	memset(this->m_wcrCurr_Exe_Path, 0, sizeof(this->m_wcrCurr_Exe_Path));
	::GetCurrentDirectory(MAX_PATH, this->m_wcrCurr_Exe_Path);

	//主工具配置文件
	this->m_iniFile.m_csIniFileName = m_wcrCurr_Exe_Path;
	this->m_iniFile.m_csIniFileName += csFileName;

}

void UpdateConfig::SaveConfig()
{
	//[FAC]
	this->m_iniFile.StoreStr(_T("FAC"), _T("PDB_1"), this->m_csServerName_Fac);
	this->m_iniFile.StoreStr(_T("FAC"), _T("F_NAME"), this->m_csFactory_Info_Name);

	//[OPTION]
	this->m_iniFile.StoreB(_T("OPTION"), _T("Do_OCP_Update"), this->m_bOCP);
	this->m_iniFile.StoreB(_T("OPTION"), _T("Do_Write_Check_Update"), this->m_bWrite_Check);

	//[PATH]
	this->m_iniFile.StoreStr(_T("PATH"), _T("Save_Path"), this->m_csSave_Path);

}