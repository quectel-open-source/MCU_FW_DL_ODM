#include "StdAfx.h"
#include "ARM_OP_XINYI.h"
#include "ts_lan.h"



CARM_OP_XINYI::CARM_OP_XINYI(void)
: m_iCom_Port(0)
, m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_csCurr_Exe_Path(_T(""))
, m_bXinYi_Reset_after_DL(FALSE)
{
}


CARM_OP_XINYI::~CARM_OP_XINYI(void)
{


}
void CARM_OP_XINYI::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_XINYI::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_XINYI::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_XINYI::Show_Progress(double d_Pgr_Percent)
{
	//此函数中应该包含进度条百分比的计算，然后在消息响应函数中完成具体步进点数的计算
	//Progress总长度是1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_XINYI::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}


