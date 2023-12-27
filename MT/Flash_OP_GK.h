#pragma once

#include "gfc.h"
#include "Com_Port.h"
#include "Gk_Gps_Dl_Api.h"

class CFlash_OP_GK
{
public:
	HWND m_hWnd_View;
	int m_iCom_Port;
	int m_iBaud_Rate;
	bool m_bRuning;
	int m_iClass_Idx;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iStop_Flag;

	QTI_PROG_ARG m_str_Qti_Arg_Main;

	CCom_Port m_Com_Port;

public:
	CFlash_OP_GK(void);
	~CFlash_OP_GK(void);

	int Do_Execute();
	void Show_Status(const wchar_t * wcp_Status);

	void LoadFirst(void);
	void LoadInit(int i_TSCH_Numb , CProgressCtrl* pCtrProgress, CString csIMGFile_Path);

	void GK_DL_ProgramClose_End(int i_TSCH_Numb);

	void Show_Progress(double i_Pgr_Percent);
	void Show_Out(const wchar_t * wcp_Status);
	bool m_bPLC_Flag;//带PLC控制的夹具
	int m_iPLC_Port;

	CGFC m_cla_GFC;

};
