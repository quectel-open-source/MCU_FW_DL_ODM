#include "stdafx.h"
#include "Tool_Update_Config.h"


Tool_Update_Config::Tool_Update_Config ()
{
}


Tool_Update_Config::~Tool_Update_Config ()
{
}


void Tool_Update_Config::InitConfigPath ( CString csFileName )
{

	//主工具配置文件
	this->m_iniFile.m_csIniFileName = GetExePath();
	this->m_iniFile.m_csIniFileName += csFileName;

}


void Tool_Update_Config::SaveConfig ()
{
	//[SQL]
	this->m_iniFile.StoreStr ( _T ( "SQL" ) , _T ( "IP_Addr" ) , this->m_csServerName_Fac );
	this->m_iniFile.StoreStr ( _T ( "SQL" ) , _T ( "F_NAME" ) , this->m_csFactory_Info_Name );

	//[PATH]
	this->m_iniFile.StoreStr ( _T ( "PATH" ) , _T ( "EXE_Path" ) , this->m_csExe_Path );
	this->m_iniFile.StoreStr ( _T ( "PATH" ) , _T ( "EXE_Name" ) , this->m_csExe_Name );
	this->m_iniFile.StoreStr ( _T ( "PATH" ) , _T ( "Setting" ) , this->m_csSetting );

	//[EXE]
	this->m_iniFile.StoreStr ( _T ( "EXE" ) , _T ( "Folder" ) , this->m_csTool_Name );

}

CString Tool_Update_Config::GetExePath ( void )
{
	CString csPath;
	TCHAR cTemp[MAX_PATH];
	::GetModuleFileName ( NULL , cTemp , sizeof ( cTemp ) );
	csPath.Format ( _T ( "%s" ) , cTemp );
	int index = csPath.ReverseFind ( '\\' );
	csPath = csPath.Left ( index );
	return csPath;
}