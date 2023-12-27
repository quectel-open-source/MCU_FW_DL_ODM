#pragma once
#include "gfc.h"
#include "IniFile.h"
#include "Com_Port.h"


class CARM_OP_ALTAIR
{
public:
	CARM_OP_ALTAIR(void);
	~CARM_OP_ALTAIR(void);

	HWND m_hWnd_View;
	bool m_bRuning;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;
	int m_iCom_Port;
	CString m_csCurr_Exe_Path;
	int m_iBaud_Rate;
	CGFC m_cla_GFC;
	CString m_strFactoryImagePath; // 工厂的golden image路径
	CString m_strFWFolderPath;     // RK_03_00文件夹目录
	CString m_strAppFW;            // burns Application FW
	CString m_strModemFW;          // burns Modem FW
	CString m_strModemFW2;         // burns Modem FW2
	CString m_strChipType;         // 芯片类型
	bool m_bIsBaudHighSpeed;       // 是低速还是高速下载，默认高速：1
	int m_iFlowControl;            // Flow Control：None,0；RTS_CTS:1
	int m_iDLMode;                 // 芯片状态，0是空片模式recovery mode and burn golden image；1是非空片模式burn golden image。默认是0,仅工厂格式化用
	int m_iProdu_Type;//通过OCP获取的机型，主要是工厂用来判断是升级还是工厂格式化下载
	CCom_Port m_cla_Com_Port;


	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);
	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);
	bool LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path);
	int Do_Execute(void);
	bool CheckModuleMode();
	bool DL_FactoryRecoveryModeFW();
	bool DL_FactoryFW();  // 前提确定非空片，才能用该模式格式化下载
};


