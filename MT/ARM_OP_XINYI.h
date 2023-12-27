#pragma once
#include "gfc.h"



class CARM_OP_XINYI
{
public:
	CARM_OP_XINYI(void);
	~CARM_OP_XINYI(void);

	HWND m_hWnd_View;
	bool m_bRuning;
	int m_iStop_Flag;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;
	int m_iCom_Port;
	CString m_csCurr_Exe_Path;
	int m_iBaud_Rate;
    BOOL m_bXinYi_Reset_after_DL;


	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);
	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);

};
