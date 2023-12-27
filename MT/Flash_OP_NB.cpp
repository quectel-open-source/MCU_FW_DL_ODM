#include "StdAfx.h"
#include "Flash_OP_NB.h"


CFlash_OP_NB::CFlash_OP_NB(void)
{
	m_iCom_Port=0;
	m_iBaud_Rate=0;

	m_bRuning =false;
	m_iClass_Idx=0;
	m_csError_Code.Format(_T(""));
	m_iError_Code=0;
	m_bPLC_Flag=false;
	m_iPLC_Port=0;
	m_iComMethod =0;
	m_iResetFlag =0;
	m_iClearNVFlag =0;
	m_bSaveDebugLog = false;
}

CFlash_OP_NB::~CFlash_OP_NB(void)
{
	//
   if (true ==  m_bSaveDebugLog)
   {
	   ::MNB_DL_Disable_MTK_LOG();
   }

}
//cb Show_State
static void __stdcall cb_show_state_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP_NB * pSv = (CFlash_OP_NB *)p_usr_Arg;

	pSv->Show_Status( wcp_State );
}
static void __stdcall cb_show_out_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP_NB * pSv = (CFlash_OP_NB *)p_usr_Arg;

	pSv->Show_Out( wcp_State );
}

void CFlash_OP_NB::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP_NB::LoadFirst(void)
{
	::MNB_DL_INIT_FIRST();
}

void CFlash_OP_NB::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP_NB::LoadInit(int i_TSCH_Numb , CProgressCtrl* pCtrProgress, CString csDA_File_Path,CString Scat_Cfg_File_Path,CString csLog_File_Path)
{
	

	//加载QTI回调
	this->m_str_Qti_Arg_Main.p_usr_Arguments = this;
	this->m_str_Qti_Arg_Main.cb_Show_REdit_Info = ::cb_show_state_r;
	this->m_str_Qti_Arg_Main.cb_Show_State = NULL;
	this->m_str_Qti_Arg_Main.cb_Show_Test_Result = NULL;
	this->m_str_Qti_Arg_Main.cb_Save_Trace_Log =  ::cb_show_out_r;
	this->m_str_Qti_Arg_Main.cb_Show_Title = NULL;

	//加载qti接口
	::MNB_DL_LOAD_ARG_QTI(i_TSCH_Numb,&this->m_str_Qti_Arg_Main);

	::MNB_DL_LOAD_CTR_PROGRESS( i_TSCH_Numb, pCtrProgress);

	//解析DA文件，载入到DA_HANDLE
	//DA文件路径
	::MNB_DL_LOAD_DA_FILE_PATH( i_TSCH_Numb , csDA_File_Path);

	::MNB_DL_LOAD_CFG_FILE_PATH( i_TSCH_Numb, Scat_Cfg_File_Path );


	//
	if ((1 == i_TSCH_Numb) && (true ==  m_bSaveDebugLog))
	{
		this->m_csLog_File_Path = csLog_File_Path;

		CTime aTime = CTime::GetCurrentTime();
		int y = aTime.GetYear();
		int m = aTime.GetMonth();
		int d = aTime.GetDay();
		int h = aTime.GetHour();
		int min = aTime.GetMinute();
		int s = aTime.GetSecond();

		this->m_csLog_File_Path_Full.Format(_T("%sdebug-%d-%d-%d-%d-%d-%d.txt"),csLog_File_Path,y,m,d,h,min,s);

		::MNB_DL_LOAD_LOG_FILE_PATH( i_TSCH_Numb, this->m_csLog_File_Path_Full);

		::MNB_DL_Enable_MTK_LOG();
	}



}


void CFlash_OP_NB::Show_Progress(int i_Pgr_Percent)
{
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)i_Pgr_Percent, (LPARAM)this->m_iClass_Idx);
}

int CFlash_OP_NB::Do_Execute()
{
	int iRtn;
	bool bDl_ResFlag =false;

	this->m_iStop_Flag = -1;
	this->m_bRuning = true;

	this->m_csError_Code.Format(_T(""));

	//----------------------------------------------------
// 	if (true ==  m_bSaveDebugLog)
// 	{
// 		CString csLog_File_Path;
// 		csLog_File_Path = this->m_csLog_File_Path;
// 
// 		unsigned long ul_File_Size =0;
// 		if(true == this->Get_File_Size(this->m_csLog_File_Path_Full, &ul_File_Size))
// 		{
// 			if (ul_File_Size>2048000)//2M 新增一个文件  2048000
// 			{
// 				CTime aTime = CTime::GetCurrentTime();
// 				int y = aTime.GetYear();
// 				int m = aTime.GetMonth();
// 				int d = aTime.GetDay();
// 				int h = aTime.GetHour();
// 				int min = aTime.GetMinute();
// 				int s = aTime.GetSecond();
// 
// 				this->m_csLog_File_Path_Full.Format(_T("%sdebug-%d-%d-%d-%d-%d-%d.txt"),csLog_File_Path,y,m,d,h,min,s);
// 				::MNB_DL_LOAD_LOG_FILE_PATH( m_iClass_Idx, this->m_csLog_File_Path_Full);
// 			}
// 			else
// 			{
// 				//...继续
// 			}
// 			  
// 		}
// 
// 	}	
	//----------------------------------------------------



	::MNB_DL_LOAD_COM_CONFIG( m_iClass_Idx , m_iCom_Port, m_iBaud_Rate );

	//升级情况将模块重启下
	//20190921
	//this->RestModule();
	if (1 == m_iResetFlag)//升级后自动重启
	{
#ifdef FW_MULTI
#ifndef FW_UPGRADE
		if (PRODU_TYPE_MOB != this->m_iProdu_Type)
		{
			//工厂下载工具 UART口，升级后自动重启，下载前格式化，无需清NV文件系统
			bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx ,IN_UART, true,true,false,m_bSaveDebugLog);
		}
		else
		{
			//多口升级工具 UART口，升级后自动重启，下载前无需格式化，无需清NV文件系统---------------暂定
			bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx , IN_UART,true,false,false,m_bSaveDebugLog );
		}
#else
		//多口升级工具 UART口，升级后自动重启，下载前无需格式化，无需清NV文件系统---------------暂定
		bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx , IN_UART,true,false,false,m_bSaveDebugLog );
#endif
#else
		if (1 == m_iClearNVFlag)//升级前清除下NV
		{
			//单口升级工具 UART/USB, 升级后自动重启,下载前无需格式化,升级前清除下NV
			bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx ,m_iComMethod, true,false,true,m_bSaveDebugLog);
		}
		else
		{
			//单口升级工具 UART/USB, 升级后自动重启,下载前无需格式化,无需清NV文件系统
			bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx ,m_iComMethod, true,false,false,m_bSaveDebugLog);
		}
#endif
	}
	else//升级后自动关机，不重启
	{
#ifdef FW_MULTI
#ifndef FW_UPGRADE
		
		if (PRODU_TYPE_MOB != this->m_iProdu_Type)
		{
			//工厂下载工具 UART口，升级后不自动重启，下载前格式化，无需清NV文件系统-------------------默认
			bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx ,IN_UART, false,true,false,m_bSaveDebugLog );
		}
		else
		{
			//多口升级工具 UART口，升级后不自动重启，下载前不格式化，无需清NV文件系统
			bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx , IN_UART,false,false,false,m_bSaveDebugLog );
		}
#else
		//多口升级工具 UART口，升级后不自动重启，下载前不格式化，无需清NV文件系统
		bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx , IN_UART,false,false,false ,m_bSaveDebugLog);
#endif
#else
		if (1 == m_iClearNVFlag)//升级前清除下NV
		{
			//单口升级工具 UART/USB, 升级后不自动重启,下载前无需格式化,升级前清除下NV
			bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx ,m_iComMethod, false,false, true,m_bSaveDebugLog);
		}
		else
		{
			//单口升级工具 UART/USB, 升级后不自动重启,下载前无需格式化,升级前无需清除下NV-------------------默认
			bDl_ResFlag = ::MNB_DL_DO_MNB_FW_DOWNLOAD( m_iClass_Idx ,m_iComMethod, false,false ,false,m_bSaveDebugLog);
		}

		
#endif	
	}

	Sleep(2000);
	if (false == bDl_ResFlag)
	{
		iRtn = RTN_FAIL;
		::MNB_DL_GET_ERROR_ALL( m_iClass_Idx, 0 , &m_iError_Code , &m_csError_Code ); //STATION_DL_ONLY
		this->Show_Progress(100);
	} 
	else
	{
		iRtn = RTN_SUCCESS;
		this->Show_Progress(100);
	}
	this->m_iStop_Flag = -1;
	this->m_bRuning = false;

	return iRtn;
}

int CFlash_OP_NB::OpenMainPort()
{
	int iRtn=RTN_FAIL;
	time_t t_Start, t_End;
	int i_TimeOut = 60;

	m_Com_Port.m_iPort = m_iCom_Port;
	m_Com_Port.m_iBaudRate = 9600;
	//m_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
	//m_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;
	Show_Status(_T("Write HAIERSTOP..."));
	Sleep(3000);
   try
   {
		if (false == m_Com_Port.Port_Open(500))
		{
			return iRtn;
			Show_Status(_T("Open Port Fail"));
		}
		 
		//m_Com_Port.Empty_Com_And_Rx_Buff();
		Show_Status(_T("Write HAIERSTOP..."));
		if (false == m_Com_Port.Write_Byte("HAIERSTOP", 9))
		//if (false == m_Com_Port.Write(_T("HAIERSTOP")))
		{
			Show_Status(_T("Write HAIERSTOP Fail"));
			throw(1);
		}	
		Sleep(3000);
   }
   catch(...)
   {
	   m_Com_Port.Port_Close();
	   return iRtn;		
   }
   m_Com_Port.Port_Close();

   //Show_Status(_T("Write HAIERSTOP Pass"));

   Sleep(6000);


   //切换到115200波特率下检查AT是否已经可用
   m_Com_Port.m_iPort = m_iCom_Port;
   m_Com_Port.m_iBaudRate = 115200;
   //m_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
   //m_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;
   try
   {
	   time(&t_Start);
	   Show_Status(_T("Check HAIERSTOP ..."));
	   if (false == m_Com_Port.Port_Open(500))
	   {
		   time(&t_End);
		   if ((int)difftime(t_End, t_Start)>i_TimeOut)
		   {
			   Show_Status(_T("Open Port Fail(1)"));
			   return iRtn;
		   }

	   }
	   while (1)
	   {		   
		   //m_Com_Port.Empty_Com_And_Rx_Buff();

		   if(m_Com_Port.Query_Tgt_CHAR(_T("ATE0"), _T("OK"), 4) == false)
		   {
			   time(&t_End);
			   if ((int)difftime(t_End, t_Start)>i_TimeOut)
			   {
				   throw(0);
			   }	   
		   }
		   else //成功
		   {
			   iRtn = RTN_SUCCESS;
			   break;
		   }
	   }
   }
   catch(...)
   {
	   m_Com_Port.Port_Close();
	   return iRtn;		
   }
	m_Com_Port.Port_Close();
	Show_Status(_T("Check HAIERSTOP Pass"));
	return iRtn;

}

//让模块重启进去下载模式
int CFlash_OP_NB::RestModule()
{
	int iRtn=RTN_FAIL;
	time_t t_Start, t_End;
	int i_TimeOut = 3;

	m_Com_Port.m_iPort = m_iCom_Port;
	m_Com_Port.m_iBaudRate = 115200;

	try
	{
		time(&t_Start);

		if (false == m_Com_Port.Port_Open(500))
		{
			Show_Status(_T("Open Port Fail"));
			throw(1);
		}
		while (1)
		{		   
			//m_Com_Port.Empty_Com_And_Rx_Buff();
			if(m_Com_Port.Query_Tgt_CHAR(_T("AT+QRST=1"), _T("Leaving the BROM"), 4) == false)
			{
				time(&t_End);
				if ((int)difftime(t_End, t_Start)>i_TimeOut)
				{
					throw(0);
				}	   
			}
			else //成功
			{
				iRtn = RTN_SUCCESS;
				break;
			}
		}

	}
	catch(...)
	{
		m_Com_Port.Port_Close();
		return iRtn;		
	}
	m_Com_Port.Port_Close();
	return iRtn;

}
bool CFlash_OP_NB::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
{
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	try
	{
		//打开
		hFile_Handle = ::CreateFile(wcp_Path,
			GENERIC_READ,				//文件的操作属性 Read only required
			FILE_SHARE_READ,			//文件共享属性 read share file
			NULL,						//文件安全特性 SecurityAttributes - none in Win95
			OPEN_EXISTING,				//文件操作 how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//文件属性 file attributes
			NULL	//如果不为零，则指定一个文件句柄。新文件将从这个文件中复制扩展属性 handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			throw(0);
		}

		//获取文件的BYTE数
		*ulp_Size = ::GetFileSize(hFile_Handle, NULL);

		//关闭
		::CloseHandle(hFile_Handle);
		hFile_Handle = INVALID_HANDLE_VALUE;
	}
	catch(...)
	{
		return false;
	}

	return true;
}