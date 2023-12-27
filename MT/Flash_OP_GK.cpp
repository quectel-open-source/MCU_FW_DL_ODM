#include "StdAfx.h"
#include "Flash_OP_GK.h"


CFlash_OP_GK::CFlash_OP_GK(void)
{
	m_iCom_Port=0;
	m_iBaud_Rate=0;

	m_bRuning =false;
	m_iClass_Idx=0;
	m_csError_Code.Format(_T(""));
	m_iError_Code=0;
	m_bPLC_Flag=false;
	m_iPLC_Port=0;
}

CFlash_OP_GK::~CFlash_OP_GK(void)
{
}
//cb Show_State
static void __stdcall cb_show_state_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP_GK * pSv = (CFlash_OP_GK *)p_usr_Arg;

	pSv->Show_Status( wcp_State );
}
static void __stdcall cb_show_out_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP_GK * pSv = (CFlash_OP_GK *)p_usr_Arg;

	pSv->Show_Out( wcp_State );
}

void CFlash_OP_GK::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP_GK::LoadFirst(void)
{
#ifdef FW_MULTI
#ifndef FW_UPGRADE
	::GK_DL_INIT_FIRST();
#endif
#endif

}

void CFlash_OP_GK::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP_GK::LoadInit(int i_TSCH_Numb , CProgressCtrl* pCtrProgress, CString csIMGFile_Path)
{
	//加载QTI回调
	this->m_str_Qti_Arg_Main.p_usr_Arguments = this;
	this->m_str_Qti_Arg_Main.cb_Show_REdit_Info = ::cb_show_state_r;
	this->m_str_Qti_Arg_Main.cb_Show_State = NULL;
	this->m_str_Qti_Arg_Main.cb_Show_Test_Result = NULL;
	this->m_str_Qti_Arg_Main.cb_Save_Trace_Log =  ::cb_show_out_r;
	this->m_str_Qti_Arg_Main.cb_Show_Title = NULL;

#ifdef FW_MULTI
#ifndef FW_UPGRADE

	//加载qti接口
	::GK_DL_LOAD_ARG_QTI(i_TSCH_Numb,&this->m_str_Qti_Arg_Main);

	::GK_DL_LOAD_CTR_PROGRESS( i_TSCH_Numb, pCtrProgress);

	//解析IMGFile文件
	//GOKE_V1.6_B9600_GB.bin
	bool bInit =false;
	bInit = ::GK_DL_LOAD_IMGFile_PATH( i_TSCH_Numb, csIMGFile_Path);
	if (false == bInit)
	{
		//...
		AfxMessageBox(_T("GPS LoadInit Fail"));
	} 
#endif
#endif

}


void CFlash_OP_GK::Show_Progress(double  i_Pgr_Percent)
{
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG,(WPARAM)(i_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}

int CFlash_OP_GK::Do_Execute()
{
	int iRtn =RTN_FAIL;
	bool bDl_ResFlag =false;

	this->m_iStop_Flag = -1;
	this->m_bRuning = true;
#ifdef FW_MULTI
#ifndef FW_UPGRADE

	this->m_csError_Code.Format(_T(""));

	::GK_DL_LOAD_COM_CONFIG( m_iClass_Idx , m_iCom_Port,m_iBaud_Rate );

	this->Show_Status(_T("Open Serial Port"));

    CString strInfo;
	strInfo.Format(_T("Open Serial Port(%d-%d)"),m_iCom_Port,m_iBaud_Rate);
	this->Show_Out(strInfo);

	//循环等待成功打开端口
	time_t t_Start;
	//开始计时
	time(&t_Start);
	int  iTimeTemp =0;

	if (false == ::GK_DL_OpenSerialPort ( m_iClass_Idx, m_iCom_Port))
	{
		this->Show_Out(_T("GK_DL_OpenSerialPort Fail"));
		return iRtn;
	}

// 	while(1)
// 	{
// 		if (true == ::GK_DL_OpenSerialPort ( m_iClass_Idx, m_iCom_Port))
// 		{
// 			this->Show_Out(_T("Open Serial Port Pass"));
// 			break;
// 		}
// 
// 		time( &t_End );
// 		iTimeTemp = (int)difftime( t_End,t_Start);
// 
// 		if (iTimeTemp>30)
// 		{
// 			this->m_iStop_Flag = -1;
// 			this->m_bRuning = false;
// 			m_csError_Code.Format(_T("Fail,GK_DL_OpenSerialPort(%d)"),m_iCom_Port);
// 			this->Show_Status(_T("Fail,Open Serial Port"));
// 			this->Show_Out(_T("Fail,Open Serial Port"));
// 			return iRtn;
// 		}
// 
// 	}
 

	this->Show_Status(_T("GPS BIN DL DOWNLOAD..."));
	this->Show_Out(_T("GPS BIN DL DOWNLOAD..."));


	iRtn = ::GK_DL_DO_FW_DOWNLOAD ( m_iClass_Idx);

	

	Sleep(2000);
	if (iRtn != RTN_SUCCESS)
	{
		CString csError_Code;
		//::GK_DL_GET_ERROR_ALL( m_iClass_Idx, &m_iError_Code , &csError_Code );
		this->Show_Progress(1.0);
		::GK_DL_CloseSerialPort ( m_iClass_Idx);

		CString strInfo;
		strInfo.Format(_T("Fail,GPS BIN DL DOWNLOAD-%d"),iRtn);

		this->Show_Status(strInfo);
		this->Show_Out(strInfo);
	} 
	else
	{
		this->Show_Progress(1.0);
		::GK_DL_CloseSerialPort ( m_iClass_Idx);
		this->Show_Status(_T("Pass,GPS BIN DL DOWNLOAD"));
		this->Show_Out(_T("Pass,GPS BIN DL DOWNLOAD"));
	}
	this->m_iStop_Flag = -1;
	this->m_bRuning = false;
#endif
#endif

	return iRtn;
}

void CFlash_OP_GK::GK_DL_ProgramClose_End(int i_TSCH_Numb)
{
#ifdef FW_MULTI
#ifndef FW_UPGRADE
	::GK_DL_ProgramClose(i_TSCH_Numb);
#endif
#endif
}
