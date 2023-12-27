#include "StdAfx.h"
#include "ARM_OP_HUAWEI.h"
#include "ts_lan.h"

#include "USBDeviceDetection.h"

extern CCriticalSection g_PLC_Pow_Ctrl;
extern CCom_Port g_cla_Com_PLC_Power;//针对一拖四的半自动夹具控制

CCriticalSection g_SSB_Ctrl;

CCriticalSection g_PLC_Power;

CCriticalSection g_cla_HS_Lock_Enter_EDL;
CCriticalSection g_cla_HS_Lock_QDLoader_Port;


void Multi_PLC_Pow_BC(LPVOID x);
void Multi_PLC_Pow_BC_1(LPVOID x);
void Multi_PLC_Pow_BC_2(LPVOID x);
void Multi_PLC_Pow_BC_3(LPVOID x);
void Multi_PLC_Pow_BC_4(LPVOID x);
void Multi_PLC_Pow_BC_5(LPVOID x);
void Multi_PLC_Pow_BC_6(LPVOID x);
void Multi_PLC_Pow_BC_7(LPVOID x);
void Multi_PLC_Pow_BC_8(LPVOID x);

CARM_OP_HUAWEI::CARM_OP_HUAWEI(void)
: m_iCom_Port(0)
,m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_Ts_Num(0)
, m_iOP_TYPE(0)
, m_bGet_Debug_Log(false)
, m_csCurr_Exe_Path(_T(""))
, m_bCheckSaveLog(FALSE)
, m_DL_FLAG(FALSE)
, m_bPowFlag(FALSE)
, m_blWrCon(TRUE)
, m_bWriteKV(FALSE)
, m_bWriteFEC(FALSE)
, m_bSSB_Update(FALSE)
, m_iFindFlagBCG_BC95D(0)
,m_bBootFlag(false)
, mBar_IMEI(_T(""))
, mBar_SN(_T(""))
, mAT_FirewareName(_T(""))
, m_iPLC_Port(0)
, m_bNetFlagDL(FALSE)
, m_bIsRepMo(false)
, m_bOutLine(FALSE)
, m_iDL_flag(0)
, m_iHS_DL_MODE_M(0)
{
	m_iPpc=0;
	m_iPsc=0;
	m_iPac=0;

	m_iPsuc=0;
	m_iPauc=0;
	m_iPgc=0;

	m_bCheckSaveLog = FALSE;

	m_str_fwpkg_FW.b_Enable_DL =false;
	m_str_fwpkg_FW.cs_Name.Format(_T(""));
	m_str_fwpkg_FW.cs_Path.Format(_T(""));

	m_iUpdateall =0;

	m_iBC95BaudSwitch =0;
	m_iOP_Robot =0;
	m_iBC95_EraseSwitch =0;
	m_iBC95_EraseSwitchFirst =0;
	m_iBC95_DC_ResetTime =0;
	m_iError_Code =0;

	m_iBcg_ssb_COM_update_B200=0;
	m_iBcg_ssb_COM_update_B200_B180=0;
	m_PassNumb=0;
	m_ErrorNumb=0;
	m_iBcg_ssb_com=0;

	m_iREP_NO_SSB = 0;

	mAT_IMEI.Format(_T(""));
	mAT_SN.Format(_T(""));

	m_iBC95_DC_ResetTime_Flag= FALSE;

	m_iTimeInterval=3;

	m_iFindVerFlag=0;

	m_i150ResetTime1=0;//150平台下载SSB复位1
	m_i150ResetTime2=0;//150平台下载SSB复位2
	m_i150ResetTime3=0;//150平台下载FW复位1
	m_i150ResetTime4=0;//150平台下载FW复位2

	m_csOC.Format(_T(""));

	m_ul_DL_All_Bytes =0;
	m_ul_Transmit_Bytes =0;


	m_strUSBPortAddress.Format(_T(""));

	m_iRebootFlag =0;


}

CARM_OP_HUAWEI::~CARM_OP_HUAWEI(void)
{
}
void CARM_OP_HUAWEI::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_HUAWEI::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_HUAWEI::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

//自动线程控电源上电
void CARM_OP_HUAWEI::Set_PowerOn_SPC(int i_Index)
{
	this->Open_Power_SPC(i_Index);
	//::SendMessage(this->m_hWnd_View, WM_PowerOn_SPC, (WPARAM)i_Index, (LPARAM)i_Index);
}

//擦除操作
bool CARM_OP_HUAWEI::ERASE_FLS_FILE_IN_EXE( int iClass_Idx)
{

	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	bool bBreak_Flag = false;
	unsigned long ulActual_Read;
	int iTemp;
	CString csTotal_Info, csData,info2,strPow,strEraseInfo;
	time_t t_Start,t_Start_T, t_End;

	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;

	bool bDownloading_Flag = false;

	CString strClassIdx,strBatPath,strDis;
	wchar_t * wcpCmd = NULL;

	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	//g_dl_load_proc.Lock();
	strClassIdx.Format(_T("%d"),iClass_Idx);

	if(TRUE == m_bCheckSaveLog)
	{
		//打开日志文件
		CString strLogPath = m_csCurr_Exe_Path + "\\NB-IoT\\" + strClassIdx + "\\log.txt";	

		hFile_Handle = ::CreateFile(strLogPath,
			GENERIC_WRITE,				//文件的操作属性 Read only required
			FILE_SHARE_READ,			//文件共享属性 read share file
			NULL,						//文件安全特性 SecurityAttributes - none in Win95
			CREATE_ALWAYS,				//文件操作 how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//文件属性 file attributes
			NULL	//如果不为零，则指定一个文件句柄。新文件将从这个文件中复制扩展属性 handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			CString info = strLogPath+" open failure";
			this->Show_Out(info);
			//AfxMessageBox(info);
			this->Show_Out(_T("Fail,ERASE_FLS_FILE_IN_EXE ----1"));
			return false;
		}
	}
	
	try  
	{
	
		//初始显示
		this->Show_Status(_T("Waiting......"));

		//初始化控制运行的相关参数
		this->m_csError_Code.Format(_T(""));

//之前有检查端口的情形
// 		//------------------------------ 检查串口是否存在 ------------------------------ 
// 		if(this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iCom_Port) == false)
// 		{
// 			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iCom_Port);
// 			this->Show_Status(this->m_csError_Code);
// 			this->Show_Out(this->m_csError_Code);
// 			throw(0);
// 		}
// 
// 		//检查串口是否可打开 20180227
// 		//
// 		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
// 		this->m_cla_Com_Port.m_iBaudRate = 9600;
// 
// 		if(this->m_cla_Com_Port.Port_Open(400))
// 		{
// 			this->m_cla_Com_Port.Port_Close();
// 		}
// 		else
// 		{
// 			this->m_csError_Code.Format(_T("FAIL,擦除:Port-%d In Using!"), this->m_iCom_Port);
// 			this->Show_Status(this->m_csError_Code);
// 			this->Show_Out(this->m_csError_Code);
// 			this->m_cla_Com_Port.Port_Close();
// 			throw(0);
// 		}

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code = _T("Create pipe fail");
			this->Show_Out(this->m_csError_Code);
			throw(0);

		}
		//------------------------------ 
		csTotal_Info.Format(_T(""));	

		//------------------------------ 打包dos指令 ------------------------------ 

		CString strCom;
		strCom.Format(_T("com%d"),this->m_iCom_Port);

		CString strExeName = _T("");
		switch(iClass_Idx)
		{
			case 1: strExeName = _T("UEUpdater1.exe"); break;
			case 2: strExeName = _T("UEUpdater2.exe"); break;
			case 3: strExeName = _T("UEUpdater3.exe"); break;
			case 4: strExeName = _T("UEUpdater4.exe"); break;
			default: strExeName = _T("UEUpdater.exe"); break;
		}


		//CString pathtmp = "\""+strBatPath+"\"";
		//UEUpdater.exe erase --port com3
		CString strBatPath = m_csCurr_Exe_Path + "\\NB-IoT\\" + strClassIdx +"\\UEUpdaterCLI\\"+strExeName;
		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 4096);
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (strBatPath.GetLength() + 128));
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 4096);

		swprintf(wcpCmd, _T("cmd.exe %cc \"%s erase \-\-port %s\""), '/', strBatPath,strCom);

		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code = _T("Create process fail");
			this->Show_Out(this->m_csError_Code);
			throw(1);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		//开始计时
		time(&t_Start);

		while(false == bDownloading_Flag) 
		{
			//读info
			memset(cpRead_Buff, 0, 4096);
			ulActual_Read = 0;

			//读数据
			if(::ReadFile(hRead_Pipe, cpRead_Buff, 4095, &ulActual_Read, NULL) == NULL)
			{
				bDownloading_Flag = true;//20190626
			}
			else
			{
				//正常
			}

			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 15)//超时，报fail
			{
				//如果异常后，使得下载按钮可用
				this->m_csError_Code = _T("Erase TimeOut");
				this->Show_Out(this->m_csError_Code);
				throw(4);
			}	

			if(TRUE == m_bCheckSaveLog)
			{
				//写头数据
				unsigned long dwNumb=0;  
				if(::WriteFile(hFile_Handle, cpRead_Buff, ulActual_Read, &dwNumb, NULL) != TRUE)
				{
					//AfxMessageBox(_T("log file Write fail!"));
					this->m_csError_Code = _T("log file Write fail");
					this->Show_Out(this->m_csError_Code);
					throw(0);
				}
			}
	

			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 4096);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				//汇总数据
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
				//Please reset the UE
				strEraseInfo.Format(_T("trying to reset ue using bootloader commands"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), strEraseInfo);
				if (NULL != wcpHead)
				{
					// 模块重启需等待
					info2.Format(_T("1%%"));
					csTotal_Info.Format(_T("%s"), (wcpHead + strEraseInfo.GetLength()));

					strPow.Format(_T("reset ue using at command(1)"));
					this->Show_Status(strPow);
					this->Show_Progress(0.1);
				}

				strEraseInfo.Format(_T("trying to reset ue using at command"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), strEraseInfo);
				if (NULL != wcpHead)
				{
					csTotal_Info.Format(_T("%s"), (wcpHead + strEraseInfo.GetLength()));
					
					strPow.Format(_T("reset ue using at command"));
					this->Show_Status(strPow);
					this->Show_Progress(0.1);
				}
				strEraseInfo.Format(_T("verifying connection"));

				wcpHead = wcsstr(csTotal_Info.GetBuffer(),strEraseInfo);
				if (NULL != wcpHead)
				{
					csTotal_Info.Format(_T("%s"), (wcpHead + strEraseInfo.GetLength()));
					this->Show_Status(_T("Verifying connection..."));
					this->Show_Progress(0.3);
				}
				strEraseInfo.Format(_T("performing full chip erase"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), strEraseInfo);
				if (NULL != wcpHead)
				{
					csTotal_Info.Format(_T("%s"), (wcpHead + strEraseInfo.GetLength()));
					this->Show_Status(_T("Performing full chip erase..."));
					this->Show_Progress(0.6);
				}

				strEraseInfo.Format(_T("erase complete"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("erase complete"));
				if (NULL != wcpHead)
				{
					//csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Status(_T("Erase complete..."));
					this->Show_Out(_T("Erase complete..."));
					this->Show_Progress(1);
					bDownloading_Flag = true;//完成
					break;
				}
			}
			//
			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 30)//超时，报fail
			{
				//如果异常后，使得下载按钮可用
				this->m_csError_Code = _T("Erase Timeout");
				this->Show_Out(this->m_csError_Code);
				throw(4);
			}				

		}
	}
	catch(...)
	{
		m_DL_FLAG = false;//端口下载取消 处于空闲态

		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);

		//关闭文件

		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		this->Show_Out(_T("Fail,ERASE_FLS_FILE_IN_EXE ----2"));


		return false;
	}
	//结束计时
	time(&t_End);

	m_DL_FLAG = false;//端口下载完成 回到空闲态

	//释放句柄的指针
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	//关闭文件
	if(hFile_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile_Handle);
	}

	this->Show_Out(_T("Pass,ERASE_FLS_FILE_IN_EXE ----1"));

	return true;
}

//------------------------------------------------------------------
//海思120平台下载核心
int CARM_OP_HUAWEI::Do_Execute_BC95_120()
{

	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpData = NULL;
	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;
	wchar_t * wcpHeadExt = NULL;

	wchar_t * wcpCmd = NULL;

	char * cpRead_Buff = NULL;

	time_t t_Start, t_End;

	int iTemp;

	bool bStatusFlag =false;
	bool bDownloading_Flag = false;
	bool bDC_Reset =false;
	bool bEraseFlag = false;

	BOOL bResetDC = TRUE;
	BOOL bDeDlflag = FALSE;
	BOOL bDeDlflagExt = FALSE;
	
	CString strClassIdx,strBatPath,strDis;
	CString  info1,info2,strDisI,strDinfo;
	CString strPos,strInfoDis;
	CString csTotal_Info, csData;

	double pos=0.0;

	unsigned long ulActual_Read;

	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	strClassIdx.Format(_T("%d"),m_iClass_Idx);

	m_iPpc=0;
	m_iPsc=0;
	m_iPac=0;

	m_iPsuc=0;
	m_iPauc=0;
	m_iPgc=0;


	iPsca_num=0;
	iPpa_num=0;
	iPsc_num=0;
	iPac_num=0;
	iSgc_num=0;

	//沿用原先的海思的120平台的下载

	int iTimeInterval= m_iTimeInterval;

	if (0 == iTimeInterval)
	{
		iTimeInterval = 4000;//20190613 海思120平台一般需要有2到3秒的上电复位的延时
	}

		
	if (1 == m_iBC95_EraseSwitch)//外部开关控制是否需要擦除
	{
#if 1	
		//重新上下电
		if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
		{
			this->Show_Out(_T("上电(查找版本前)"));
			::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
		}
		else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
		{
			csData.Format(_T("SPC重新上电-CH%d"),this->m_iClass_Idx);

			this->Show_Out(csData);

			this->Set_PowerOn_SPC(this->m_iClass_Idx);
		}
		else
		{
			//下载前上电
			if(true == this->m_bPLC_PowFlag)
			{
				this->Show_Out(_T("上电(查找版本前)"));
				this->Send_Open_PLC_Mode_Cmd();
				Sleep(3000);
			}

		}

		this->Show_Status(_T("Read FW Version......"));
		this->Show_Out(_T("Read FW Version......"));

		//***************************************************************
		//打开端口
		bStatusFlag = this->NB_Port_Open(9600);
		if (false == bStatusFlag)
		{
			csData.Format(_T("Port(%d) Open Fail-Read FW check"),m_iCom_Port);

			this->Show_Status(csData);	
			//错误信息
			this->m_csError_Code.Format(_T("%s"),csData);
			this->m_iError_Code= ERR_OPEN_PORT_FAIL;

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
			//throw(0);
		}
		else
		{
			m_iFindVerFlag = this->FindVer(m_iClass_Idx);

			if (1 == this->m_iFindVerFlag)//擦除版本
			{		
				this->Show_Status(_T("开始擦除......"));
				this->Show_Out(_T("开始擦除......"));

				bEraseFlag = this->ERASE_FLS_FILE_IN_EXE(m_iClass_Idx);

				Sleep(500);
				this->Show_Status(_T("擦除结束......"));
				this->Show_Out(_T("开始结束......"));
			}
			else
			{
				this->Show_Out(_T("未找到版本不需要擦除"));
			}

			this->NB_Port_Close();

		}
#endif
	}

#if 1
	if(TRUE == m_bCheckSaveLog)
	{
		//打开日志文件

		CString strLogPath = m_csCurr_Exe_Path + "\\NB-IoT\\" + strClassIdx + "\\log.txt";	

		hFile_Handle = ::CreateFile(strLogPath,
			GENERIC_WRITE,				//文件的操作属性 Read only required
			FILE_SHARE_READ,			//文件共享属性 read share file
			NULL,						//文件安全特性 SecurityAttributes - none in Win95
			CREATE_ALWAYS,				//文件操作 how to create
			FILE_FLAG_SEQUENTIAL_SCAN,	//文件属性 file attributes
			NULL	//如果不为零，则指定一个文件句柄。新文件将从这个文件中复制扩展属性 handle of file with attributes to copy
			);

		if(hFile_Handle == INVALID_HANDLE_VALUE)
		{
			CString info = strLogPath+" open failure";
			this->Show_Status(info);
			AfxMessageBox(info);
			return RTN_FAIL;
		}
	}
#endif
	try  
	{
		//初始显示
		this->Show_Status(_T("Waiting......"));


		if (0 == m_iBC95_EraseSwitch)
		{
			//重新上下电
			if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
			{
				this->Show_Out(_T("上电(查找版本前)"));
				::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
			}
			else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
			{
				csData.Format(_T("上电(查找版本前)-CH%d"),this->m_iClass_Idx);

				this->Show_Out(csData);

				this->Set_PowerOn_SPC(this->m_iClass_Idx);
			}
			else
			{
				//下载前上电
				if(true == this->m_bPLC_PowFlag)
				{
					this->Show_Out(_T("上电(查找版本前)"));
					this->Send_Open_PLC_Mode_Cmd();
					Sleep(3000);
				}

			}
		}

		//开始计时
		time(&t_Start);

		//初始化控制运行的相关参数
		this->m_bRuning = true;
		this->m_csError_Code.Format(_T(""));

		//------------------------------ 检查串口是否存在 ------------------------------ 
		if(this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iCom_Port) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iCom_Port);
			this->m_iError_Code=ERR_CONFIRM_PORT;

			this->Show_Status(this->m_csError_Code);
			throw(1);
		}

		//检查串口是否可打开 20180227
		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
		this->m_cla_Com_Port.m_iBaudRate = 9600;

		if(this->m_cla_Com_Port.Port_Open(400))
		{
			this->m_cla_Com_Port.Port_Close();
		}
		else
		{
			//错误信息
			this->m_csError_Code.Format(_T("FAIL, Port-%d In Using!"), this->m_iCom_Port);
			
			this->m_iError_Code = ERR_OPEN_PORT_FAIL;

			this->Show_Status(this->m_csError_Code);

			this->m_cla_Com_Port.Port_Close();
			throw(1);
		}
		
		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			AfxMessageBox(_T("Create pipe fail"));

			this->m_csError_Code.Format(_T("Create pipe fail"));
			this->m_iError_Code = ERR_CREATE_PIPE_FAIL;
			throw(1);

		}
		//------------------------------ 
		csTotal_Info.Format(_T(""));	

		//------------------------------ 打包dos指令 ------------------------------ 
		CString strCom;
		strCom.Format(_T("COM%d"),m_iCom_Port);

		CString strFirmwarePath = m_str_fwpkg_FW.cs_Path;
		CString strUEUpdaterPath;

		CString strPow;

		CString strExeName = _T("");
		switch(m_iClass_Idx)
		{
			case 1: strExeName = _T("UEUpdater1.exe"); break;
			case 2: strExeName = _T("UEUpdater2.exe"); break;
			case 3: strExeName = _T("UEUpdater3.exe"); break;
			case 4: strExeName = _T("UEUpdater4.exe"); break;
			default: strExeName = _T("UEUpdater.exe"); break;
		}

		strUEUpdaterPath.Format(_T("%s\\NB-IoT\\%d\\UEUpdaterCLI\\%s"),m_csCurr_Exe_Path,m_iClass_Idx,strExeName);
		//UEUpdater.exe检查文件是否存在
		unsigned long ul_File_Size =0;
		if(this->Get_File_Size(strUEUpdaterPath, &ul_File_Size) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, UEUpdater.exe File Not Find!"), this->m_iCom_Port);
			this->m_iError_Code = ERR_FIND_UEUPDATE_EXE_FAIL;

			this->Show_Status(this->m_csError_Code);
			throw(1);
		}

		if (strFirmwarePath.Find(_T('\ '))>=0)
		{
			this->m_csError_Code.Format(_T("FAIL,Firmware 路径有空格!"), this->m_iCom_Port);
			this->m_iError_Code = ERR_FIND_PATH_HAS_SPACE;

			this->Show_Status(this->m_csError_Code);
			throw(1);
		}

		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 4096);
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (strFirmwarePath.GetLength() +strUEUpdaterPath.GetLength() + 128));
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 4096);

		CString strDlFlag = _T("updateall");

// 		CString strDlFlag = _T("updatecores");
// 		if (m_iUpdateall == 1)
// 		{
/*			strDlFlag = _T("updateall");*/
// 		}
// 		else
// 		{
// 			strDlFlag = _T("updatecores");
// 		}

		//UEUpdater.exe updateall --in B650.fwpkg –-port com3
		if (m_iBC95BaudSwitch == 0) //默认支持高速波特率912600波特率切换
 		{
 			swprintf(wcpCmd, _T("cmd.exe %cc \"%s updateall \-\-in %s \-\-port %s\""), '/', strUEUpdaterPath,strFirmwarePath,strCom);
 		}
 		else
		{
			swprintf(wcpCmd, _T("cmd.exe %cc \"%s %s \-\-in %s \-\-port %s \-\-noBaudSwitch \-\-passive\""), '/', strUEUpdaterPath,strDlFlag,strFirmwarePath,strCom);
		}

		//--------------------保存Firmware 下载命令
		if(TRUE == m_bCheckSaveLog)
		{
			int iTemp;
			char* cp_Buff=NULL;
			unsigned long dwNumb=0; 
			//分配内存
			iTemp = (strFirmwarePath.GetLength() +strUEUpdaterPath.GetLength() + 128)*2+8;
			this->m_cla_GFC.BUFF_CHAR_CALLOC(&cp_Buff, iTemp);
			//加载数据
			this->m_cla_GFC.WideChar_To_MultiByte(wcpCmd, cp_Buff, &iTemp);

			if(::WriteFile(hFile_Handle, cp_Buff, iTemp, &dwNumb, NULL) != TRUE)
			{
				AfxMessageBox(_T("log file Write fail!"));
				this->m_cla_GFC.BUFF_CHAR_FREE(&cp_Buff);
				throw(1);
			}
			this->m_cla_GFC.BUFF_CHAR_FREE(&cp_Buff);
		}
		//-------------------


		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程

		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			AfxMessageBox(_T("Create process fail!"));

			this->m_csError_Code.Format(_T("Create process fail"));
			this->m_iError_Code = ERR_CREATE_PROCESS_FAIL;

			throw(1);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		this->Show_Status(_T("DL Start!"));
		this->Show_Progress(0.0);

		//清零
		csTotal_Info.Format(_T(""));
		while(false == bDownloading_Flag) 
		{
			//读info
			memset(cpRead_Buff, 0, 4096);
			ulActual_Read = 0;

			//读数据
			if(::ReadFile(hRead_Pipe, cpRead_Buff, 4095, &ulActual_Read, NULL) == NULL)
			{
				bDownloading_Flag = true;//20190626
			}

			if(TRUE == m_bCheckSaveLog)
			{
				//写头数据
				unsigned long dwNumb=0;  
				if(::WriteFile(hFile_Handle, cpRead_Buff, ulActual_Read, &dwNumb, NULL) != TRUE)
				{
					this->Show_Out(_T("log file Write fail"));
				}
			}
	
			//读到info数据了
			if(ulActual_Read > 0)
			{
				CString strNum,strInfo;

				m_DL_FLAG=	TRUE;

				//字符转换
				memset(wcpData, 0, 4096);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				//汇总数据
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
				//Please reset the UE
				strDinfo.Format(_T("trying to reset ue using at command"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), strDinfo);

				if (NULL != wcpHead)
				{
					this->Show_Status(_T("Please wait for a few minutes"));
					this->Show_Progress(0.1);
				}
				else
				{
					//Trying to reset UE using bootloader commands...
					wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("trying to reset ue using bootloader commands"));
					if (NULL == wcpHead)
					{
						//Waiting for bootloader to send info message...
						wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("waiting for bootloader"));
						if (NULL != wcpHead)
						{
							this->Show_Status(_T("Waiting for bootloader to send info message..."));
							//EVB情形下第一条关键信息
							bDeDlflag = TRUE;
							this->Show_Progress(0.1);
						}
						else
						{
							//Unable to verify bootloader connection - try resetting again...
							wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("try resetting again"));
							if (NULL != wcpHead)
							{
								this->Show_Status(_T("Unable to verify bootloader connection - try resetting again..."));

								//EVB情形下可能第二条关键信息
								bDeDlflagExt = TRUE;
								this->Show_Progress(0.1);
							}
						}
					}
					else
					{
						this->Show_Status(_T("Trying to reset ue using BootLoader commands"));
						this->Show_Out(_T("Trying to reset ue using BootLoader commands"));
						//模块高速下载情形下第一条关键信息
						bDeDlflag = TRUE;
						this->Show_Progress(0.1);
					}					
				}		
				if (NULL != wcpHead)
				{
					//m_iInfoDis++;
					this->Show_Progress(0.1);
					csTotal_Info.Format(_T("%s"), (wcpHead + 10));

					//if (1 < m_iInfoDis[iClass_Idx-1]) //第一次提示，第二次会自动下载无需重启
					if((TRUE ==bDeDlflag) ||(TRUE ==bDeDlflagExt))
					{		
						// 模块重启需等待
						if(TRUE ==bDeDlflag)
						{
							this->Show_Status(_T("Module Reset"));
						}
						else
						{
							this->Show_Status(_T("Module Reset Again"));
						}

						if(TRUE == m_bPowFlag)
						{
// 							ClosePower(iClass_Idx);//关闭电源	
// 							Sleep(4000);
// 							OpenPower(iClass_Idx);//打开电源
// 							Sleep(500);

						}
						else
						{		
							if(TRUE ==bDeDlflag)
							{
								this->Show_Status(_T("Module Reset By Hand"));

							}
							else
							{
								this->Show_Status(_T("Module Reset By Hand Again"));
							}			
						}
						

						if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
						{
							if ((m_iBC95_DC_ResetTime==1)
								&&(m_iBC95_DC_ResetTime_Flag == FALSE))
							{
								strPow.Format(_T("Module Reset2(状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);

								m_iBC95_DC_ResetTime_Flag = TRUE;
#ifdef FW_MULTI							
								//向总控PC发送DC重启指令
								//bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("REON") );
								bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
								Sleep(iTimeInterval);
								bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );

#endif
								strPow.Format(_T("Module Reset2(状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);
							} 
							else
							{
								strPow.Format(_T("Module Reset2(状态:%d:通道:%d-TS:%d-已复位,请等待)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);
								//让DC再一次重启
								if (TRUE == bResetDC)
								{
									bResetDC = FALSE;

									strPow.Format(_T("Module Reset2(状态:%d:通道:%d-TS:%d-开启2)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
									this->Show_Status(strPow);
									this->Show_Out(strPow);
#ifdef FW_MULTI
									bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
									Sleep(iTimeInterval);
									bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
#endif
									strPow.Format(_T("Module Reset2(状态:%d:通道:%d-TS:%d-完成2)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
									this->Show_Out(strPow);
								}
							}
						}
						else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
						{
							csData.Format(_T("Module Reset-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
						else
						{
							//重新上下电
							strPow.Format(_T("Module Reset By Hand"));
							this->Show_Status(strPow);
							this->Show_Out(strPow);
						}

						bDeDlflag = FALSE;//失效
						bDeDlflagExt= FALSE;//失效
					}
					//goto start;

				}
				//-------------------------------------------------------------
				//-------------------------------------------------------------
				strDinfo.Format(_T("initial bootinfo"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(),strDinfo);
				if (NULL != wcpHead)
				{
					info2.Format(_T("Initial bootInfo"));
					this->Show_Status(info2);
					this->Show_Progress(0.1);
					csTotal_Info.Format(_T("%s"), (wcpHead + info2.GetLength()));
					//Save_Trace_Log_File(strInfoDis);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("programming securitya core"));
				//大约30个
				if (NULL != wcpHead)
				{

					info2.Format(_T("Programming SecurityA core"));
					wcpHead+= info2.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iPsca_num++;
					if (iPsca_num<30)
					{
						pos=(double)iPsca_num/30;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);

					strPos.Format(_T("Programming SecurityA core......"));
					this->Show_Status(strPos);
					//Save_Trace_Log_File(strInfoDis);
				}

				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("programming protocola core"));
				//大约200个
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming ProtocolA core"));

					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iPpa_num++;
					if (iPpa_num<200)
					{
						pos=(double)iPpa_num/200;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);
					strPos.Format(_T("Programming ProtocolA core......"));
					this->Show_Status(strPos);
					//Save_Trace_Log_File(strInfoDis);
				}
				//15个
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("programming securityb core"));				
				if (NULL != wcpHead)
				{			
					strInfoDis.Format(_T("Programming SecurityB core"));
					wcpHead+=strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iPsc_num++;
					if (iPsc_num<15)
					{
						pos=(double)iPsc_num/15;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);
					strPos.Format(_T("Programming SecurityB core......"));
					this->Show_Status(strPos);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("programming applicationa core"));
				//33个				
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming ApplicationA core"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iPac_num++;
					if (iPac_num<33)
					{
						pos=(double)iPac_num/33;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);
					strPos.Format(_T("Programming ApplicationA core......"));
					this->Show_Status(strPos);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("setting front-end configuration"));		
				//4个
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Setting front-end configuration"));
					this->Show_Progress(0.7);
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					this->Show_Status(_T("Setting front-end configuration......"));
					//Save_Trace_Log_File(strInfoDis);

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("setting general configuration"));				
				//20个
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Setting general configuration"));
					wcpHead+=  strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), wcpHead);
					iSgc_num++;
					if (iSgc_num<20)
					{
						pos=(double)iSgc_num/20;
					} 
					else
					{
						pos=1;
					}
					this->Show_Progress(pos);
					strPos.Format(_T("Setting general configuration......"));
					this->Show_Status(strPos);
					//Save_Trace_Log_File(strInfoDis);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("sorry"));
				if (NULL != wcpHead)
				{
					//csTotal_Info.Format(_T("%s"), (wcpHead + 1));

					//
					this->m_csError_Code.Format(_T("FAIL,FW DL FAIL!"));
					this->m_iError_Code = ERR_FW_DL_FAIL;

					this->Show_Status(this->m_csError_Code);
					this->Show_Out(this->m_csError_Code);
					//增加对实际原因的打印
					this->Show_Out(csTotal_Info);

					this->Show_Progress(1.0);

					bDownloading_Flag =true;
					//Save_Trace_Log_File(strInfoDis);
					throw(1);
				}				 
				//wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("application finished"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("update complete"));
				if (NULL != wcpHead)
				{
					this->Show_Status(_T("DownLoad PASS"));
					
					this->Show_Out(_T("DownLoad PASS"));

					this->Show_Progress(1.0);

					bDownloading_Flag = true;//完成
					break;
				}
				//------------------------------
			}

			//
			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 300)//超时，报fail
			{
				//如果异常后，使得下载按钮可用
				this->m_csError_Code.Format(_T("FW Download Time Out"));
				this->m_iError_Code = ERR_FW_DL_TIMEOUT;

				this->Show_Out(_T("FW Download Time Out"));
				throw(4);
			}				

		}
	}

	catch(...)
	{
		time(&t_End);

		m_DL_FLAG = FALSE;//端口下载取消 处于空闲态

		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);

		//关闭文件
		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}

		//
		this->m_bRuning = false;
		this->Show_Out(_T("DownLoad FAIL"));

		return RTN_FAIL;
	}
	//结束计时
	time(&t_End);

	m_DL_FLAG= FALSE;//端口下载完成 回到空闲态

	//释放句柄的指针
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);


	//关闭文件
	if(hFile_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile_Handle);
	}


	if(TRUE == m_bPowFlag)
	{
		//ClosePower(iClass_Idx);//关闭电源	
	}

	//
	this->m_bRuning = false;

	this->Show_Out(_T("DownLoad PASS"));

	return RTN_SUCCESS;
}

int CARM_OP_HUAWEI::Do_Execute()
{
	int iDl_Res =RTN_FAIL;

	switch(m_iBBchip)
	{
	case BB_HI2152:
		iDl_Res = Do_Execute_HI2152();

		break;

	case BB_HI9500:
		iDl_Res = Do_Execute_HI2152_5G();

		break;

	case BB_HI2110:
		iDl_Res = Do_Execute_HI2110();

		break;
    //海思B200平台
	case BB_HI2120:
		iDl_Res = Do_Execute_HI2120();

		break;
		
	default:break;
	}
	return iDl_Res;
}



int CARM_OP_HUAWEI::Do_Execute_HI2110()
{
	CString  csData;
	int iDl_Res =RTN_FAIL;

	//初始化
	m_iBC95_DC_ResetTime_Flag= FALSE;
	m_iFindVerFlag =0;
	m_iPpc=0;
	m_iPsc=0;
	m_iPac=0;
	m_iPsuc=0;
	m_iPauc=0;
	m_iPgc=0;

	csData.Format(_T("DL Begin...(CH%d-TS%d)"),m_iClass_Idx,m_Ts_Num);

	this->Show_Out(csData);

	if ( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95D_INFO)>=0)
	{
		m_iFindFlagBCG_BC95D =1;
	} 
	else
	{
		m_iFindFlagBCG_BC95D =0;
	}

	//清空
	this->m_csError_Code.Format(_T(""));
	this->m_iError_Code=0;
	
   //---海思150平台下载--------------------------------------------------------------
	if(( m_str_fwpkg_FW.cs_Path.Find(VERSION_G_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC28_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC68_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC35_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95D_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B5JB_INFO)>=0)	
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B8JB_INFO)>=0)		
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B5JC_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B8JC_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC39_INFO)>=0)

		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B5JD_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC95B8JD_INFO)>=0)
		||( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC35GJD_INFO)>=0)

		)
	{
		//先用串口升级SSB.BIN,然后再升级Firmware
		this->m_iDL_flag =0;//初始化
		this->m_bRuning = true;//移到前面

		bool bBootFlag = this->m_bBootFlag;//保存下配置文件中参数 20190611
		int  iBC95_EraseSwitch = this->m_iBC95_EraseSwitch;
		bool  bNetFlagDL = m_bNetFlagDL;

		//**************************************************
		this->Show_Out(_T("DownLoad--1"));
		this->m_bBootFlag =false;//第一次下载无需检测SSB

		if (FALSE == m_bOutLine)//在线
		{
			//20191017
			if ((true == m_bIsRepMo) && (0 == m_iREP_NO_SSB))////非裸片REP工单
			{
				this->m_iBC95_EraseSwitch = 1;
			}
			else
			{
				this->m_iBC95_EraseSwitch = 0; //第一次下载无需查找版本擦除
			}
		}
		else
		{
			//离线的按照开关来
		}

		

		iDl_Res = this->Do_Execute_BC95G();

		this->m_bBootFlag = bBootFlag;//变量还原
		this->m_iBC95_EraseSwitch = iBC95_EraseSwitch;//变量还原
		//**************************************************


		//20190404 如果SSB下载成功，但核下载失败
		//兼容下：SSB下载失败
		//----再次下载第二次
		if (RTN_FAIL == iDl_Res)
		{
			if ((2 == this->m_iDL_flag)  //SSB 下载失败
				||(3 ==this->m_iDL_flag))//SSB 下载成功,但核下载失败
			{
				//**************************************************
				this->Show_Out(_T("DownLoad--2"));
				//m_bNetFlagDL = TRUE;//暂且认为已经下载过	
				//如果需要正常SSB检测

				//清空
				this->m_csError_Code.Format(_T(""));
				this->m_iError_Code=0;

				iDl_Res = this->Do_Execute_BC95G();
				//m_bNetFlagDL = bNetFlagDL;//变量还原
				//**************************************************

				//再做一次下载补救-----防止前面一次SSB检测失败 20190603
				if (RTN_FAIL == iDl_Res)
				{
					//**************************************************
					this->Show_Out(_T("DownLoad--3"));

					this->m_bNetFlagDL = TRUE;//暂且认为已经下载过
					this->m_iBC95_EraseSwitch = 0; //第三次也无需擦除
					this->m_bBootFlag =false;//第三次也无需SSB检测或者检测SSB

					this->m_iFindVerFlag =1;//暂且认为版本可查的

					//清空
					this->m_csError_Code.Format(_T(""));
					this->m_iError_Code=0;
					
					iDl_Res = this->Do_Execute_BC95G();

					this->m_bNetFlagDL = bNetFlagDL;//变量还原
					this->m_bBootFlag = bBootFlag;//变量还原
					this->m_iBC95_EraseSwitch = iBC95_EraseSwitch;//变量还原
					//**************************************************
				}
			}
			else
			{
				//...
				csData.Format(_T("DownLoad Fail---->%d"),this->m_iDL_flag);
				this->Show_Out(csData);
			}
		}
		else
		{
			//...
			csData.Format(_T("DownLoad Firmware Pass"));
			this->Show_Out(csData);
		}

		//下载完断电
		if (LOAD_AUTO_ROBOT != m_iOP_Robot) //手动线
		{
			if( m_str_fwpkg_FW.cs_Path.Find(VERSION_BC39_INFO)<0)
			{
				if(true == this->m_bPLC_PowFlag)
				{
					csData.Format(_T("14-CH%d DC OFF"),m_iClass_Idx);
					this->Show_Out(csData);
					this->Send_Close_PLC_Mode_Cmd();
					Sleep(2000);
				}
			}

		}
		this->m_bRuning = false;	
	}
	//-----------------------------------------------------------------------
	else
	{
		//沿用原先的海思的120平台的下载
		iDl_Res = this->Do_Execute_BC95_120();
	}

	return iDl_Res;
}

void CARM_OP_HUAWEI::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_HUAWEI::Show_Progress(double d_Pgr_Percent)
{
	//此函数中应该包含进度条百分比的计算，然后在消息响应函数中完成具体步进点数的计算
	//Progress总长度是1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}
bool CARM_OP_HUAWEI::LOAD_FW_FILE_HI2110(const wchar_t * wcp_FW_File_Path)
{

	//判定该 NB FW 路径下有多少可用FW文件
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CFileFind cla_Finder; //文件查找类
	BOOL b_Found;
	int j=0;
	wchar_t * wcprFW_Name[] = {_T("FWPKG")};

	try
	{
		//--------------------------------------- 开始解析信息，载入下载文件 --------------------------------------- 
		//清空Intel下载用文件列表
		this->m_str_fwpkg_FW.cs_Name.Format(_T(""));
		this->m_str_fwpkg_FW.cs_Path.Format(_T(""));

		//提取软件路径
		if(this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
		{
			this->m_csError_Code.Format(_T("ERROR, Get fls file directory fail!"));
			throw(0);
		}

		//寻找文件开始
		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);

		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		while(b_Found) 
		{
			b_Found = cla_Finder.FindNextFile();

			if(cla_Finder.IsDots())
			{
				continue;//找到的是当前目录或上级目录则跳过
			}

			if(!cla_Finder.IsDirectory())	//如果不是文件夹，便是文件
			{
				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//获取找到的文件名

				cs_FW_Name.MakeUpper();
				CString strFW_File_Path = wcp_FW_File_Path;
				strFW_File_Path.MakeUpper();

				
				//判定文件类型，载入.fls文件
				//if(wcsstr(cs_FW_Name, _T(".FWPKG")) != NULL)
				if(wcsstr(strFW_File_Path,cs_FW_Name) != NULL)

				{
					//去掉文件类型显示
					cs_FW_Name.Replace(_T(".FWPKG"), _T("\0"));

					m_str_fwpkg_FW.cs_Name.Format(_T("%s"), cs_FW_Name);

					m_str_fwpkg_FW.cs_Path.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

					m_str_fwpkg_FW.b_Enable_DL = true;
				}//if(wcsstr(cs_FW_Name, _T(".fwpkg")) != NULL
			}
		}//while(b_Found

		//
		cla_Finder.Close();
		//

		if(m_str_fwpkg_FW.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("ERROR, LOAD_FW_FILE, There is no %s.file!"), wcprFW_Name[j]);
			throw(0);
		}

	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	return true;
}

bool CARM_OP_HUAWEI::LOAD_FW_FILE_HI2152(const wchar_t * wcp_FW_File_Path)
{
	//判定该 LTE FW 路径下有多少可用FW文件
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Dir;
	CFileFind cla_Finder; //文件查找类
	BOOL b_Found;
	int j=0;

	try
	{
	    //20200428  不需要解析BAT文件，只要和bat同一个文件夹的都保存
		//
		//清空下载用文件列表
		for(j=0; j<FB_MAX; j++)
		{
			this->m_strr_Img_File_List[j].cs_Name.Format(_T(""));
			this->m_strr_Img_File_List[j].cs_Path.Format(_T(""));
			this->m_strr_Img_File_List[j].b_Enable_DL = false;
		}


		//提取软件路径
		if(this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
		{
			this->m_csError_Code.Format(_T("Error, Get mbn file directory fail!"));
			throw(0);
		}

		this->m_strUsbLoadPath = cs_FW_Dir;

		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);

		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		j=0;

		while(b_Found) 
		{
			b_Found = cla_Finder.FindNextFile();

			if(cla_Finder.IsDots())
			{
				continue;//找到的是当前目录或上级目录则跳过
			}

			if(!cla_Finder.IsDirectory())	//如果不是文件夹，便是文件
			{

				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//获取找到的文件名
				if (j<FB_MAX)
				{
					this->m_strr_Img_File_List[j].cs_Name.Format(_T("%s"),cs_FW_Name);
					this->m_strr_Img_File_List[j].cs_Path.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
					this->m_strr_Img_File_List[j].b_Enable_DL = true;
					j++;
				}
			}
		}

	}
	catch(...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

    return true;
}

bool CARM_OP_HUAWEI::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	bool bRet = false;

	switch(m_iBBchip)
	{
		case BB_HI2152:
		case BB_HI9500:
			bRet = LOAD_FW_FILE_HI2152(wcp_FW_File_Path);

			break;
		case BB_HI2110:
		case BB_HI2120:
			bRet = LOAD_FW_FILE_HI2110(wcp_FW_File_Path);

			break;
		default:break;
	}
	return bRet;
}

bool CARM_OP_HUAWEI::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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


int CARM_OP_HUAWEI::Do_Execute_BC95G(void)
{
	int iDL_Result1(RTN_FAIL),iDL_Result1_COM(RTN_FAIL),iDL_SSB_Result(RTN_FAIL);

	CString strCommand1,strCommand1_Com,strInfo;
	CString csData;

	unsigned char* cpData =NULL;
	wchar_t * wcpData = NULL;

	//参数iIndex为 1，2，3，4. ...
	int iIxIndex = this->m_iClass_Idx-1;

	int iBcg_ssb_com = m_iBcg_ssb_com;//通过配置文件开关 是否需要下载SSB

	bool bCheckSSB_Flag = false;
	bool bErase_Flag = false;
	bool bStatusFlag =false;

	m_i150ResetTime1=0;//150平台下载SSB复位1
	m_i150ResetTime2=0;//150平台下载SSB复位2
	m_i150ResetTime3=0;//150平台下载FW复位1
	m_i150ResetTime4=0;//150平台下载FW复位2

	CString strExeName = _T("");
	CString strSSB_ExeName = _T("");
	bool bFindExe = false;

	switch(m_iClass_Idx)
	{
		case 1: strExeName = _T("UEUpdater1.exe"); break;
		case 2: strExeName = _T("UEUpdater2.exe"); break;
		case 3: strExeName = _T("UEUpdater3.exe"); break;
		case 4: strExeName = _T("UEUpdater4.exe"); break;
		default:
			{
				AfxMessageBox(_T("m_iClass_Idx ERR"));
				strSSB_ExeName = _T("UEUpdater.exe"); 
			}
			break;

	}

	switch(m_iClass_Idx)
	{
		case 1: strSSB_ExeName = _T("SsbLoader1.exe"); break;
		case 2: strSSB_ExeName = _T("SsbLoader2.exe"); break;
		case 3: strSSB_ExeName = _T("SsbLoader3.exe"); break;
		case 4: strSSB_ExeName = _T("SsbLoader4.exe"); break;
		default: 
			{
				AfxMessageBox(_T("m_iClass_Idx ERR"));
				strSSB_ExeName = _T("SsbLoader.exe"); 
			}
			break;
	}

	if (FALSE == m_bOutLine)//在线
	{
		if ((true == m_bIsRepMo) && (0 == m_iREP_NO_SSB))////非裸片REP工单 ////REP工单 不需要检查SSB,肯定有SSB
		{
			if (1 == m_iBC95_EraseSwitch)//外部开关控制是否需要擦除
			{
				bErase_Flag = true;
			}
			else
			{
				//...
			}
			//
			bCheckSSB_Flag = false;//不需要检测ssb

		}

		if (TRUE == m_bNetFlagDL)//网络标志位>=DL //20190603 如果标志位大于下载标志位，无需再检测SSB --------------
		{
			if (1 == m_iBC95_EraseSwitch)//外部开关控制是否需要擦除
			{
				bErase_Flag = true;
			}
			else
			{
				//...
			}
			//
			bCheckSSB_Flag = false;//不需要检测ssb
			
		}
		else //
		{
			if(true == m_bBootFlag)//检测SSB的开关
			{
				bCheckSSB_Flag = true;
			}
			else
			{
				//...
				bCheckSSB_Flag = false;
			}

			//...
			if (1 == m_iBC95_EraseSwitch)//外部开关控制是否需要擦除
			{
				bErase_Flag = true;
			}
			else
			{
				//...
				bErase_Flag = false;
			}

		}
	} 
	else //离线
	{
		if(true == m_bBootFlag)//检测SSB的开关
		{
			bCheckSSB_Flag = true;
		}
		else
		{
			//...
			bCheckSSB_Flag = false;
		}
		if (1 == m_iBC95_EraseSwitch)//外部开关控制是否需要擦除
		{
			bErase_Flag = true;
		}
		else
		{
			//...
			bErase_Flag = false;
		}
	}


	try
	{
		//下载前上电 
		if (LOAD_AUTO_ROBOT ==  m_iOP_Robot) //全自动
		{
			csData.Format(_T("1-上电(下载前)-TS%d-CH%d"),this->m_Ts_Num,this->m_iClass_Idx);

			this->Show_Out(csData);

			::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( this->m_Ts_Num, _T("ON") );
		}
		else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动
		{
			csData.Format(_T("1-SPC上电(下载前)-CH%d"),this->m_iClass_Idx);

			this->Show_Out(csData);

			this->Set_PowerOn_SPC(this->m_iClass_Idx);
		}
		else
		{
			if(true == this->m_bPLC_PowFlag)//PLC
			{
				//csData.Format(_T("1-上电(下载前)-CH%d"),this->m_iClass_Idx);

				//this->Show_Out(csData);

				//this->Send_Open_PLC_Mode_Cmd();//20191125 是否需要？？？？
				//Sleep(2000);

			}
		}

		this->Show_Status(_T("Waiting......(150)"));

		this->Show_Out(_T("Waiting......(150)"));

		//初始化控制运行的相关参数
		this->m_csError_Code.Format(_T(""));

		//------------------------------ 检查串口是否存在 ------------------------------ 
		if(this->m_cla_GFC.Confirm_Reg_Com_Port(this->m_iCom_Port) == false)
		{
			//20190713
			//错误信息
			this->m_csError_Code.Format(_T("FAIL, Confirm com port %d fail!"), this->m_iCom_Port);
			this->m_iError_Code=ERR_CONFIRM_PORT;

			this->Show_Status(this->m_csError_Code);
			this->Show_Out(this->m_csError_Code);
			throw(0);
		}
		//开一次机检测有没有Boot字样 然后再关机
		//非裸片再去检测SSB这些 20190312

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//下载请必须擦除  ---------20200429
// 		bCheckSSB_Flag  = false;
// 		bErase_Flag = true;
// 		m_iFindVerFlag  = 1;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


		if(true == bCheckSSB_Flag)	
		{	
			this->Show_Status(_T("check SSB..."));
			this->Show_Out(_T("check SSB..."));
			//***************************************************************
			//打开端口
			g_SSB_Ctrl.Lock();

			bStatusFlag = this->NB_Port_Open(115200);
			if (false == bStatusFlag)
			{
				csData.Format(_T("Port(%d) Open Fail-SSB check"),m_iCom_Port);

				this->Show_Status(csData);	
				this->Show_Out(csData);
				//错误信息
				this->m_csError_Code.Format(_T("%s"),csData);
				this->m_iError_Code=ERR_OPEN_PORT_FAIL;

				g_SSB_Ctrl.Unlock();
				throw(0);
			}
			//Check SSB
			if(false == this->CheckSSB())
			{
				iBcg_ssb_com =1;//没有检测到SSB信息，后续需要下载SSB
			}
			else
			{
				iBcg_ssb_com =0;//检测到SSB信息，后续不需要下载SSB
			}
			//关闭端口
			this->NB_Port_Close();

			g_SSB_Ctrl.Unlock();
			//***************************************************************
			
		}//(true == bCheckSSB_Flag) 
		else
		{
			//...
		}

		csData.Format(_T("BC95_First_Erase:%d"),m_iBC95_EraseSwitchFirst);
		this->Show_Out(csData);

		if ((true == bErase_Flag)//是否需要擦除
			||(1 == m_iBC95_EraseSwitchFirst))//配置文件中的次开关打开就必须擦除
		{
//20191017 如果条件成立，直接擦除
			m_iFindVerFlag =1;//
#if 0
			this->Show_Status(_T("Read FW Version......"));
			this->Show_Out(_T("Read FW Version......"));

			//打开端口
			bStatusFlag = this->NB_Port_Open(9600);
			if (false == bStatusFlag)
			{
				csData.Format(_T("Port(%d) Open Fail-Erase check"),m_iCom_Port);

				this->Show_Status(csData);	
				//错误信息
				this->m_csError_Code.Format(_T("%s"),csData);
				this->m_iError_Code=ERR_OPEN_PORT_FAIL;

				csData.Format(_T("%s"),csData);
				this->Show_Out(csData);
				throw(0);
			}
			if (true ==  this->FindVerAuto())
			{
				m_iFindVerFlag =1;
				
			}
			else
			{
				m_iFindVerFlag =0;
				
			}
			//关闭端口
			this->NB_Port_Close();
#endif

			//先正常打开端口查询版本关闭端口后，再做擦除动作
			if (1 == m_iFindVerFlag)
			{
				this->Show_Status(_T("开始擦除......"));
				this->Show_Out(_T("开始擦除......"));

				this->ERASE_FLS_FILE_IN_EXE(m_iClass_Idx);

				//检查上一轮有没有遗留UEUpdater1.exe

				bFindExe = m_cla_GFC.Find_Process( strExeName);
				if (true == bFindExe) //.exe 还存在，继续等待
				{
					bFindExe = m_cla_GFC.Kill_Process(strExeName);
					if (false ==  bFindExe)
					{
						csData.Format(_T("%s Del FAIL"),strExeName);
						this->Show_Out(csData);
					} 
					else
					{
						csData.Format(_T("%s Del PASS"),strExeName);
						this->Show_Out(csData);
					}
				}

				this->Show_Status(_T("擦除结束......"));
				this->Show_Out(_T("擦除结束......"));
			} 
			else
			{
				this->Show_Out(_T("未找到版本不需要擦除"));
			}

		}//true == bErase_Flag
		else
		{
			//...
			this->Show_Out(_T("擦除开关关闭不需要擦除"));

		}
		//如果能查询到Firmware的，则认为是模块已经有SSB，不能再升级SSB
		if (1 == m_iFindVerFlag)
		{
			iBcg_ssb_com =0;
		}

		strInfo.Format(_T("CH%d-\\(WrCon=%d-BaudSwitch=%d-Bcg_ssb_com=%d-WriteKV=%d-WriteFEC=%d\\)"),this->m_iClass_Idx,m_blWrCon,m_iBC95BaudSwitch,iBcg_ssb_com,m_bWriteKV,m_bWriteFEC);
		this->Show_Out(strInfo);

		//--WriteKV QFLASH 升级需要加入

		if (m_blWrCon == TRUE)//整个升级包括射频参数等
		{
			if (m_iBC95BaudSwitch == 0) //默认支持高速波特率912600波特率切换
			{
				strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updateall -p COM%d -i %s"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
			}
			else
			{
				strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updateall -p COM%d -i %s  --noBaudSwitch  --passive"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
			}
		}
		else
		{
			if ((TRUE ==  m_bWriteKV) && (TRUE ==  m_bWriteFEC))
			{
				if (m_iBC95BaudSwitch == 0) //默认支持高速波特率912600波特率切换
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s --writeKV --writeFec"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
				else
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --noBaudSwitch  --passive --writeKV --writeFec"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
			}
			else if ((FALSE ==  m_bWriteKV) && (TRUE ==  m_bWriteFEC))
			{
				if (m_iBC95BaudSwitch == 0) //默认支持高速波特率912600波特率切换
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --writeFec"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
				else
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --noBaudSwitch  --passive --writeFec"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
			}
			else if ((TRUE ==  m_bWriteKV) && (FALSE ==  m_bWriteFEC))
			{
				if (m_iBC95BaudSwitch == 0) //默认支持高速波特率912600波特率切换
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s --writeKV"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
				else
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --noBaudSwitch  --passive --writeKV"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
			}
			else
			{
				if (m_iBC95BaudSwitch == 0) //默认支持高速波特率912600波特率切换
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
				else
				{
					strCommand1.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\UEUpdaterCLI\\%s updatecores -p COM%d -i %s  --noBaudSwitch  --passive"),_T('/'),m_csCurr_Exe_Path,m_iClass_Idx,strExeName,this->m_iCom_Port,m_str_fwpkg_FW.cs_Path);
				}
			}
		}

		

		// 	//工厂裸片方式下载
		if(m_iFindFlagBCG_BC95D ==1)
		{
			strCommand1_Com.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\BC95_G\\SsbUpdater\\%s loadfirst -p COM%d -i %s\\NB-IoT\\%d\\BC95_G\\ssb-BC95-D.bin"),'/',m_csCurr_Exe_Path,m_iClass_Idx,strSSB_ExeName,this->m_iCom_Port,m_csCurr_Exe_Path,m_iClass_Idx);
		}
		else //BC95-G/BC35/BC68系列
		{
			strCommand1_Com.Format(_T("cmd.exe %cc %s\\NB-IoT\\%d\\BC95_G\\SsbUpdater\\%s loadfirst -p COM%d -i %s\\NB-IoT\\%d\\BC95_G\\ssb-BC95-G.bin"),'/',m_csCurr_Exe_Path,m_iClass_Idx,strSSB_ExeName,this->m_iCom_Port,m_csCurr_Exe_Path,m_iClass_Idx);
		}

		this->Show_Progress(0.0);

		// 	//下载前需查询IMEI并保存
		// 	OnReadAtImei();

		//是否用串口下载SSB.BIN ----重要工厂裸片需要串口下载SSB.BIN
#if 1
		if (1 == iBcg_ssb_com)
		{	
			this->Show_Status(_T("DL SSB..."));
			this->Show_Out(_T("DL SSB..."));

			iDL_Result1_COM = DL_FLS_FILE_IN_EXE_BCG_COM(strCommand1_Com);//下载SSB

			//可能处理太快，SSB.EXE还未来得及释放
			Sleep(1000);


			bFindExe = m_cla_GFC.Find_Process( strSSB_ExeName);
			if (true == bFindExe) //.exe 还存在，继续等待
			{
				bFindExe = m_cla_GFC.Kill_Process(strSSB_ExeName);
				if (false ==  bFindExe)
				{
					csData.Format(_T("%s Del ssb exe FAIL"),strSSB_ExeName);
					this->Show_Out(csData);
				} 
				else
				{
					csData.Format(_T("%s Del ssb exe PASS"),strSSB_ExeName);
					this->Show_Out(csData);
				}
			}

			bFindExe = m_cla_GFC.Find_Process( strExeName);
			if (true == bFindExe) //.exe 还存在，继续等待
			{
				bFindExe = m_cla_GFC.Kill_Process(strExeName);
				if (false ==  bFindExe)
				{
					csData.Format(_T("%s Del FAIL"),strExeName);
					this->Show_Out(csData);
				} 
				else
				{
					csData.Format(_T("%s Del PASS"),strExeName);
					this->Show_Out(csData);
				}
			}

			if (RTN_SUCCESS == iDL_Result1_COM)
			{
				m_iDL_flag = 1;//SSB 下载成功

				this->Show_Out(_T("DL Firmware..."));
				this->Show_Status(_T("DL Firmware..."));
				this->Show_Out(strCommand1);

				iDL_Result1 = DL_FLS_FILE_IN_EXE_BCG(strCommand1);//下载FW

				if (RTN_SUCCESS == iDL_Result1)
				{
					m_iDL_flag = 4;//SSB 下载成功,核下载成功
					m_PassNumb++;
					Sleep(2000);
				} 
				else
				{
					m_iDL_flag = 3;//SSB 下载成功,但核下载失败				
					throw(1);
				}
				
			}
			else
			{
				m_iDL_flag = 2;//SSB 下载失败
				throw(2);
			}
		} 
		else
#endif
		{
			this->Show_Status(_T("DL Firmware..."));

			iDL_Result1 = DL_FLS_FILE_IN_EXE_BCG(strCommand1);//下载FW
			if (RTN_SUCCESS == iDL_Result1)
			{
				m_PassNumb++;
			} 
			else
			{
				throw(3);
			}		 
			Sleep(2000);
			//
			bFindExe = m_cla_GFC.Find_Process( strSSB_ExeName);
			if (true == bFindExe) //.exe 还存在，继续等待
			{
				bFindExe = m_cla_GFC.Kill_Process(strSSB_ExeName);
				if (false ==  bFindExe)
				{
					csData.Format(_T("%s Del ssb exe FAIL"),strSSB_ExeName);
					this->Show_Out(csData);
				} 
				else
				{
					csData.Format(_T("%s Del ssb exe PASS"),strSSB_ExeName);
					this->Show_Out(csData);
				}
			}

			bFindExe = m_cla_GFC.Find_Process( strExeName);
			if (true == bFindExe) //.exe 还存在，继续等待
			{
				bFindExe = m_cla_GFC.Kill_Process(strExeName);
				if (false ==  bFindExe)
				{
					csData.Format(_T("%s Del FAIL"),strExeName);
					this->Show_Out(csData);
				} 
				else
				{
					csData.Format(_T("%s Del PASS"),strExeName);
					this->Show_Out(csData);
				}
			}


		}
	}
	catch (...)
	{

		this->m_DL_FLAG = false;

		this->m_ErrorNumb++;
		Sleep(2000);

		return RTN_FAIL;

	}
	return RTN_SUCCESS;

}

int CARM_OP_HUAWEI::DL_FLS_FILE_IN_EXE_BCG(CString strCommand)
{
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	unsigned long ulActual_Read;
	int iTemp;
	CString csTotal_Info, csData,strDataInfo;
	double dPercent = 0.0;
	time_t t_Start, t_End;
	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;

	wchar_t * wcpHead_Temp = NULL;

	bool bDownloading_Flag = false,bDC_Reset =false;

	CString strDis,strInfo,strPow;
	wchar_t * wcpCmd = NULL;

	int iPpa_num  =0,iPsc_num  =0,iPac_num =0,iSgc_num =0,iPsca_num=0;
	int pos =0;
	CString strPos,strInfoDis,strInfoDis_T;

	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	BOOL bLogSave = FALSE;

	CString strLogPath;

	time_t start, finish;

	time_t start_T, finish_T;

	int iTimeOut =150;//

#ifdef FW_MULTI
	iTimeOut =150;
#else
	iTimeOut =230; //主要是考虑客户升级可能采用慢速下载，可能超过150秒
#endif

	try  
	{
		time( &start );

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			AfxMessageBox(_T("Create pipe fail"));

			this->m_csError_Code.Format(_T("Create pipe fail"));
			this->m_iError_Code = ERR_CREATE_PIPE_FAIL;

			throw(0);

		}
		//------------------------------ 
#ifndef FW_MULTI 
		if(TRUE == m_bCheckSaveLog)
		{
			//打开日志文件

			strLogPath.Format(_T("%s\\NB-IoT\\%d\\log.txt"),m_csCurr_Exe_Path,m_iClass_Idx);

			hFile_Handle = ::CreateFile(strLogPath,
				GENERIC_WRITE,				//文件的操作属性 Read only required
				FILE_SHARE_READ,			//文件共享属性 read share file
				NULL,						//文件安全特性 SecurityAttributes - none in Win95
				CREATE_ALWAYS,				//文件操作 how to create
				FILE_FLAG_SEQUENTIAL_SCAN,	//文件属性 file attributes
				NULL	//如果不为零，则指定一个文件句柄。新文件将从这个文件中复制扩展属性 handle of file with attributes to copy
				);

			CString info;
			if(hFile_Handle == INVALID_HANDLE_VALUE)
			{
				info.Format(_T("%s open Fail"),strLogPath);
				this->Show_Status(info);
				this->Show_Out(info);
				bLogSave = FALSE;
			}
			else
			{

				info.Format(_T("%s open Pass"),strLogPath);
				this->Show_Out(info);

				bLogSave = TRUE;

			}
		}
		else
		{
			bLogSave = FALSE;
		}
#endif
		//---------------------------------------------------------

		csTotal_Info.Format(_T(""));
		strDataInfo.Format(_T(""));

		//------------------------------ 打包dos指令 ------------------------------ 

		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (strCommand.GetLength() + 128));

		swprintf(wcpCmd, _T("%s"), strCommand);

		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄e
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的

		csData.Format(_T("FW:Write_Pipe=%d,hRead_Pipe=%d"),hWrite_Pipe,hRead_Pipe);
		this->Show_Out(csData);

		//
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code.Format(_T("Create process fail"));
			this->m_iError_Code = ERR_CREATE_PROCESS_FAIL;


			AfxMessageBox(_T("Create process fail!"));
			throw(1);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		//开始计时
		time(&t_Start);
		while(false == bDownloading_Flag) 
		{
			//读info
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;

			//读数据
			if(::ReadFile(hRead_Pipe, cpRead_Buff, 2047, &ulActual_Read, NULL) == NULL)
			{
				bDownloading_Flag = true;//20190626
				
				this->Show_Status(_T("FW ReadFile NULL"));
				this->Show_Progress(1.0);

				this->Show_Out(_T("FW ReadFile NULL"));
				this->m_csError_Code.Format(_T("FW ReadFile NULL"));
				this->m_iError_Code = ERR_FW_ReadFile_NULL;	
				throw(0);
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				this->Show_Status(_T("FW Download Time Out"));
				this->Show_Progress(1.0);

				this->Show_Out(_T("FW Download Time Out"));
				this->m_csError_Code.Format(_T("FW Download Time Out"));
				this->m_iError_Code = ERR_FW_DL_TIMEOUT;
				throw(0);
			}

			//读到info数据了
			if(ulActual_Read > 0)
			{
				m_DL_FLAG = true;

				//字符转换
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				if(TRUE == bLogSave)
				{
					//写头数据
					unsigned long dwNumb=0;  
					if(::WriteFile(hFile_Handle, cpRead_Buff, ulActual_Read, &dwNumb, NULL) != TRUE)
					{
						this->Show_Out(_T("log file Write fail"));
					}
				}
				else
				{
					//..
				}
				//汇总数据
				csTotal_Info += csData;
				strDataInfo = csTotal_Info;

				//Waiting for bootloader to send info message
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Waiting for bootloader to send info message"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Waiting for bootloader to send info message"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));

					this->Show_Status(strInfoDis);
					this->Show_Out(strInfoDis);
					this->Show_Progress(0.1);

					if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
					{						
#ifdef FW_MULTI							
						//如果第一次没响应，主动第二次复位
						m_i150ResetTime3++;
						if(m_i150ResetTime3 ==2)
						{
							strPow.Format(_T("Module Reset2(FW1-状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);

							csData.Format(_T("10-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(2000);

							csData.Format(_T("10-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );

							strPow.Format(_T("Module Reset2(FW1-状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
						}
#endif
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
					{
						m_i150ResetTime3++;
						if(m_i150ResetTime3 ==2)
						{
							csData.Format(_T("10-SPC重新上电-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
					}
					else
					{
						if(true == this->m_bPLC_PowFlag)
						{
							m_i150ResetTime3++;
							if(m_i150ResetTime3 ==2)
							{
								strPow.Format(_T("Module Reset2(FW1-状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);

								csData.Format(_T("10-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(2000);

								csData.Format(_T("10-CH%d DC 0N"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Open_PLC_Mode_Cmd();

								strPow.Format(_T("Module Reset2(FW1-状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
								this->Show_Status(strPow);
								this->Show_Out(strPow);
							}
						}
						else
						{						
							m_i150ResetTime3++;
							if(m_i150ResetTime3 ==2)
							{
								this->Show_Status(_T("Reset"));
							}
						}
					}
				}
				//Trying to reset UE using bootloader commands
				//Trying to reset UE using AT command
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Trying to reset UE using bootloader commands"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Trying to reset UE using bootloader commands"));
					wcpHead+= strInfoDis.GetLength();

					csTotal_Info.Format(_T("%s"), (wcpHead + 1));

					this->Show_Status(strInfoDis);
					this->Show_Out(strInfoDis);
					this->Show_Progress(0.1);

					//增加下上下电
					if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
					{
						strPow.Format(_T("Module Reset1(FW2状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);
						this->Show_Out(strPow);
#ifdef FW_MULTI	
						//如果第一次没响应，主动第二次复位
						m_i150ResetTime4++;
						if(m_i150ResetTime4 ==2)
						{
							csData.Format(_T("11-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(2000);

							csData.Format(_T("11-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}
#endif
						strPow.Format(_T("Module Reset1(FW2状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);
						this->Show_Out(strPow);
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
					{
						//如果第一次没响应，主动第二次复位
						m_i150ResetTime4++;
						if(m_i150ResetTime4 ==2)
						{
							csData.Format(_T("11-SPC重新上电-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}					
					}
					else
					{
						//PLC模式暂不重新上下电
						if(true == this->m_bPLC_PowFlag)
						{
							m_i150ResetTime4++;
							if(m_i150ResetTime4 ==2)
							{
								csData.Format(_T("11-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(2000);

								csData.Format(_T("11-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Open_PLC_Mode_Cmd();
							}
						}
						else
						{
							this->Show_Out(_T("PLC NOT SUPPORT(FW DL)"));
							
							//如果第一次没响应，主动第二次复位
							m_i150ResetTime4++;
							if(m_i150ResetTime4 ==2)
							{
								this->Show_Status(_T("Reset"));
							}				

						}
					}
				}
				//try resetting again
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("try resetting again"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("try resetting again"));
					wcpHead+= strInfoDis.GetLength();

					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					//Save_Trace_Log_File(_T("UE RESET"));
					this->Show_Status(strInfoDis);
					this->Show_Progress(0.1);

					this->Show_Out(strInfoDis);

					if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
					{
						csData.Format(_T("12-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
						this->Show_Out(csData);

						bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
						Sleep(2000);

						csData.Format(_T("12-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
						this->Show_Out(csData);

						bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
					{
						csData.Format(_T("12-SPC重新上电-CH%d"),this->m_iClass_Idx);

						this->Show_Out(csData);

						this->Set_PowerOn_SPC(this->m_iClass_Idx);
					}
					else
					{
						if(true == this->m_bPLC_PowFlag)//半自动PLC下载夹具
						{
							csData.Format(_T("12-CH%d DC OFF"),m_iClass_Idx);
							this->Show_Out(csData);

							this->Send_Close_PLC_Mode_Cmd();
							Sleep(2000);

							csData.Format(_T("12-CH%d DC ON"),m_iClass_Idx);
							this->Show_Out(csData);

							this->Send_Open_PLC_Mode_Cmd();
						}
						else
						{
							this->Show_Status(_T("Reset..."));
						}
					}
					continue;

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Initial bootInfo"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Initial bootInfo"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Status(strInfoDis);
					this->Show_Progress(0.12);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming SecurityA core"));
				//大约10个
				if (NULL != wcpHead)
				{
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					iPsca_num++;
					if (iPsca_num<10)
					{
						pos=(iPsca_num*100)/10;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					
					strPos.Format(_T("Programming SecurityA core:%d%%..."),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);
				}

				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming ProtocolA core...  "));

				//大约200个
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming ProtocolA core..."));

					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 2));
					iPpa_num++;
					if (iPpa_num<200)
					{
						pos=(iPpa_num*100)/200;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					strPos.Format(_T("Programming ProtocolA core:%d%%......"),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);

				}

				//15个
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming SecurityB core"));				
				if (NULL != wcpHead)
				{			
					strInfoDis.Format(_T("Programming SecurityB core..."));
					wcpHead+=strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 2));
					iPsc_num++;
					if (iPsc_num<15)
					{
						pos=(iPsc_num*100)/15;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					strPos.Format(_T("Programming SecurityB core:%d%%......"),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);
				}
				//wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Setting front-end configuration"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming ApplicationA core"));
				//33个				
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming ApplicationA core..."));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 2));
					iPac_num++;
					if (iPac_num<33)
					{
						pos=(iPac_num*100)/33;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					strPos.Format(_T("Programming ApplicationA core:%d%%......"),pos);		

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);
				}
				//wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Setting general configuration"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Setting front-end configuration"));		
				//4个
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Setting front-end configuration"));

					wcpHead+= strInfoDis.GetLength();
		
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
	
					this->Show_Status(strInfoDis);
					this->Show_Progress(0.65);

				}
				//wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming Application core"));
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Setting general configuration"));				
				//20个
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Setting general configuration"));
					wcpHead+=  strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					iSgc_num++;
					if (iSgc_num<20)
					{
						pos=(iSgc_num*100)/20;
					} 
					else
					{
						pos=100;
					}
					dPercent = (double)pos/100;
					strPos.Format(_T("Setting general configuration:%d%%......"),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dPercent);
				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Sorry"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Sorry"));
					bDownloading_Flag =true;

					this->m_csError_Code.Format(_T("FAIL,FW DL FAIL!"));
					this->m_iError_Code = ERR_FW_DL_FAIL;

					this->Show_Out(strInfoDis);

					throw(1);
				}	
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Update complete"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Update complete"));
					this->Show_Out(strInfoDis);
					bDownloading_Flag =true;
					break;
				}			
			}
		}
	}
	catch(...)
	{
		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		m_DL_FLAG = false;


		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);

		this->Show_Status(_T("Download Fail"));
		this->Show_Progress(1.0);

		if(hFile_Handle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hFile_Handle);
		}


		return RTN_FAIL;
	}
	//释放句柄的指针
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}

	m_DL_FLAG= false;

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	//Save_Trace_Log_File(_T("DL PASS"));
	this->Show_Status(_T("Download Pass"));
	this->Show_Progress(1.0);

	if(hFile_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile_Handle);
	}
	return RTN_SUCCESS;
}


int CARM_OP_HUAWEI::DL_FLS_FILE_IN_EXE_BCG_COM(CString strCommand)
{

	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int iTemp;
	CString csTotal_Info, csData,strDataInfo;
	time_t t_Start, t_End;

	time_t t_Start_T, t_End_T;

	wchar_t * wcpHead = NULL;
	wchar_t * wcpEnd = NULL;


	bool bDownloading_Flag = false,bDC_Reset =false;

	double dAdd_Percent;//下载各步骤所占有的进度条份额

	CString strDis,strInfo,strPow;
	wchar_t * wcpCmd = NULL;

	int iPpa_num  =0,iPsc_num  =0,iPac_num =0,iSgc_num =0,iPsca_num=0;
	int pos =0;
	CString strPos,strInfoDis;

	int iTimeOut =40;//海思提供的是45秒的时间

	int iTimeOut_DC =8;// 如果8秒没有任何响应的话，再DC重启下尝试

	int  iStart =0;

	BOOL  bFind_Process =FALSE;


	CString  strSSB_ExeName =_T("");
	switch(m_iClass_Idx)
	{
		case 1: strSSB_ExeName = _T("SsbLoader1.exe"); break;
		case 2: strSSB_ExeName = _T("SsbLoader2.exe"); break;
		case 3: strSSB_ExeName = _T("SsbLoader3.exe"); break;
		case 4: strSSB_ExeName = _T("SsbLoader4.exe"); break;
		default:  
			{
				AfxMessageBox(_T("m_iClass_Idx ERR"));
				strSSB_ExeName = _T("SsbLoader.exe"); 
			}
			break;

	}

	try  
	{

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承


		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			AfxMessageBox(_T("Create pipe fail"));

			this->m_csError_Code.Format(_T("Create pipe fail"));
			this->m_iError_Code = ERR_CREATE_PIPE_FAIL;

			throw(0);
		}
		//------------------------------ 

		csTotal_Info.Format(_T(""));
		strDataInfo.Format(_T(""));
		//------------------------------ 打包dos指令 ------------------------------ 

		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (strCommand.GetLength() + 128));

		swprintf(wcpCmd, _T("%s"), strCommand);
		//Save_Trace_Log_File(strCommand);

		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			AfxMessageBox(_T("Create process fail!"));
			this->m_csError_Code.Format(_T("Create process fail"));
			this->m_iError_Code = ERR_CREATE_PROCESS_FAIL;	
			throw(1);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		//开始计时
		time(&t_Start);

		time( &t_Start_T );//20190725
		
		int iFirstFlag =1,iDC_Flag=1;

		int i =0,iTimeTemp =0;
		
		while(false == bDownloading_Flag) 
		{
			//读info
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;

			if (FALSE == ::PeekNamedPipe(hRead_Pipe,cpRead_Buff,2047,&ulActual_Read,0,0))
			{
				//...
				//this->Show_Out(_T("PeekNamedPipe Fail"));

				//------------------------------------------------
				time( &t_End );
				iTimeTemp = (int)difftime( t_End,t_Start);
				//
				if( iTimeTemp>= iTimeOut )
				{
					//发个消息将SSB.EXE文件杀掉
					bool bKill_Flag =false;
					bKill_Flag = m_cla_GFC.Kill_Process(strSSB_ExeName);
					if (false ==  bKill_Flag)
					{
						csData.Format(_T("%s Del FAIL_1"),strSSB_ExeName);
						this->Show_Out(csData);
					} 
					else
					{
						csData.Format(_T("%s Del PASS_1"),strSSB_ExeName);
						this->Show_Out(csData);
					}

				
					for(i =0; i<20;i++)
					{	
						csData.Format(_T("Wait SSB Del......%d"),i);

						this->Show_Status(csData);

						bFind_Process = m_cla_GFC.Find_Process( strSSB_ExeName);

						if (TRUE == bFind_Process) //SsbLoader.exe 还存在，继续等待
						{
							Sleep(500);
							continue;
						}
						else
						{
							break;//SsbLoader.exe已经删除成功
						}
											
					}
	
					this->Show_Status(_T("SSB.BIN Download Time Out_1"));
					this->Show_Progress(1.0);
					this->Show_Out(_T("SSB.BIN Download Time Out_1"));

					this->m_csError_Code.Format(_T("SSB.BIN Download Time Out_1"));
					this->m_iError_Code = ERR_SSB_DL_TIMEOUT;

					throw(0);
				}
				else
				{
					//...
				}
				//------------------------------------------------

			} 
			else
			{
				if(ulActual_Read>0)
				{
					//读数据
					if(::ReadFile(hRead_Pipe, cpRead_Buff, 2047, &ulActual_Read, NULL) == NULL)
					{
						bDownloading_Flag = true;//20190626
						this->Show_Out(_T("SSB ReadFile NULL"));
						this->m_csError_Code.Format(_T("SSB ReadFile NULL"));
						this->m_iError_Code = ERR_SSB_ReadFile_NULL;	
					}
					else
					{
						//
						time( &t_Start_T );
					}
				}
				else
				{
					//...
					time( &t_End );
					iTimeTemp = (int)difftime( t_End,t_Start_T);
					//
					if( iTimeTemp >= iTimeOut )
					{
						//发个消息将SSB.EXE文件杀掉
						bool bKill_Flag =false;
						bKill_Flag = m_cla_GFC.Kill_Process(strSSB_ExeName);
						if (false ==  bKill_Flag)
						{
							csData.Format(_T("%s Del FAIL_2"),strSSB_ExeName);
							this->Show_Out(csData);
						} 
						else
						{
							csData.Format(_T("%s Del PASS_2"),strSSB_ExeName);
							this->Show_Out(csData);
						}

						for(i =0; i<20;i++)
						{	
							csData.Format(_T("Wait SSB Del......%d"),i);

							this->Show_Status(csData);

							bFind_Process = m_cla_GFC.Find_Process( strSSB_ExeName);

							if (TRUE == bFind_Process) //SsbLoader.exe 还存在，继续等待
							{
								Sleep(500);
								continue;
							}
							else
							{
								break;//SsbLoader.exe已经删除成功
							}

						}

						this->Show_Status(_T("SSB.BIN Download Time Out_2"));
						this->Show_Progress(1.0);

						this->Show_Out(_T("SSB.BIN Download Time Out_2"));
						this->m_csError_Code.Format(_T("SSB.BIN Download Time Out_2"));
						this->m_iError_Code = ERR_SSB_DL_TIMEOUT;
						throw(0);
					}
					else
					{
						if( iTimeTemp == iTimeOut_DC )//超时8秒无响应的话，RESET下DC
						{
							if (LOAD_AUTO_ROBOT ==  m_iOP_Robot)
							{	
								//再上一次电
								if (0 == iStart)//首次
								{
									csData.Format(_T("3-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
									this->Show_Out(csData);

									bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF"));
									Sleep(2000);

									csData.Format(_T("3-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
									this->Show_Out(csData);

									bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
								} 
								iStart++;

							}//(LOAD_AUTO_ROBOT ==  m_iOP_Robot)

							else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动
							{
								csData.Format(_T("3-SPC重新上电-CH%d"),this->m_iClass_Idx);

								this->Show_Out(csData);

								this->Set_PowerOn_SPC(this->m_iClass_Idx);
							}
							else
							{
								if(true == this->m_bPLC_PowFlag)
								{
									//再上一次电
									if (0 == iStart)//首次
									{
										csData.Format(_T("3-CH%d DC OFF"),m_iClass_Idx);
										this->Show_Out(csData);

										this->Send_Close_PLC_Mode_Cmd();
										Sleep(2000);

										csData.Format(_T("3-CH%d DC ON"),m_iClass_Idx);
										this->Show_Out(csData);

										this->Send_Open_PLC_Mode_Cmd();
									} 
									iStart++;
								}
								else
								{
									if (0 == iStart)//首次
									{
										csData.Format(_T("请手动模块复位(2)"));
										this->Show_Status(csData);
									}
									iStart++;
								}
							}//else

						}//( iTimeTemp == 8 )
						else
						{
							//
// 							csData.Format(_T("Auto Reset,Please Wait...%d"),iTimeTemp);
// 							this->Show_Status(csData);
						}

					}//else
				}

			}

			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));
				this->Show_Out(csData);

				//汇总数据
				csTotal_Info += csData;
				strDataInfo = csTotal_Info;

				//Waiting for bootloader to send info message
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Waiting for bootloader to send info message"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Waiting for bootloader to send info message"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Progress(0.1);

					if (LOAD_AUTO_ROBOT ==  m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
					{
						strPow.Format(_T("Module Reset1(SSB1状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);

						strPow.Format(_T("%s"),strPow);
						this->Show_Out(strPow);
#ifdef FW_MULTI	
						if (m_iTimeInterval>0)
						{
							//向总控PC发送DC重启指令
							csData.Format(_T("4-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(m_iTimeInterval);

							csData.Format(_T("4-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}					

						//如果第一次没响应，主动第二次复位
						m_i150ResetTime1++;
						if(m_i150ResetTime1 ==2)
						{
							csData.Format(_T("5-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(2000);

							csData.Format(_T("5-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);
							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}
#endif
						strPow.Format(_T("Module Reset1(SSB1状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);

						strPow.Format(_T("%s"),strPow);
						this->Show_Out(strPow);
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
					{
						if (m_iTimeInterval>0)//20200529 暂且用该方式限制上下电
						{
							csData.Format(_T("4-SPC重新上电-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
						//如果第一次没响应，主动第二次复位
						m_i150ResetTime1++;
						if(m_i150ResetTime1 ==2)
						{
							csData.Format(_T("4-SPC重新上电-2-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}

					}
					else
					{
						//PLC模式暂不重新上下电
						if(true == this->m_bPLC_PowFlag)
						{
							strPow.Format(_T("Module Reset1_PLC(SSB1状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
#ifdef FW_MULTI	

 							if (m_iTimeInterval>0)
 							{
								csData.Format(_T("4-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(m_iTimeInterval);

								csData.Format(_T("4-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Open_PLC_Mode_Cmd();
 							}		

							//如果第一次没响应，主动第二次复位
							m_i150ResetTime1++;
							if(m_i150ResetTime1 ==2)
							{
								csData.Format(_T("5-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(2000);

								csData.Format(_T("5-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Open_PLC_Mode_Cmd();
							}
#endif
							strPow.Format(_T("Module Reset1_PLC(SSB1状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
						}
						else
						{
							m_i150ResetTime1++;
							if(m_i150ResetTime1 ==2)
							{
								this->Show_Status(_T("请手动给模块复位(3)"));
							    this->Show_Out(_T("PLC NOT SUPPORT(SSB DL)"));
							}
						}
					
					}
					continue;
				}
				//Waiting for UE to reboot to establish communication...
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Waiting for UE to reboot"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Waiting for UE to reboot"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));

					//this->Show_Status(strInfoDis);
					this->Show_Progress(0.1);

					if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
					{
						strPow.Format(_T("Module Reset1(SSB2状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);
						this->Show_Out(strPow);
#ifdef FW_MULTI					
						if (m_iTimeInterval>0)
						{
							//向总控PC发送DC重启指令
							csData.Format(_T("6-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(m_iTimeInterval);

							csData.Format(_T("6-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);
							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}
						
						//如果第一次没响应，主动第二次复位
						m_i150ResetTime2++;
						if(m_i150ResetTime2 ==2)
						{
							csData.Format(_T("7-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
							Sleep(2000);

							csData.Format(_T("7-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
							this->Show_Out(csData);

							bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
						}
						
#endif
						strPow.Format(_T("Module Reset1(SSB2状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
						this->Show_Status(strPow);

						strPow.Format(_T("%s"),strPow);
						this->Show_Out(strPow);
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
					{

						if (m_iTimeInterval>0)//暂且用此方式 限制重启 20200529
						{
							csData.Format(_T("5-SPC重新上电-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
						//如果第一次没响应，主动第二次复位
						m_i150ResetTime2++;
						if(m_i150ResetTime2 ==2)
						{
							csData.Format(_T("5-SPC重新上电-2-CH%d"),this->m_iClass_Idx);

							this->Show_Out(csData);

							this->Set_PowerOn_SPC(this->m_iClass_Idx);
						}
					}
					else
					{
						//PLC模式暂不重新上下电
						if(true == this->m_bPLC_PowFlag)
						{
							strPow.Format(_T("Module Reset1_PLC2(SSB2状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
#ifdef FW_MULTI	
 							if (m_iTimeInterval>0)
 							{
 								//向总控PC发送DC重启指令
								csData.Format(_T("6-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Close_PLC_Mode_Cmd();
								Sleep(m_iTimeInterval);

								csData.Format(_T("6-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);
								this->Send_Open_PLC_Mode_Cmd();
 							}					
// 							
							//如果第一次没响应，主动第二次复位
							m_i150ResetTime2++;
							if(m_i150ResetTime2 ==2)
							{
								csData.Format(_T("7-CH%d DC OFF"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Close_PLC_Mode_Cmd();
								Sleep(2000);

								csData.Format(_T("7-CH%d DC ON"),m_iClass_Idx);
								this->Show_Out(csData);

								this->Send_Open_PLC_Mode_Cmd();
							}
#endif
							strPow.Format(_T("Module Reset1_PLC2(SSB2状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
							this->Show_Status(strPow);
							this->Show_Out(strPow);
						}
						else
						{

							//如果第一次没响应，主动第二次复位
							m_i150ResetTime2++;
							if(m_i150ResetTime2 ==2)
							{
								this->Show_Out(_T("PLC NOT SUPPORT(SSB DL)(4)"));
								this->Show_Status(_T("请手动给模块复位"));
							}

						}
					}
					continue;

				}
				//try resetting again
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("try resetting again"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("try resetting again"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Status(strInfoDis);
					this->Show_Progress(0.1);

					this->Show_Out(_T("try resetting again"));

					strPow.Format(_T("Module Reset1(SSB3状态:%d:通道:%d-TS:%d-开始)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
					this->Show_Status(strPow);
					this->Show_Out(strPow);

					//20190716
					if (LOAD_AUTO_ROBOT ==  m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
					{
#ifdef FW_MULTI							
						csData.Format(_T("8-TS%d-CH%d DC OFF"),m_Ts_Num,m_iClass_Idx);
						this->Show_Out(csData);

						bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("OFF") );
						Sleep(2000);

						csData.Format(_T("8-TS%d-CH%d DC ON"),m_Ts_Num,m_iClass_Idx);
						this->Show_Out(csData);
						bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("ON") );
#endif
					}
					else  if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
					{
						csData.Format(_T("8-SPC重新上电-CH%d"),this->m_iClass_Idx);

						this->Show_Out(csData);

						this->Set_PowerOn_SPC(this->m_iClass_Idx);
					}
					else
					{
						//PLC模式暂不重新上下电
						if(true == this->m_bPLC_PowFlag)
						{
							csData.Format(_T("8-CH%d DC OFF"),m_iClass_Idx);
							this->Show_Out(csData);

							this->Send_Close_PLC_Mode_Cmd();
							Sleep(2000);

							csData.Format(_T("8-CH%d DC ON"),m_iClass_Idx);
							this->Show_Out(csData);
							this->Send_Open_PLC_Mode_Cmd();
						}
						else
						{
							this->Show_Status(_T("请手动给模块复位(4)"));
							this->Show_Out(_T("PLC NOT SUPPORT(SSB DL)"));
						}
					}
					strPow.Format(_T("Module Reset1_PLC(SSB3状态:%d:通道:%d-TS:%d-完成)"),m_iOP_Robot,m_iClass_Idx,m_Ts_Num);
					this->Show_Status(strPow);
					this->Show_Out(strPow);

					continue;

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Initial bootInfo"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Initial bootInfo"));
					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					this->Show_Progress(0.2);

					this->Show_Out(_T("Initial bootInfo"));

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Programming image using FSB"));
				//大约30个
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Programming image using FSB..."));

					wcpHead+= strInfoDis.GetLength();
					csTotal_Info.Format(_T("%s"), (wcpHead + 1));
					iPsca_num++;
					if (iPsca_num<10)
					{
						pos=(iPsca_num*100)/10;
					} 
					else
					{
						pos=100;
					}
					dAdd_Percent =(double)pos/100;

					strPos.Format(_T("Programming image using FSB:%d%%......"),pos);

					this->Show_Status(strPos);
					this->Show_Progress(dAdd_Percent);

				}
				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Sorry"));
				if (NULL != wcpHead)
				{
					if (NULL == wcsstr(csTotal_Info.GetBuffer(), _T("first-stage")))
					{
						strInfoDis.Format(_T("Sorry"));
						bDownloading_Flag =true;//循环结束标志

						this->Show_Status(_T("SSB.BIN Download Fail"));
						this->Show_Progress(1.0);
						this->Show_Out(_T("SSB.BIN Download Fail"));

						this->m_csError_Code.Format(_T("SSB.BIN Download Fail!"));
						this->m_iError_Code = ERR_SSB_DL_FAIL;
						throw(1);
					} 
					//Sorry - something went wrong - The UE is not running the first-stage bootloader
					//标识模块中已经存在SSB，无需再进行下载 20190625
					else
					{
						strInfoDis.Format(_T("Finished"));
						bDownloading_Flag =true;//循环结束标志

						this->Show_Status(_T("SSB.BIN Download Finished"));
						this->Show_Out(_T("SSB.BIN Download Finished"));

						this->Show_Progress(1.0);
						continue;
					}

				}	

				wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("Finished"));
				if (NULL != wcpHead)
				{
					strInfoDis.Format(_T("Finished"));
					bDownloading_Flag =true;//循环结束标志
					this->Show_Status(_T("SSB.BIN Download Finished"));
					this->Show_Out(_T("SSB.BIN Download Finished"));

					this->Show_Progress(1.0);
					continue;
				}			
			}
		}
	}
	catch(...)
	{
		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);

		return RTN_FAIL;
	}
	//释放句柄的指针
	if(hRead_Pipe != NULL)
	{
		CloseHandle(hRead_Pipe);
		hRead_Pipe = NULL;
	}
	if(hWrite_Pipe != NULL)
	{
		CloseHandle(hWrite_Pipe);
		hWrite_Pipe = NULL;
	}

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	return RTN_SUCCESS;
}

bool CARM_OP_HUAWEI::NB_Port_Open(int iBaudRate)
{
	time_t start, finish;
	int iTimeOut =6;//打开端口改为循环查找打开的方式

	this->m_cla_Com_Port.m_iBaudRate = iBaudRate;
	this->m_cla_Com_Port.m_iPort = m_iCom_Port;
	this->m_cla_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
	this->m_cla_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;


	//大华定制
	if (FALSE == this->m_bOutLine)
	{
		if (0 == wcscmp(this->m_csOC,_T("BC35GJB-02-DAH")))
		{
			this->m_cla_Com_Port.CommDcb.StopBits = TWOSTOPBITS;
		}
		
	}	

	

	try
	{
		//
		time( &start );
		do
		{
			Sleep( 200 );
			if(this->m_cla_Com_Port.Port_Open(300))
			{
				return true;
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				throw(0);
			}
		}
		while( true );
	}
	catch(...)
	{
		return false;
	}
}
void CARM_OP_HUAWEI::NB_Port_Close()
{
	this->m_cla_Com_Port.Port_Close();
}

bool CARM_OP_HUAWEI::OnReadAtImei(void)
{
	// TODO: Add your control notification handler code here
	CString csCOM,strATCom,csData,csTemp;
	CString info;

	mAT_IMEI.Format(_T(""));

	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;
	int iError = 0;

	//
	try
	{
		//清空 COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- 检查IMEI------------------------- 
		iTimeOut = 6;
		csAT_Cmd.Format( _T( "AT+CGSN=1" ) );

		//
		time( &start );
		//
		do
		{
			//发送
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//读取
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//+CGSN:86XXXXXXXXX	OK
			//
			if(wcsstr(wcrData, _T("OK")))
			{
				//提取有用数据
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &mAT_IMEI) == true)
				{
					csData.Format(_T("AT_IMEI:1=%s"),mAT_IMEI);
					this->Show_Out(csData);

					mAT_IMEI.MakeUpper();
					mAT_IMEI.Remove(_T(' '));
					mAT_IMEI.Replace(_T("+CGSN:"),_T(""));

					csData.Format(_T("AT_IMEI:2=%s"),mAT_IMEI);
					this->Show_Out(csData);

					if (false == mAT_IMEI.IsEmpty())
					{
						return true;//pass, 退出while
					} 
					
				}				
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				throw(0);
			}
		}
		while( true );
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return false;
	}
	return false;
	
}
void CARM_OP_HUAWEI::OnWriteAtImei(void)
{
	// TODO: Add your control notification handler code here
	CString csCOM,strATCom,csData;
	CString strImei=_T("");

	this->m_cla_Com_Port.m_iBaudRate = 9600;
	this->m_cla_Com_Port.m_iPort = m_iCom_Port;

	if(!this->m_cla_Com_Port.Port_Open(400))
	{
		this->Show_Status(_T("Port is using"));
		return;
	}
	try
	{ 
		//发送
		//AT+NTSETID=1,mAT_IMEI
		this->Show_Status(_T("Write IMEI Start..."));
		strImei.Format(_T("AT+QUSETID=1,%s\r\n"),mAT_IMEI);
		if(this->m_cla_Com_Port.Query_Tgt_CHAR(strImei, _T("OK"), 3) == false)
		{
			strImei.Format(_T("AT+NTSETID=1,%s\r\n"),mAT_IMEI);

			if(this->m_cla_Com_Port.Query_Tgt_CHAR(strImei, _T("OK"), 3) == false)
			{
				throw(0);
			}	
		}
			
		//提取AT返回数据
		strImei.Format(_T("%s Write Sucess"),mAT_IMEI);
		this->Show_Status(strImei);

		this->m_cla_Com_Port.Port_Close();

	}
	catch(...)
	{	
		strImei.Format(_T("%s Write Fail"),mAT_IMEI);
		this->Show_Status(strImei);
		mAT_IMEI.Format(_T(""));
		this->m_cla_Com_Port.Port_Close();
		return ;
	}

}

//查询是否可以查询到版本号
int CARM_OP_HUAWEI::FindVer(int index)
{
	// TODO: Add your control notification handler code here
	CString strATCom,csData;
	CString csCOM,csTemp;
	CString info;

	time_t start, finish;
	CString csAT_Cmd,strAT_FirewareName;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;
	int iError = 0;

	//
	try
	{
		//清空 COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- 检查FW------------------------- 
		iTimeOut = 15;
		csAT_Cmd.Format( _T( "ATI" ) );

		//
		time( &start );
		//
		do
		{
			//发送
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//读取
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
			{	
				//提取有用数据
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &mAT_FirewareName) == true)
				{
					mAT_FirewareName.MakeUpper();
					if(wcsstr(mAT_FirewareName, _T("REVISION")))
					{						
						mAT_FirewareName.Remove(_T(' '));
						mAT_FirewareName.Replace(_T("REVISION:"),_T(""));
						break;//pass, 退出while
					}
				}			

			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				throw(0);
			}
		}
		while( true );
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return 0;
	}
	return 1;

}


bool CARM_OP_HUAWEI::Do_Execute_Erase()
{
	CString strDis;
	bool bEraseFlag = false;

	CString csCOM,strATCom,csData;

	this->m_cla_Com_Port.m_iBaudRate = 9600;
	this->m_cla_Com_Port.m_iPort = m_iCom_Port;

	if(this->m_cla_Com_Port.Port_Open(400))
	{
		m_cla_Com_Port.Port_Close();
	}
	else
	{
		this->Show_Status(_T("Port is using"));
		return false;
	}


	this->Show_Status(_T("Erase Start......"));
	this->Show_Out(_T("Erase Start......"));

	bEraseFlag = ERASE_FLS_FILE_IN_EXE(m_iClass_Idx);

	if (true == bEraseFlag)
	{
		strDis.Format(_T("Erase Pass..."));
	} 
	else
	{
		strDis.Format(_T("Erase Fail..."));
	}
	this->Show_Out(strDis);
	this->Show_Status(strDis);

	return bEraseFlag;

}

bool CARM_OP_HUAWEI::OnWriteAtImei_Auto(void)
{
	// TODO: Add your control notification handler code here
	CString csData;
	CString strImei=_T("");

	try
	{ 
		//发送
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();
		//AT+QUSETID=1,mBar_IMEI
		this->Show_Status(_T("Write IMEI Start..."));

		strImei.Format(_T("AT+QUSETID=1,%s\r\n"),mBar_IMEI);

		if(this->m_cla_Com_Port.Query_Tgt_CHAR(strImei, _T("OK"), 5) == false)
		{
			strImei.Format(_T("AT+NTSETID=1,%s\r\n"),mBar_IMEI);

			if(this->m_cla_Com_Port.Query_Tgt_CHAR(strImei, _T("OK"), 5) == false)
			{
				throw(0);
			}	
		}

		//提取AT返回数据
		strImei.Format(_T("%s Write Sucess"),mBar_IMEI);
		this->Show_Status(strImei);
	}
	catch(...)
	{	
		strImei.Format(_T("%s Write Fail"),mBar_IMEI);
		this->Show_Status(strImei);
		mAT_IMEI.Format(_T(""));
		return false;
	}
	return true;

}

bool CARM_OP_HUAWEI::OnWriteAtSN_Auto(void)
{
	// TODO: Add your control notification handler code here
	CString strSN=_T("");
	try
	{ 
		//发送
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();
		//AT+QUSETID=1,mBar_IMEI
		this->Show_Status(_T("Write SN Start..."));

		strSN.Format(_T("AT+QUSETID=0,%s\r\n"),mBar_SN);

		if(this->m_cla_Com_Port.Query_Tgt_CHAR(strSN, _T("OK"), 5) == false)
		{
			throw(0);
		}

		//提取AT返回数据
		strSN.Format(_T("%s Write Sucess"),mBar_SN);
		this->Show_Status(strSN);

	}
	catch(...)
	{	
		strSN.Format(_T("%s Write Fail"),mBar_SN);
		this->Show_Status(strSN);
		return false;
	}
	return true;
}
bool CARM_OP_HUAWEI::OnReadAtSN_Auto(void)
{
	// TODO: Add your control notification handler code here
	CString strSN=_T("");
	CString csCOM,strATCom,csData,csTemp;
	CString info;

	mAT_SN.Format(_T(""));

	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;
	int iError = 0;

	//
	try
	{
		//清空 COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- 检查IMEI------------------------- 
		iTimeOut = 6;
		csAT_Cmd.Format( _T( "AT+CGSN=0" ) );

		//
		time( &start );
		//
		do
		{
			//发送
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//读取
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
			{
				//提取有用数据
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &mAT_SN) == true)
				{
					csData.Format(_T("AT_SN:1=%s"),mAT_SN);
					this->Show_Out(csData);

					mAT_SN.MakeUpper();
					mAT_SN.Remove(_T(' '));
					mAT_SN.Replace(_T("+CGSN:"),_T(""));

					csData.Format(_T("AT_SN:2=%s"),mAT_SN);
					this->Show_Out(csData);

					return true;//pass, 退出while
				}				
		
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				throw(0);
			}
		}
		while( true );
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return false;
	}
	return true;
}


//写号，SN
bool CARM_OP_HUAWEI::WRITE_IMEI_SN()
{
    bool  bStatusFlag =false,bGetAtImei=false;
	CString csData;

	bool  bImeiCheck =false,bSnCheck=false;

	this->Show_Out(_T("Check and Write Start......"));
	//增加检测
	//*******************************************************
	if(mBar_IMEI.GetLength() != 15)
	{
		csData.Format(_T("FAIL,写号IMEI长度!=15(%s)!"),mBar_IMEI);

		this->Show_Status(csData);		
		this->Show_Out(csData);	

		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_IMEI_LEN;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));
		return false;
	}
   //打开口
	//*******************************************************
	bStatusFlag = NB_Port_Open(9600);
	if (false == bStatusFlag)
	{
		csData.Format(_T("端口打开失败!"));

		this->Show_Status(csData);	
		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_OPEN_PORT_FAIL;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));
		return false;
	}

	//查询版本
	//*******************************************************
	Sleep(500);
	bStatusFlag = FindVerAuto();
	if (false == bStatusFlag)
	{
		csData.Format(_T("Fail,未查询到模块版本号!"));

		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_FIND_FW_VERSION;

		this->Show_Status(csData);	
		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));
		//20181024
		NB_Port_Close();

		return false;
	}
	else
	{
		csData.Format(_T("查询模块版本(%s)"),mAT_FirewareName);

		this->Show_Status(csData);
		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}


	//检查版本与DB上匹配
	//*******************************************************
	//Sleep(500);
	mDB_FirewareName.MakeUpper();
	if(wcscmp(mDB_FirewareName,mAT_FirewareName)!= 0)
	{
		//不匹配
		csData.Format(_T("Fail,查询的版本和数据库不匹配!"));

		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_FW_VER_NOT_MATCH_DB;

		this->Show_Status(csData);	
		csData.Format(_T("%s-AT(%s)DB(%s)"),csData,mAT_FirewareName,mDB_FirewareName);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));

		//20181024
		NB_Port_Close();

		return false;
	}
	else
	{
		csData.Format(_T("模块版本和数据库匹配"));

		this->Show_Status(csData);
	
		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}

	//*******************************************************

	//增加先查询IMEI
	//*******************************************************
	//Sleep(500);
	bImeiCheck = OnReadAtImei();

	//*******************************************************

	//增加先查询SN
	//*******************************************************
	bSnCheck = OnReadAtSN_Auto();

	if((true == bImeiCheck) && (true == bSnCheck))
	{
		//20190822 增加 AT IMEI和 bar IMEI比较
		if (0 != wcscmp(mAT_IMEI,mBar_IMEI))
		{
			csData.Format(_T("FAIL,查询模块IMEI(%s)+BAR IMEI不匹配"),mAT_IMEI);
			this->Show_Status(csData);
			this->Show_Out(csData);
			//20181024
			NB_Port_Close();
			return false;
		}
		   

		csData.Format(_T("PASS,查询模块IMEI+SN(1)(%s)"),mBar_IMEI);

		this->Show_Status(csData);
		bGetAtImei=true;
		//20181024
		NB_Port_Close();

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));
		return  true;
	}
	if (false == bImeiCheck)
	{
		csData.Format(_T("未查询到模块IMEI(1)(%s)!"),mBar_IMEI);

		this->Show_Status(csData);	

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}

	if (false == bSnCheck)
	{
		csData.Format(_T("未查询到模块SN(1)(%s)!"),mBar_SN);

		this->Show_Status(csData);	
		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}


	//如没查询到IMEI，则认为需写IMEI
	//*******************************************************
	//Sleep(500);
	if (false == bImeiCheck)
	{
		bStatusFlag = OnWriteAtImei_Auto();
		if (false == bStatusFlag)
		{
			csData.Format(_T("FAIL,写IMEI(%s)!"),mBar_IMEI);

			this->Show_Status(csData);		
			//错误信息
			this->m_csError_Code.Format(_T("%s"),csData);
			this->m_iError_Code= ERR_W_IMEI;

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
			this->Show_Out(_T("Check and Write End......"));
			//20181024
			NB_Port_Close();
			return false;
		}
		else
		{
			csData.Format(_T("PASS,写IMEI(%s)"),mBar_IMEI);

			this->Show_Status(csData);

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
		}
	}
	//*******************************************************
	//写SN	
	//Sleep(500);
	if (false == bSnCheck)
	{
		bStatusFlag = OnWriteAtSN_Auto();
		if (false == bStatusFlag)
		{
			csData.Format(_T("FAIL,写SN(%s)"),mBar_SN);

			this->Show_Status(csData);

			//错误信息
			this->m_csError_Code.Format(_T("%s"),csData);
			this->m_iError_Code= ERR_W_SN;

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
			this->Show_Out(_T("Check and Write End......"));

			//20181024
			NB_Port_Close();

			return false;
		}
		else
		{
			csData.Format(_T("PASS,写SN(%s)"),mBar_SN);

			this->Show_Status(csData);

			csData.Format(_T("%s"),csData);
			this->Show_Out(csData);
		}
	}

	//再查询IMEI
	//*******************************************************
	//Sleep(500);
	bStatusFlag = OnReadAtImei();
	if (false == bStatusFlag)
	{
		csData.Format(_T("FAIL,查询模块IMEI(%s)(2)!"),mBar_IMEI);

		this->Show_Status(csData);

		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_FIND_IMEI_AT;

		bGetAtImei=false;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));

		//20181024
		NB_Port_Close();

		return false;
	}
	else
	{
		csData.Format(_T("PASS,查询模块IMEI(%s)(2)"),mBar_IMEI);

		this->Show_Status(csData);

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);

		bGetAtImei=true;
	}






	//*******************************************************
	//AT的IMEI和总控提供的mBar_IMEI匹配
	if (NULL != wcsstr(mAT_IMEI,mBar_IMEI))
	{
		csData.Format(_T("PASS,IMEI_BarCode+AT(%s-%s)!"),mBar_IMEI,mAT_IMEI);

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	} 
	else
	{
		csData.Format(_T("FAIL,IMEI_BarCode+AT(%s-%s)!"),mBar_IMEI,mAT_IMEI);

		this->Show_Status(csData);

		::AfxMessageBox(csData);

		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_IMEI_AT_NOT_MATCH_DB;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));

		//20181024
		NB_Port_Close();

		return false;
	}


	//*******************************************************
	//读SN
	bStatusFlag = OnReadAtSN_Auto();
	if (false == bStatusFlag)
	{
		csData.Format(_T("FAIL,读SN(%s)"),mBar_SN);

		this->Show_Status(csData);

		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code= ERR_FIND_SN_AT;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		this->Show_Out(_T("Check and Write End......"));

		//20181024
		NB_Port_Close();

		return false;
	}
	else
	{
		csData.Format(_T("PASS,读SN(%s)"),mBar_SN);

		this->Show_Status(csData);

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
	}
	
	mBar_IMEI.Format(_T(""));
	mBar_SN.Format(_T(""));
	this->Show_Out(_T("Check and Write End......"));

	//20181024
	NB_Port_Close();
	return true;
}
//查询是否可以查询到版本号
bool CARM_OP_HUAWEI::FindVerAuto(void)
{
	// TODO: Add your control notification handler code here
	CString csCOM,strATCom,csData,csTemp;
	CString info;

	mAT_FirewareName.Format(_T(""));

	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;
	int iError = 0;

	//
	try
	{
		//清空 COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- 检查IMEI------------------------- 
		iTimeOut = 8;
		csAT_Cmd.Format( _T( "ATI" ) );

		//
		time( &start );
		//
		do
		{
			//清空 COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

			//发送
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//读取
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
			{
				//提取有用数据
				if (this->m_cla_GFC.GET_AT_RSP_STRING(wcrData, _T("OK"), -1, &mAT_FirewareName) == true)
				{
					mAT_FirewareName.MakeUpper();
					if(wcsstr(mAT_FirewareName, _T("REVISION")))
					{						
						mAT_FirewareName.Remove(_T(' '));
						mAT_FirewareName.Replace(_T("REVISION:"),_T(""));
						break;//pass, 退出while
					}
				}				
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				throw(0);
			}
		}
		while( true );
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return false;
	}
	return true;

}

//打开GNSS使能 BC39
// 主串口发送 AT+QGNSSC=1  Power on GNSS 
bool CARM_OP_HUAWEI::Set_QGNSSC_PowerOn(void)
{
	// TODO: Add your control notification handler code here
	CString csData;
	CString info;
	//打开口
	//*******************************************************
	bool bStatusFlag = false;
	bStatusFlag = NB_Port_Open(9600);
	if (false == bStatusFlag)
	{
		csData.Format(_T("Set_QGNSSC_PowerOn:Open Port Fail!"));

		this->Show_Out(csData);	
		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code=0x03000000|0x00002029;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		return false;
	}


	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;

	//
	try
	{
		//清空 COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- 检查IMEI------------------------- 
		iTimeOut = 8;
		csAT_Cmd.Format( _T( "AT+QGNSSC=1" ) );

		//
		time( &start );
		//
		do
		{
			//清空 COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

			//发送
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//读取
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
			{
				break;				
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				throw(0);
			}
		}
		while( true );
		//-------------------------
	}
	catch( ... )
	{
		//fail
		this->Show_Out(_T("Set_QGNSSC_PowerOn Fail"));
		NB_Port_Close();
		return false;
	}
	this->Show_Out(_T("Set_QGNSSC_PowerOn Pass"));
	NB_Port_Close();
	return true;

}
//关闭GNSS使能 BC39
// 主串口发送 AT+QGNSSC=0  Power OFF GNSS 
bool CARM_OP_HUAWEI::Set_QGNSSC_PowerOFF(void)
{
	CString csData;
	CString info;
	//打开口
	//*******************************************************
	bool bStatusFlag = false;
	bStatusFlag = NB_Port_Open(9600);
	if (false == bStatusFlag)
	{
		csData.Format(_T("Set_QGNSSC_PowerOFF:Open Port Fail!"));

		this->Show_Out(csData);	
		//错误信息
		this->m_csError_Code.Format(_T("%s"),csData);
		this->m_iError_Code=0x03000000|0x00002029;

		csData.Format(_T("%s"),csData);
		this->Show_Out(csData);
		return false;
	}

	time_t start, finish;
	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut, iRec_Bits;

	//
	try
	{
		//清空 COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//------------------------- 检查IMEI------------------------- 
		iTimeOut = 8;
		csAT_Cmd.Format( _T( "AT+QGNSSC=0" ) );

		//
		time( &start );
		//
		do
		{
			//清空 COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

			//发送
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
			memset( wcrData, 0, sizeof( wcrData ) );
			Sleep( 200 );
			//读取
			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
			//
			if(wcsstr(wcrData, _T("OK")))
			{
				break;				
			}
			//
			time( &finish );
			//
			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				throw(0);
			}
		}
		while( true );
		//-------------------------
	}
	catch( ... )
	{
		//fail
		this->Show_Out(_T("Set_QGNSSC_PowerOFF Fail"));
		NB_Port_Close();
		return false;
	}
	this->Show_Out(_T("Set_QGNSSC_PowerOFF Pass"));
	NB_Port_Close();
	return true;

}

//20190130 设置每个通道PLC的电源上下电
void CARM_OP_HUAWEI::SetPLC_Open_Path(void)
{
	BOOL ret = FALSE;
	CString  csData;
	this->Show_Status(_T("Wait DC ON"));
	ret = OpenPLC_Power_Path_Huawei(this->m_iClass_Idx);

	if (TRUE == ret)
	{
		csData.Format(_T("PLC_Open Sucess"));
	} 
	else
	{
		csData.Format(_T("PLC_Open Fail"));
	}
	this->Show_Out(csData);	
}
void CARM_OP_HUAWEI::SetPLC_Close_Path(void)
{
	BOOL ret = FALSE;
	CString  csData;
	//this->Show_Status(_T("Wait DC OFF"));
	ret = ClosePLC_Power_Path_Huawei(this->m_iClass_Idx);
	if (TRUE == ret)
	{
		csData.Format(_T("PLC_OFF Sucess"));
	} 
	else
	{
		csData.Format(_T("PLC_OFF Fail"));
	}
	this->Show_Out(csData);	

}

//打开其中一路PLC的电源
BOOL CARM_OP_HUAWEI::OpenPLC_Power_Path_Huawei(int Index)
{
	time_t t_Start, t_End;
	unsigned long ulIndex=0;
	int  i_TimeOut=2;
	unsigned char ucOpenCmd[4],ucrTgt_Key[4];
	CString csData;
	char cp_Read_Buff[8];
	int ulTgt_Key_Quan=0;
	unsigned long ul_Expect_Read_Bytes=5;
	unsigned long ulp_Real_Rec_Bytes=0;

	try
	{	
		g_PLC_Pow_Ctrl.Lock();

		//发送一次指令
		ucOpenCmd[0]=0xFF;
		ucOpenCmd[1]=Index;
		ucOpenCmd[2]=0x1A;
		ucOpenCmd[3]=0x00;

		//期望回值
		ucrTgt_Key[0] = 0xFF;
		ucrTgt_Key[1] = Index;
		ucrTgt_Key[2] = 0x1B;
		ucrTgt_Key[3] = 0x00;
		ulTgt_Key_Quan = 4;

		if (g_cla_Com_PLC_Power.Port_Open(500))
		{
			g_PLC_Pow_Ctrl.Unlock();
			return FALSE;
		}


		//清空接收BUFF
		 g_cla_Com_PLC_Power.Empty_Com_And_Rx_Buff();

		//循环
		time(&t_Start);
		//
		do
		{
			g_cla_Com_PLC_Power.Write_Byte((char*)ucOpenCmd,4);

			//期望值为：0xFF 01 1B 00 
			memset(&cp_Read_Buff,0,sizeof(cp_Read_Buff));
			Sleep(20);
			g_cla_Com_PLC_Power.Read_Byte(cp_Read_Buff,ul_Expect_Read_Bytes,&ulp_Real_Rec_Bytes);

			if(ulp_Real_Rec_Bytes!=0)
			{
				if(this->m_cla_GFC.mem_uc_str((unsigned char*)cp_Read_Buff, ulp_Real_Rec_Bytes, ucrTgt_Key, ulTgt_Key_Quan, &ulIndex) == true)
				{
					g_cla_Com_PLC_Power.Port_Close();

					g_PLC_Pow_Ctrl.Unlock();
					return TRUE;
				}			
			}

			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		g_cla_Com_PLC_Power.Port_Close();

		g_PLC_Pow_Ctrl.Unlock();
		return FALSE;
	}
	return TRUE;
}

//关闭其中一路PLC的电源
BOOL CARM_OP_HUAWEI::ClosePLC_Power_Path_Huawei(int Index)
{
	time_t t_Start, t_End;
	unsigned long ulIndex=0;
	int  i_TimeOut=2;
	unsigned char ucOpenCmd[4],ucrTgt_Key[4];
	CString csData;
	char cp_Read_Buff[8];
	int ulTgt_Key_Quan=0;
	unsigned long ul_Expect_Read_Bytes=5;
	unsigned long ulp_Real_Rec_Bytes=0;

	try
	{
		g_PLC_Pow_Ctrl.Lock();

		if (g_cla_Com_PLC_Power.Port_Open(500))
		{
			g_PLC_Pow_Ctrl.Unlock();
			return FALSE;
		}

		//发送一次指令
		ucOpenCmd[0]=0xFF;
		ucOpenCmd[1]=Index;
		ucOpenCmd[2]=0x1C;
		ucOpenCmd[3]=0x00;

		//期望回值
		ucrTgt_Key[0] = 0xFF;
		ucrTgt_Key[1] = Index;
		ucrTgt_Key[2] = 0x1D;
		ucrTgt_Key[3] = 0x00;
		ulTgt_Key_Quan = 4;


		//清空接收BUFF
		g_cla_Com_PLC_Power.Empty_Com_And_Rx_Buff();
		//循环
		time(&t_Start);
		//
		do
		{
			g_cla_Com_PLC_Power.Write_Byte((char*)ucOpenCmd,4);

			//期望值为：0xFF 01 1B 00 
			memset(&cp_Read_Buff,0,sizeof(cp_Read_Buff));
			Sleep(20);
			m_cla_Com_PLC_Power_Huawei.Read_Byte(cp_Read_Buff,ul_Expect_Read_Bytes,&ulp_Real_Rec_Bytes);

			if(ulp_Real_Rec_Bytes!=0)
			{
				if(this->m_cla_GFC.mem_uc_str((unsigned char*)cp_Read_Buff, ulp_Real_Rec_Bytes, ucrTgt_Key, ulTgt_Key_Quan, &ulIndex) == true)
				{
					g_cla_Com_PLC_Power.Port_Close();
					g_PLC_Pow_Ctrl.Unlock();
					return TRUE;
				}			
			}

			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		g_cla_Com_PLC_Power.Port_Close();
		g_PLC_Pow_Ctrl.Unlock();
		return FALSE;
	}
}

void Multi_PLC_Pow_BC(LPVOID x)
{
	CString strData;
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;

	//关闭其中一路PLC的电源
	strData.Format(_T("2-CH%d-DC OFF"),pSv->m_iClass_Idx);
	pSv->Send_Close_PLC_Mode_Cmd();

	Sleep(500);
	//打开其中一路PLC的电源
	strData.Format(_T("2-CH%d-DC ON"),pSv->m_iClass_Idx);
	pSv->Send_Open_PLC_Mode_Cmd();

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_1(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//关闭其中一路PLC的电源
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path2"));
	Sleep(500);
	//打开其中一路PLC的电源
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path2"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_2(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//关闭其中一路PLC的电源
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path2"));
	Sleep(500);
	//打开其中一路PLC的电源
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path2"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_3(LPVOID x)
{
	g_PLC_Power.Lock();


	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//关闭其中一路PLC的电源
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path3"));
	Sleep(500);
	//打开其中一路PLC的电源
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path3"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_4(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//关闭其中一路PLC的电源
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path4"));
	Sleep(500);
	//打开其中一路PLC的电源
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path4"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_5(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//关闭其中一路PLC的电源
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path5"));
	Sleep(500);
	//打开其中一路PLC的电源
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path5"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_6(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//关闭其中一路PLC的电源
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path6"));
	Sleep(500);
	//打开其中一路PLC的电源
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path6"));

	g_PLC_Power.Unlock();
}

void Multi_PLC_Pow_BC_7(LPVOID x)
{
	g_PLC_Power.Lock();

	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;
	//关闭其中一路PLC的电源
	pSv->Send_Close_PLC_Mode_Cmd();
	pSv->Show_Out(_T("ClosePLC_Power_Path7"));
	Sleep(500);
	//打开其中一路PLC的电源
	pSv->Send_Open_PLC_Mode_Cmd();
	pSv->Show_Out(_T("OpenPLC_Power_Path7"));

	g_PLC_Power.Unlock();
}



void Multi_Auto_PLC_Pow_BC_1(LPVOID x)
{
	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;

	//关闭其中一路PLC的电源
	CString strTsInfo;
	strTsInfo.Format(_T("2-TS%d-CH%d DC OFF"),pSv->m_Ts_Num,pSv->m_iClass_Idx);
	::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( pSv->m_Ts_Num, _T("OFF") );
	pSv->Show_Out(strTsInfo);

	Sleep(2000);
	//打开其中一路PLC的电源
	strTsInfo.Format(_T("2-TS%d-CH%d DC ON"),pSv->m_Ts_Num,pSv->m_iClass_Idx);
	::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( pSv->m_Ts_Num, _T("ON") );
	pSv->Show_Out(strTsInfo);
}

void Multi_Auto_SPC_Pow_BC_1(LPVOID x)
{
	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;

	pSv->Set_PowerOn_SPC(pSv->m_iClass_Idx);

	
}




bool  CARM_OP_HUAWEI::CheckSSB(void)
{
	bool  bDC_Reset =false;

	unsigned char* cpData =NULL;
	wchar_t * wcpData = NULL;
	time_t start, finish;

	unsigned char cData[2048];

	unsigned long BUFF_LEN=0;

	CString csData,strInfo;

	int iTimeOut=8;//最大延时8秒

	try
	{
		//读一段数据检查
		m_cla_Com_Port.Empty_Com_And_Rx_Buff();
		//关闭下DC
		//向总控PC发送DC重启指令
		if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
		{		
			this->m_hThread_Auto_PLC_Pow_Handle_BC = CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)Multi_Auto_PLC_Pow_BC_1,
				(void *)this,
				0,
				&this->m_dwThread_Auto_PLC_Pow_BC_ID);
		}
		else if (LOAD_AUTO_ROBOT_SPC ==  m_iOP_Robot) //全自动SPC
		{
			this->m_hThread_Auto_SPC_Pow_Handle_BC = CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)Multi_Auto_SPC_Pow_BC_1,
				(void *)this,
				0,
				&this->m_dwThread_Auto_SPC_Pow_BC_ID);
		}
		else
		{
			if(true == this->m_bPLC_PowFlag)
			{
				this->m_hThread_PLC_Pow_Handle_BC = CreateThread(NULL,
					0,
					(LPTHREAD_START_ROUTINE)Multi_PLC_Pow_BC,
					(void *)this,
					0,
					&this->m_dwThread_PLC_Pow_BC_ID);
			}
			else
			{
				this->Show_Status(_T("需要手动复位检查SSB..."));
				Sleep(5000);
			}


		}

		//分配内存		
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 1024+128);
		time( &start );
		while (1)
		{
			m_cla_Com_Port.GET_BUFF_BYTE(&cpData,&BUFF_LEN);

			if ((BUFF_LEN>0)&&(BUFF_LEN<2048))
			{
				//字符转换
				memset(wcpData, 0, 1024+128);
				memset(cData, 0, 2048);
				memcpy(cData,cpData,BUFF_LEN);
				for (int i=0;i<(int)BUFF_LEN;i++)
				{
					if (cData[i]==0)
					{
						cData[i]='_';
					}
				}

				int iTemp=0;
				this->m_cla_GFC.MultiByte_To_WideChar((const char *)cData, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);
				csData.MakeUpper();
				if (wcsstr(csData.GetBuffer(), _T("SSB")))//检查有没有SSB字段信息
				{
					this->Show_Status(_T("check SSB OK..."));

					strInfo.Format(_T("CH%d-check SSB OK"),m_iClass_Idx);
					this->Show_Out(strInfo);
					
					break;
				} 
				else
				{						
					//...

				}
			}
			time( &finish );

			if( (int)difftime( finish, start ) >= iTimeOut )
			{
				this->Show_Status(_T("check SSB Fail"));
				this->Show_Out(_T("check SSB Fail"));
				throw(1);
				break;
			}

		}
		
	}
	catch(...)
	{
		//
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		return false;

	}

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);

	return true;


}


bool CARM_OP_HUAWEI::WRITE_BYTE(char * cp_Send_Data, unsigned long ul_Data_Quan)
{
	return g_cla_Com_PLC_Power.Write_Byte(cp_Send_Data, ul_Data_Quan);
}

void CARM_OP_HUAWEI::Clear_Com_Rx_Buffer(void)
{
	g_cla_Com_PLC_Power.Empty_Com_And_Rx_Buff();
}
bool CARM_OP_HUAWEI::READ_PKT_RSP_V2(RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut)
{
	return this->READ_PKT_RSP_SYNC_V2(str_Rsp_Info_V2, i_TimeOut);
}
bool CARM_OP_HUAWEI::READ_PKT_RSP_SYNC_V2(RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut)
{
	//用“核对法”，检查是否有正确的RSP
	//用于数据包返回值是固定数据的情况
	time_t t_Start, t_End;
	int i, j;
	int iRsp_Pkt_Numb = 0;//读到的实际PKT包的数量统计
	int iData_Index = 0;//检查串口BUFF数据的当前Index值，（已核对无误的PKT包数据就不用再核对了）
	unsigned char * ucpRx_Buff = NULL;
	unsigned long ulRx_Quan = 0;

	try
	{
		//计时开始
		time(&t_Start);

		//总循环，时间超时控制其退出
		do
		{
			//暂停
			if(this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//超时检测
			time(&t_End);
			if(difftime(t_End, t_Start) >= i_TimeOut)
			{
				if(g_cla_Com_PLC_Power.m_str_Com_Rx_Buff.ul_Data_Length <= 0)
				{
					this->m_csError_Code.Format(_T("FAIL, Rsp reply nothing!"));
				}
				else
				{
					this->m_csError_Code.Format(_T("FAIL, Rsp does not reply right data!"));
				}

				throw(0);
			}


			//检查一次接收BUFF，有数据后读，并返回BUFF的地址和数据量
			this->READ_BYTE(&ucpRx_Buff, &ulRx_Quan);


			//-------------------------------------- 有新数据，便判定是否要核对“期望值” -------------------------------------- 
			//根据COM_BUFF中有效数据的情况，判断是否要进行解析操作
			if(ulRx_Quan <= 0)
			{
				Sleep(1);//1毫秒足够了，每次检查串口内可读数据的操作所需时间不到1毫秒
				continue;
			}
			else if(ulRx_Quan < str_Rsp_Info_V2.ui_Rsp_Data_Length)//没有达到至少的数量，等待
			{
				Sleep(20);//这个时间很重要，影响程序的稳定
				continue;
			}

			//当前状态已有足够数据量的响应
			//已当前核对到的串口Index值作为起始值
			for(i=iData_Index; i<ulRx_Quan; i++)
			{
				//发现串口BUFF有个字符与“期望响应”的首字符一致
				if(*(ucpRx_Buff + i) == str_Rsp_Info_V2.ucr_Expect_Rsp[0])
				{
					//开始按“期望响应”的数据进行全面核对
					for(j=0; j<str_Rsp_Info_V2.ui_Rsp_Data_Length; j++)
					{
						//检查串口BUFF的Index号是否超越了串口BUFF中的数据
						if((i + j) < ulRx_Quan)
						{
							if(*(ucpRx_Buff + i + j) != str_Rsp_Info_V2.ucr_Expect_Rsp[j])
							{
								break;//发现任意数据不同，则退出，没找到期望数据
							}
						}
						else
						{
							break;//有用数据结束了，也退出循环
						}
					}//for(j=0; j<str_Rsp_Info_V2.ui_Rsp_Data_Length;

					//若期望值核对到了最后一位，便是找到一个匹配的响应
					if(j == str_Rsp_Info_V2.ui_Rsp_Data_Length)
					{
						//统计，找到了一个期望数据包
						iRsp_Pkt_Numb++;
						//在这个包之前的串口BUFF数据，算已检查过，后续便不再检查
						iData_Index = i + str_Rsp_Info_V2.ui_Rsp_Data_Length;
						//按iData_Index作为下次检查的起点，由于循环会“+1”，所以赋值是“-1”
						i = iData_Index - 1;
					}
					else if(j > str_Rsp_Info_V2.ui_Rsp_Data_Length)
					{
						this->m_csError_Code.Format(_T("ERROR, READ_PKT_RSP_SYNC_V2, j = %d, Rsp_Data_Length = %d;"), j, str_Rsp_Info_V2.ui_Rsp_Data_Length);
						::AfxMessageBox(this->m_csError_Code);
						throw(0);
					}
				}//发现串口BUFF有个字符与“期望响应”的首字符一致
			}//for(i=iData_Index; i<ulRx_Quan; i++

			//检查串口BUFF中的数据结束后
			//检查响应包的数量对不对
			if(iRsp_Pkt_Numb == str_Rsp_Info_V2.ui_Rsp_Pkt_Quan)
			{
				//期望数据包的数量正确，跳出while
				break;//pass
			}
		}
		while(true);
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool CARM_OP_HUAWEI::READ_BYTE(unsigned char ** ucp2_Buff_Pointer, unsigned long * ulp_Data_Length)
{
	//读串口，返回串口实际接收到数据的BUFF的地址和BUFF中的byte数据量
	bool bRtn = false;

	bRtn = g_cla_Com_PLC_Power.Read_Byte_Sync_V1();
	*ucp2_Buff_Pointer = g_cla_Com_PLC_Power.m_str_Com_Rx_Buff.ucp_Buff;
	*ulp_Data_Length = g_cla_Com_PLC_Power.m_str_Com_Rx_Buff.ul_Data_Length;

	return bRtn;
}
bool CARM_OP_HUAWEI::Send_Open_PLC_Mode_Cmd(void)
{
	PKT_BUFF_V2_S_Ext str_Pkt_Cmd;
	RSP_INFO_V2_S_Ext str_Rsp_Info;
	CString strData;

	strData.Format(_T("Send Open PLC Port cmd(Port%d) ....."),g_cla_Com_PLC_Power.m_iPort);
	this->Show_Status(strData);

	g_PLC_Pow_Ctrl.Lock();

	try
	{	
		//打包命令（FF 01 1A 00）
		str_Pkt_Cmd.ucr_Buff[0] = 0xFF;
		str_Pkt_Cmd.ucr_Buff[1] = this->m_iClass_Idx;
		str_Pkt_Cmd.ucr_Buff[2] = 0x1A;
		str_Pkt_Cmd.ucr_Buff[3] = 0x00;
		str_Pkt_Cmd.ucr_Buff[4] = NULL;
		//
		str_Pkt_Cmd.ul_Data_Length = 4;

		//设定预计回复//打包命令（FF 01 1B 00）
		str_Rsp_Info.ucr_Expect_Rsp[0] = 0xFF;
		str_Rsp_Info.ucr_Expect_Rsp[1] = this->m_iClass_Idx;
		str_Rsp_Info.ucr_Expect_Rsp[2] = 0x1B;
		str_Rsp_Info.ucr_Expect_Rsp[3] = 0x00;
		str_Rsp_Info.ucr_Expect_Rsp[4] = NULL;
		//
		str_Rsp_Info.ui_Rsp_Data_Length = 4;
		str_Rsp_Info.ui_Rsp_Pkt_Quan = 1;

		//发送	
		if(this->QUERY_PKT(str_Pkt_Cmd, str_Rsp_Info, 4) == false)
		{
			throw(0);
		}
	}
	catch(...)
	{
		this->m_csError_Code += _T(" [Send Open PLC Port]");
		this->Show_Out(_T("Send Open PLC Port Fail"));

		//20190716
		Sleep(300);

		g_PLC_Pow_Ctrl.Unlock();
		return false;
	}

	//20190716
	Sleep(300);

	g_PLC_Pow_Ctrl.Unlock();

	return true;
}
bool CARM_OP_HUAWEI::Send_Close_PLC_Mode_Cmd(void)
{
	PKT_BUFF_V2_S_Ext str_Pkt_Cmd;
	RSP_INFO_V2_S_Ext str_Rsp_Info;
	CString strData;

	//strData.Format(_T("Send Close PLC Port cmd(Port%d) ....."),g_cla_Com_PLC_Power.m_iPort);
	//this->Show_Status(strData);

	g_PLC_Pow_Ctrl.Lock();

	try
	{	
		//打包命令（FF 01 1C 00）
		str_Pkt_Cmd.ucr_Buff[0] = 0xFF;
		str_Pkt_Cmd.ucr_Buff[1] = this->m_iClass_Idx;
		str_Pkt_Cmd.ucr_Buff[2] = 0x1C;
		str_Pkt_Cmd.ucr_Buff[3] = 0x00;
		str_Pkt_Cmd.ucr_Buff[4] = NULL;
		//
		str_Pkt_Cmd.ul_Data_Length = 4;

		//设定预计回复//打包命令（FF 01 1D 00）
		str_Rsp_Info.ucr_Expect_Rsp[0] = 0xFF;
		str_Rsp_Info.ucr_Expect_Rsp[1] = this->m_iClass_Idx;
		str_Rsp_Info.ucr_Expect_Rsp[2] = 0x1D;
		str_Rsp_Info.ucr_Expect_Rsp[3] = 0x00;
		str_Rsp_Info.ucr_Expect_Rsp[4] = NULL;
		//
		str_Rsp_Info.ui_Rsp_Data_Length = 4;
		str_Rsp_Info.ui_Rsp_Pkt_Quan = 1;

		//发送	
		if(this->QUERY_PKT(str_Pkt_Cmd, str_Rsp_Info, 4) == false)
		{
			throw(0);
		}
	}
	catch(...)
	{
		this->m_csError_Code += _T(" [Send Close PLC Port]");
		this->Show_Out(_T("Send Close PLC Port Fail"));

		g_PLC_Pow_Ctrl.Unlock();

		return false;
	}

	//this->Show_Out(_T("Send Close PLC Port Pass"));

	g_PLC_Pow_Ctrl.Unlock();

	return true;
}

bool CARM_OP_HUAWEI::QUERY_PKT(PKT_BUFF_V2_S_Ext str_Pkt_V2_Write, RSP_INFO_V2_S_Ext str_Rsp_Info_V2, int i_TimeOut)
{
	time_t t_Start, t_End;

	try
	{
		//清空接收BUFF
		this->Clear_Com_Rx_Buffer();
		//this->m_csError_Code.Format(_T(""));

		//循环
		time(&t_Start);
		//
		do
		{
			if(this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//发送指令
			this->WRITE_BYTE((char *)str_Pkt_V2_Write.ucr_Buff, str_Pkt_V2_Write.ul_Data_Length);

			//核对有没有期望数据
			
			if(this->READ_PKT_RSP_V2(str_Rsp_Info_V2, 3) == true)
			{
				//读到校验正确的应答
				break;//pass
			}

			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CARM_OP_HUAWEI::Get_Img_File_Index(CString cs_Img_Name, int * ip_Index)
{
	if(wcsstr(cs_Img_Name, _T("SYSTEM_2K")) != NULL)
	{
		*ip_Index = SYSTEM_IMG_2K_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BALONG_MODEM_FW_2K")) != NULL)
	{
		*ip_Index = MODEM_IMG_2K_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("SYSTEM_4K")) != NULL)
	{
		*ip_Index = SYSTEM_IMG_4K_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BALONG_MODEM_FW_4K")) != NULL)
	{
		*ip_Index = MODEM_IMG_4K_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("PTABLE")) != NULL)
	{
		*ip_Index = PTABLE_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("M3BOOT")) != NULL)
	{
		*ip_Index = M3BOOT_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BSP_MCORE")) != NULL)
	{
		*ip_Index = M3IMAGE_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("FASTBOOT")) != NULL)
	{
		*ip_Index = FASTBOOT_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BOOT")) != NULL)
	{
		*ip_Index = KERNEL_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("BALONG_MODEM")) != NULL)
	{
		*ip_Index = MODEM_IMG_FILE_I; 
	}
	else if(wcsstr(cs_Img_Name, _T("USERDATA")) != NULL)
	{
		*ip_Index = USERDATA_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("MODEM_DT")) != NULL)
	{
		*ip_Index = DTS_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("LPHY")) != NULL)
	{
		*ip_Index = DSP_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("HIFI")) != NULL)
	{
		*ip_Index = HIFI_IMG_FILE_I;
	}
	else if(wcsstr(cs_Img_Name, _T("NV")) != NULL)
	{
		*ip_Index =NVDLOAD_IMG_FILE_I ;
	}
	else
	{
		this->m_csError_Code.Format(_T("FAIL, Get %s.bin/img file index error!"), cs_Img_Name);
		return false;
	}

	return true;
}


bool CARM_OP_HUAWEI::FB_DEVICE_V1()
{
	//使用fastboot.exe探测一次是否有Adb设备

	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	unsigned long ulActual_Read;
	CString csTotal_Info;
	int iTemp;
	bool bSuccess_Flag = false;


	try
	{
		//
		csTotal_Info.Format(_T(""));


		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_DEVICE, Create pipe fail!"));
			throw(0);
		}
		//------------------------------ 


		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() + 128));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//------------------------------ 打包dos指令 ------------------------------ 
		swprintf(wcpCmd, _T("cmd.exe %cc %s\\NB-IoT\\CH%d\\fastboot.exe devices"), '/', this->m_csCurr_Exe_Path, this->m_iClass_Idx);


		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//创建子进程
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_DEVICE, Create process fail!"));
			throw(2);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 


		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		//读info（若.exe运行已结束，还继续read的话，会死在这一步）
		memset(cpRead_Buff, 0, 2048);
		ulActual_Read = 0;
		//
		ReadFile(hRead_Pipe, cpRead_Buff, 2000, &ulActual_Read, NULL);

		//读到info数据了
		if(ulActual_Read > 0)
		{
			//字符转换
			memset(wcpData, 0, 2048);
			this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

			csTotal_Info.Format(_T("%s"), wcpData);
			csTotal_Info.Replace(_T('\r'), _T(' '));
			csTotal_Info.Replace(_T('\n'), _T(' '));

			//			this->Save_Trace_Log(csTotal_Info);

			//汇总数据
			csTotal_Info.MakeLower();

			this->Show_Status(csTotal_Info);
		}//if(ulActual_Read > 0

		//检查是否pass
		if((wcsstr(csTotal_Info, _T("mdm")) != NULL) && (wcsstr(csTotal_Info, _T("fastboot")) != NULL))
		{
			bSuccess_Flag = true;//pass
		}
		//------------------------------


		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	}
	catch(...)
	{
		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return bSuccess_Flag;
}
//---------------------------------------------------------
bool CARM_OP_HUAWEI::FB_DL_IMAGE_FILE(const CString strFastBootCmd, CString  csFB_Type_Name,unsigned long csFB_size)
{
	//使用fastboot.exe下载一个文件

	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csTemp;
	int iTemp;
	bool bSuccess_Flag = false;

	if(strFastBootCmd.IsEmpty() == true)
	{
		this->m_csError_Code.Format(_T("ERROR, FB_DL_IMAGE_FILE, FastBootCmd is null!"));
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	CString strFastBootCmdTmp;
	strFastBootCmdTmp = strFastBootCmd;

	try
	{
		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_DL_FILE, Create pipe fail!"));
			throw(0);
		}
		//------------------------------ 

		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() + strFastBootCmd.GetLength() + 2048));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//
		CString strFastbootPath,subPath;
		strFastbootPath.Format(_T("%s\\NB-IoT\\%d\\fastboot.exe"),this->m_csCurr_Exe_Path, this->m_iClass_Idx);

		//去掉开启的fastboot字样，替换为fastboot.exe全路径
		subPath = strFastBootCmd.Right(strFastBootCmd.GetLength()-8);

		//------------------------------ 打包dos指令 ------------------------------ 
		//去除>> burn_log.txt
		subPath.Replace(_T(">> burn_log.txt"),_T(""));

		swprintf(wcpCmd, _T("cmd.exe %cc %s %s"),'/', strFastbootPath,subPath);

		this->Save_Trace_Log(wcpCmd);


		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//创建子进程
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_DL_FILE, Create process fail!"));
			throw(2);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 


		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		csTotal_Info.Format(_T(""));
		//开始计时
		time(&t_Start);
		//
		while(bSuccess_Flag == false)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 2000, &ulActual_Read, NULL);

			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				this->Save_Trace_Log(csData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));
			

				//显示每次的读出data
				csTemp.Format(_T("FB Download [ %s ] %s"), csFB_Type_Name, csData);
				this->Show_Status(csTemp);

				//汇总数据
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
			}//if(ulActual_Read > 0


			//检查是否有错误报出
			if(wcsstr(csTotal_Info, _T("fail")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, FB_DL_FILE, Download %s fail!"), csFB_Type_Name);
				this->m_iError_Code = ERR_FB_DL_FILE_FAIL;

				throw(3);
			}

			//检查是否pass  
			if((wcsstr(csTotal_Info, _T("finished")) != NULL) && (wcsstr(csTotal_Info, _T("total time")) != NULL))
			{
				//显示进度条
				this->m_ul_Transmit_Bytes += csFB_size;
// 
				double dwPor =0.0;
				dwPor = (double)this->m_ul_Transmit_Bytes / this->m_ul_DL_All_Bytes;


				this->Show_Progress(dwPor);

				bSuccess_Flag = true;//pass
				break;//退出while
			}


			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 20)//超时，报fail
			{
				this->m_csError_Code.Format(_T("FAIL, FB_DL_FILE, Time out!"));
				this->m_iError_Code = ERR_FB_DL_FILE_TIME_OUT;
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------


		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	}
	catch(...)
	{
		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return bSuccess_Flag;
}

int CARM_OP_HUAWEI::Do_Execute_HI2152_5G()
{
	CString  csData;
	int iDl_Res =RTN_FAIL;
	int iTimeOut = 30;//探测adb设备的超时
	int iTemp=0,iTemp1=0, i;
	time_t t_Start, t_End;

	try
	{

		this->m_bRuning = true;

		int irQDL_Port_List[64];
		int iQDL_Port_Quan;
		bool bEDL_Now = false;
		int iQDLoader_Port =0;

//仅支持单口
#ifndef FW_MULTI
		//------------------------------ 判断COM口是否已是“EDL的QDLoader口” ------------------------------ 
		//清空
		iQDL_Port_Quan = 0;
		memset(irQDL_Port_List, 0, sizeof(irQDL_Port_List));

		//读PC机存在的所有“QDLoader”口记录
		this->Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(irQDL_Port_List, &iQDL_Port_Quan);

		//核对“当前串口”是不是已经是“QDLoader口”
		for(i=0; i<iQDL_Port_Quan; i++)
		{
			if(this->m_iCom_Port == irQDL_Port_List[i])
			{
				//“当前串口”已经是“QDLoader口”
				bEDL_Now = true;
				break;//已经是EDL模式
			}
		}//for(j=0; j<iQDL_Port_Quan; j++
		//确定下载用的“QDLoader口”
		if(bEDL_Now == true)
		{
			//已经是EDL模式
			iQDLoader_Port = this->m_iCom_Port;

			csData.Format(_T("QDLoader Port(%d)..."),this->m_iCom_Port);
			this->Show_Out(csData);
		}
		else
		{
			//切换到EDL模式
			if(this->Switch_EDL( &iQDLoader_Port ) == false)
			{
				this->Show_Out(m_csError_Code);
				this->Show_Status(this->m_csError_Code);
				throw(2);
			}

		}
#else
		//已经是EDL模式
		iQDLoader_Port = this->m_iCom_Port;
#endif
		//使用burn.exe下载固件包中的usbloader.bin文件
		this->Show_Status(_T("Start download usbloader ......"));
		this->Show_Out(_T("Start download usbloader ......"));

	

		//Hiburn.exe    b5000            burn       115200    "HUAWEI Mobile Connect - DownLoad port"    D:\images      COM10
		if (false  == this->DL_HiBurn(iQDLoader_Port))
		{
			this->Show_Progress(1.0);
			iDl_Res =RTN_FAIL;
			this->m_bRuning = false;
			return RTN_FAIL;
		}
		else
		{
			this->Show_Progress(1.0);
			this->Show_Status(_T("Download Firwmare Pass"));
		}
		this->m_bRuning = false;

		return RTN_SUCCESS;
	}
	catch(...)
	{
		iDl_Res =RTN_FAIL;

		this->m_bRuning = false;

	}

	this->m_bRuning = false;

	return iDl_Res;
}

int CARM_OP_HUAWEI::Do_Execute_HI2152()
{
	CString  csData;
	int iDl_Res =RTN_FAIL;
	int iTimeOut = 30;//探测adb设备的超时
	int iTemp=0,iTemp1=0, i;
	time_t t_Start, t_End;

	try
	{
		this->m_bRuning = true;

		int irQDL_Port_List[64];
		int iQDL_Port_Quan;
		bool bEDL_Now = false;
		int iQDLoader_Port =0;

//仅支持单口
#ifndef FW_MULTI
		//------------------------------ 判断COM口是否已是“EDL的QDLoader口” ------------------------------ 
		//清空
		iQDL_Port_Quan = 0;
		memset(irQDL_Port_List, 0, sizeof(irQDL_Port_List));

		//读PC机存在的所有“QDLoader”口记录
		this->Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(irQDL_Port_List, &iQDL_Port_Quan);

		//核对“当前串口”是不是已经是“8909的QDLoader口”
		for(i=0; i<iQDL_Port_Quan; i++)
		{
			if(this->m_iCom_Port == irQDL_Port_List[i])
			{
				//“当前串口”已经是“8909的QDLoader口”
				bEDL_Now = true;
				break;//已经是EDL模式
			}
		}//for(j=0; j<iQDL_Port_Quan; j++
		//确定下载用的“QDLoader口”
		if(bEDL_Now == true)
		{
			//已经是EDL模式
			iQDLoader_Port = this->m_iCom_Port;

			csData.Format(_T("QDLoader Port(%d)..."),this->m_iCom_Port);
			this->Show_Out(csData);
		}
		else
		{
			//切换到EDL模式
			if(this->Switch_EDL( &iQDLoader_Port ) == false)
			{
				this->Show_Out(m_csError_Code);
				this->Show_Status(this->m_csError_Code);
				throw(2);
			}

		}
#else
		iQDLoader_Port = this->m_iCom_Port;
#endif

		//使用burn.exe下载固件包中的usbloader.bin文件
		this->Show_Status(_T("Start download usbloader ......"));
		this->Show_Out(_T("Start download usbloader ......"));


		//Hiburn.exe    v711            burn       115200    "HUAWEI Mobile Connect - DownLoad port"    D:\images      COM10
		if (false  == this->DL_HiBurn(iQDLoader_Port))
		{
			this->Show_Progress(1.0);

			iDl_Res =RTN_FAIL;
			this->m_bRuning = false;
			return RTN_FAIL;
		}
		else
		{
			this->Show_Progress(1.0);

			this->Show_Status(_T("Download Firwmare Pass"));
		}
		this->m_bRuning = false;

		return RTN_SUCCESS;
	}
	catch(...)
	{
		iDl_Res =RTN_FAIL;

		this->m_bRuning = false;

	}

	this->m_bRuning = false;

	return iDl_Res;
}

//查询ADB端口
bool CARM_OP_HUAWEI::Find_ADB(void)
{
	CUSBDeviceDetection usbDeviceDetect;
	CStringArray strPhysicalAddress,strDeviceInstancePath;
	UINT uDeviceCount;
	int i =0;

	CString strPAD;

	usbDeviceDetect.GetUsbPhysicalAddressList(strPhysicalAddress,strDeviceInstancePath,uDeviceCount);

	if (strPhysicalAddress.GetSize()>0)
	{	
#ifdef FW_MULTI
#ifndef FW_UPGRADE

		for (i=0;i<strPhysicalAddress.GetSize();i++)
		{
			 strPAD = strPhysicalAddress.GetAt(i);
			 if(0 == wcscmp(strPAD ,m_strUSBPortAddress))
			 {
				 return true;
			 }
		}
		return false;
#endif
#else
		return true;
#endif

	}
	else
	{
		return false;
	}

}
//---------------------------------------------------------
bool CARM_OP_HUAWEI::FB_GET_FLASH_SIZE(int*  p_iFlash_size)
{
	//使用fastboot.exe下载获取FLASH 大小

	//先确认是2K还是4K
	//fastboot -i 0x12d1 getvar pagesize 2>&1
	//pagesize: 2048
	//finished. total time: 0.008s


	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csTemp;
	int iTemp;
	bool bSuccess_Flag = false;

	try
	{
		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_GET_FLASH_SIZE, Create pipe fail!"));
			throw(0);
		}
		//------------------------------ 

		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() + 512));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//
		CString strFastbootCmd;
		strFastbootCmd.Format(_T("%s\\NB-IoT\\%d\\fastboot.exe -i 0x12d1 getvar pagesize 2>&1"),this->m_csCurr_Exe_Path, this->m_iClass_Idx);


		//------------------------------ 打包dos指令 ------------------------------ 
		swprintf(wcpCmd, _T("cmd.exe %cc %s"),'/', strFastbootCmd);

		this->Save_Trace_Log(wcpCmd);


		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//创建子进程
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, FB_GET_FLASH_SIZE, Create process fail!"));
			throw(2);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 


		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		csTotal_Info.Format(_T(""));
		//开始计时
		time(&t_Start);
		//
		while(bSuccess_Flag == false)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 2000, &ulActual_Read, NULL);

			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				this->Save_Trace_Log(csData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				//pagesize: 2048
				//finished. total time: 0.008s


				//数值转换（提取COM口号码）
				csData.MakeLower();

				wchar_t wcrData[128];

				wchar_t * wcpPoint = NULL;

				swprintf_s(wcrData, _T("%s"), csData);

				wcpPoint = wcsstr(wcrData, _T("pagesize:"));
				if (NULL != wcpPoint)
				{
					wcpPoint += 10;
					*p_iFlash_size = _wtoi(wcpPoint);
				}
				else
				{
					::AfxMessageBox(_T("pagesize get fail!"));
					this->Show_Out(_T("pagesize get fail!"));
					this->Show_Status(_T("pagesize get fail!"));
					throw(0);
				}

				this->Show_Out(csData);

				//汇总数据
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
			}//if(ulActual_Read > 0


			//检查是否有错误报出
			if(wcsstr(csTotal_Info, _T("fail")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, FB_GET_FLASH_SIZE, Download fail!"));
				throw(3);
			}

			//检查是否pass  
			if((wcsstr(csTotal_Info, _T("finished")) != NULL) && (wcsstr(csTotal_Info, _T("total time")) != NULL))
			{
				bSuccess_Flag = true;//pass
				break;//退出while
			}


			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 20)//超时，报fail
			{
				this->m_csError_Code.Format(_T("FAIL, FB_GET_FLASH_SIZE, Time out!"));
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------


		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	}
	catch(...)
	{
		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return bSuccess_Flag;
}
//---------------------------------------------------------
bool CARM_OP_HUAWEI::DL_Usbloader(void)
{
	//使用burn.exe下载固件包中的usbloader.bin文件

// 	------------------------------------------------------------------------
// 
// 		ports number :  1
// 		Starting at: Thu Dec  5 18:58:25 2019
// 		thread number : 0 ,Find Device on : COM33
//		COM33 : Download RAMInit -->
//		 COM33 : Download BOOTLOADER -->
// 		All done at : %s Thu Dec  5 18:58:28 2019
// 
// 		------------------------------------------------------------------------


	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csTemp;
	int iTemp;
	bool bSuccess_Flag = false;

	try
	{
		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, DL_Usbloader, Create pipe fail!"));
			this->m_iError_Code = DL_Usbloader_Create_Pipe_Fail;
			return false;
		}
		//------------------------------ 

		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength()*2 + 512));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//
		CString strFastbootCmd;
		//20200120 --usbloader.bin文件改为固件包中的文件

		strFastbootCmd.Format(_T("%s\\NB-IoT\\%d\\burn.exe %s"),this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_strUsbLoadPath);


		//------------------------------ 打包dos指令 ------------------------------ 
		swprintf(wcpCmd, _T("cmd.exe %cc %s"),'/', strFastbootCmd);

		this->Save_Trace_Log(wcpCmd);


		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//创建子进程
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, DL_Usbloader, Create process fail!"));
			this->m_iError_Code = DL_Usbloader_Create_Process_Fail;
			bSuccess_Flag =false;
			throw(2);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 


		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		csTotal_Info.Format(_T(""));
		//开始计时
		time(&t_Start);
		//
		while(bSuccess_Flag == false)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 2000, &ulActual_Read, NULL);

			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				csData.Format(_T("%s"), wcpData);

				this->Save_Trace_Log(csData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				this->Show_Out(csData);

				//汇总数据
				csTotal_Info += csData;
				csTotal_Info.MakeLower();
			}//if(ulActual_Read > 0


			//检查是否有错误报出
			if(wcsstr(csTotal_Info, _T("fail")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Usbloader, Download fail!"));
				this->m_iError_Code = ERR_DL_USBLOADER_FAIL;
				bSuccess_Flag =false;
				throw(3);
			}

			//检查是否pass  
			if(wcsstr(csTotal_Info, _T("all done")) != NULL)
			{
				bSuccess_Flag = true;//pass
				break;//退出while
			}


			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= 30)//超时，报fail
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Usbloader, Time out!"));
				this->m_iError_Code = ERR_DL_USBLOADER_TIMEOUT;
				bSuccess_Flag =false;
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------


		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
	}
	catch(...)
	{
		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return bSuccess_Flag;
}


//---------------------------------------------------------
bool CARM_OP_HUAWEI::DL_HiBurn(int  iQDLoader_Port)
{
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpBengin = NULL;
	wchar_t * wcpEnd = NULL;

	char * cpRead_Buff = NULL;
	double dNew_Percent = 0.0;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData;
	int iTemp;
	bool bSuccess_Flag = false;
	int iTimeout = 300;//300秒改为240秒


	int iBenginPos =-1;
	int iBenginEnd =-1;
	CString strPos;


	try
	{
		//检查下端口有没有出来 20200511
		this->Show_Status(_T("Check Port......"));
		this->Save_Trace_Log(_T("Check Port......"));

		if(this->Confirm_Dynamic_Com_Port(iQDLoader_Port, 30) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port:%d"), this->m_iCom_Port);
			this->Show_Status(_T("Check Port Fail"));
			this->Save_Trace_Log(_T("Check Port Fail"));
			throw(0);
		}
		this->Show_Status(_T("Check Port Pass"));
		this->Save_Trace_Log(_T("Check Port Pass"));



		//显示
		this->Show_Status(_T("DL_HiBurn......"));
		this->Save_Trace_Log(_T("DL_HiBurn......"));

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, DL_HiBurn, Create pipe fail!"));
			this->m_iError_Code  = DL_HiBurn_Create_Pipe_Fail;
			throw(1);
		}
		//------------------------------ 


		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength()*3 + 256));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 1024);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 1024);



		//------------------------------ 打包dos指令 ------------------------------  


       if ( BB_HI2152 == this->m_iBBchip)
       {
		   if(0 == m_iHS_DL_MODE_M)// Quectel DLoader Port
		   {
			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe v711 burn  115200 \"Quectel DLoader Port\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);

		   }
		   else if(1 == m_iHS_DL_MODE_M)//HUAWEI Mobile Connect - DownLoad port
		   {
			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe v711 burn  115200 \"HUAWEI Mobile Connect - DownLoad port\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);

		   }
// 		   else if(2 == m_iHS_DL_MODE_M)//HUAWEI Mobile Connect - PC UI Interface
// 		   {
// 			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe v711 update  115200 \"HUAWEI Mobile Connect - PC UI Interface\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);
// 
// 		   }
		   else
		   {
			   //...
		   }

       } 
       else if ( BB_HI9500 == this->m_iBBchip)
       {
		   if(0 == m_iHS_DL_MODE_M)// Quectel DLoader Port
		   {
			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe b5000 burn  115200 \"Quectel DLoader Port\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);

		   }
		   else if(1 == m_iHS_DL_MODE_M)//HUAWEI Mobile Connect - DownLoad port
		   {
			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe b5000 burn  115200 \"HUAWEI Mobile Connect - DownLoad port\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);

		   }
// 		   else if(2 == m_iHS_DL_MODE_M)//HUAWEI Mobile Connect - PC UI Interface
// 		   {
// 			   swprintf(wcpCmd, _T("cmd.exe %cc %s\\HIS\\%d\\hiburn%d.exe b5000 update  115200 \"HUAWEI Mobile Connect - PC UI Interface\"  %s COM%d"),'/',this->m_csCurr_Exe_Path, this->m_iClass_Idx,this->m_iClass_Idx,this->m_strUsbLoadPath,iQDLoader_Port);
// 
// 		   }
		   else
		   {
			   //...
		   }

	   }
	   else
	   {
		   AfxMessageBox(_T("BB chip Error,Please Check"));
		   this->m_csError_Code.Format(_T("FAIL, DL_HiBurn,BB chip Error!"));
		   this->m_iError_Code  = DL_HiBurn_Create_Process_Fail;
		   throw(2);
	   }
        
		this->Save_Trace_Log(wcpCmd);


		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//创建子进程
		if(CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE) 
		{
			this->m_csError_Code.Format(_T("FAIL, DL_HiBurn, Create process fail!"));
			this->m_iError_Code  = DL_HiBurn_Create_Process_Fail;
			throw(2);
		}
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 



		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		csTotal_Info.Format(_T(""));
		//开始计时
		time(&t_Start);
		//
		while(bSuccess_Flag == false)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 1024);
			ulActual_Read = 0;


			if (TRUE == ::PeekNamedPipe(hRead_Pipe,cpRead_Buff,1024,&ulActual_Read,0,0))
			{
				//
				if (ulActual_Read > 0)
				{								
					ReadFile(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, NULL);
					//注意：如果端口状态异常的话，程序会吊死在这里。一直无返回。


					//读到info数据了
					if(ulActual_Read > 0)
					{
						//字符转换
						memset(wcpData, 0, 1024);
						this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

						//处理数据
						csData.Format(_T("%s"), wcpData);
						csData.Replace(_T('\r'), _T(' '));
						csData.Replace(_T('\n'), _T(' '));
						csData.MakeLower();



						this->Save_Trace_Log(wcpData);
						this->Show_Out(wcpData);


						//逐条显示每次的读出data
						this->Show_Status_In_Each(wcpData, _T("DL_HiBurn "));


						//汇总数据
						csTotal_Info = csData;

						

						while(1)
						{
							wcpBengin = wcsstr(csTotal_Info.GetBuffer(), _T("[ "));
							if (NULL != wcpBengin)
							{
								csTotal_Info.Format(_T("%s"),wcpBengin+5);
							}
							else
							{
								break;
							}						
						}
						
						wcpBengin = wcsstr(csTotal_Info.GetBuffer(), _T("["));
						wcpEnd = wcsstr(csTotal_Info.GetBuffer(), _T("%]"));

						if ((NULL != wcpBengin)&&(NULL != wcpEnd))
						{

							iBenginPos =  csTotal_Info.Find( _T("["));
							iBenginEnd =  csTotal_Info.Find( _T("%]"));

							strPos = csTotal_Info.Mid(iBenginPos+1,iBenginEnd-iBenginPos-1);
							//字符转数字
							dNew_Percent = _wtof(strPos.GetBuffer());
							dNew_Percent =dNew_Percent/100;

							this->Show_Progress(dNew_Percent);
						}

						//20180425
					}//if(ulActual_Read > 0
				}
				else
				{
					//
					//this->Show_Status(_T("Data Waiting..."));
				}
			}
			else
			{
				//
				//this->Show_Status(_T("PeekNamedPipe NULL..."));

			}



			//检查是否有错误报出
			if(wcsstr(csTotal_Info, _T("failed:1")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}

			if(wcsstr(csTotal_Info, _T("not scanned")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}

			if(wcsstr(csTotal_Info, _T("failed to be written")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}

			if(wcsstr(csTotal_Info, _T("oserror")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}

			if(wcsstr(csTotal_Info, _T("image file download fail")) != NULL) 
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn error!"));
				this->m_iError_Code  = DL_HiBurn_FAIL;
				throw(3);
			}			


			//检查是否pass
			if(wcsstr(csTotal_Info, _T("image file download pass")) != NULL)
			{
				bSuccess_Flag = true;//pass
				break;//退出while
			}


			//超时检测
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn, Time out!"));
				this->m_iError_Code  = ERR_DL_USBLOADER_TIMEOUT;
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------


		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	}
	catch(...)
	{
		//显示失败信息
		this->Show_Status(this->m_csError_Code);
		this->Save_Trace_Log(m_csError_Code);



		//释放句柄的指针
		if(hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);
		//
		return false;
	}

	//
	return true;
}
void CARM_OP_HUAWEI::Show_Status_In_Each(const wchar_t * wcp_Status, const wchar_t * wcp_Title_In_Each)
{
	CString csTemp;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpSrc = NULL;//“源数据”的buff，wcstok在提取数据时会改变源数据内容，所以先把源数据放入buff，解析buff数据，不伤害参数源。
	unsigned long ulBuff_Len = wcslen(wcp_Status) + 8;//用于存放“源数据”的buff长度


	//分配内存
	this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpSrc, ulBuff_Len);


	//载入“源数据”
	swprintf(wcpSrc, _T("%s"), wcp_Status);

	//提取第一个值(0)
	wcpEach = wcstok(wcpSrc, _T("\r\n"));
	//
	while(wcpEach != NULL)
	{
		//显示
		csTemp.Format(_T("%s%s"), wcp_Title_In_Each, wcpEach);
		this->Show_Status(csTemp);
		//this->Save_Trace_Log(csTemp);

		//无延迟，则看不出显示信息的变化
		Sleep(20);

		//提取下一个值
		wcpEach = wcstok(NULL, _T("\r\n"));
	}//while(wcpEach != NULL


	//释放内存
	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpSrc);
}


//每片模块下载前都必须给每个通道的程控电源上电
void  CARM_OP_HUAWEI::Open_Power_SPC(int iIndex)
{
#ifdef	FW_MULTI
#ifndef FW_UPGRADE

	switch(iIndex)
	{
	case 1:
		{
			//打开电源
			this->m_cla_PS_1->Power_Off(1);

			this->Show_Out(_T("----Power_Off----"));

			Sleep(2000);

			//上电
			//程控电源上电
			if(this->m_cla_PS_1->m_bCtrl_PS == true)
			{
				this->m_cla_PS_1->POWER_ON_V1( 1 );

				this->Show_Out(_T("----Power_On----"));

				//
				Sleep(500);
			}
			else
			{
				this->Show_Out(_T("m_cla_PS_1->m_bCtrl_PS= false"));
			}

		}
		break;
	case 2:
		{
			//打开电源
			this->m_cla_PS_2->Power_Off(1);

			this->Show_Out(_T("----Power_Off----"));

			Sleep(2000);

			//上电
			//程控电源上电
			if(this->m_cla_PS_2->m_bCtrl_PS == true)
			{
				this->m_cla_PS_2->POWER_ON_V1( 1 );

				this->Show_Out(_T("----Power_On----"));
				//
				Sleep(500);
			}
			else
			{
				this->Show_Out(_T("m_cla_PS_2->m_bCtrl_PS= false"));
			}

		}
		break;
	case 3:
		{
			//打开电源
			this->m_cla_PS_3->Power_Off(1);

			this->Show_Out(_T("----Power_Off----"));

			Sleep(2000);

			//上电
			//程控电源上电
			if(this->m_cla_PS_3->m_bCtrl_PS == true)
			{
				this->m_cla_PS_3->POWER_ON_V1( 1 );

				this->Show_Out(_T("----Power_On----"));
				//
				Sleep(500);
			}
			else
			{
				this->Show_Out(_T("m_cla_PS_3->m_bCtrl_PS= false"));
			}

		}
		break;
	case 4:
		{
			//打开电源
			this->m_cla_PS_4->Power_Off(1);

			this->Show_Out(_T("----Power_Off----"));

			Sleep(2000);

			//上电
			//程控电源上电
			if(this->m_cla_PS_4->m_bCtrl_PS == true)
			{
				this->m_cla_PS_4->POWER_ON_V1( 1 );

				this->Show_Out(_T("----Power_On----"));

				//
				Sleep(500);
			}
			else
			{
				this->Show_Out(_T("m_cla_PS_4->m_bCtrl_PS= false"));
			}

		}
		break;
	default:break;
	}

#endif
#endif
}

bool CARM_OP_HUAWEI::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
{
	time_t t_Start, t_End;
	CString csA, csB;
	int j = 0;

	try
	{
		csB.Format(_T(" ."));

		time(&t_Start);
		csA.Format(_T("Wait port(%d) is normal..."),i_Tgt_Com_Port);
		this->Show_Status(csA);
		this->Show_Out(csA);
		Save_Trace_Log(csA);

		do
		{
			//读出regedit中的COM列表
			if(this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == true)
			{
				//找到串口了
				//等待5秒后继续检查一遍
				Sleep(5000);
				if(this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == false)
				{
					//继续					
				}
				else
				{
					this->Show_Status(_T("Get Port Pass"));
					this->Show_Out(_T("Get Port Pass"));
					Save_Trace_Log(_T("Get Port Pass"));
					return true;
				}

			}

			//系统暂停
			if(this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//检查是否超时
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port %d timeout!"), i_Tgt_Com_Port);
				//this->m_iError_Code  = Confirm_Dynamic_Com_Port_fail1;
				throw(0);
			}

			//进度简易显示
			if((j % 10) == 0)
			{
				csA.Format(_T("Detect dynamic com port %d"), i_Tgt_Com_Port);
			}

			csA += csB;
			this->Show_Status(csA);
			j++;

			//每次读取注册表的延迟
			Sleep(800);
		}
		while(true);
	}
	catch(...)
	{
		this->Show_Status(m_csError_Code);
		this->Show_Out(m_csError_Code);
		Save_Trace_Log(m_csError_Code);
		return false;
	}

	return false;
}

bool CARM_OP_HUAWEI::Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(int * ip_QDLoader_Port_List, int * ip_Port_Quan)
{
	//从注册表读取“QDLoader下载设备”列表

	wchar_t wcrUSB_PID_VID[64];//DM口
	CString csrUsb_Dev_List[64], csFriendly_Name;
	int iUsb_Dev_Quan, i, iCnt;
	wchar_t wcrData[128];
	wchar_t * wcpPoint = NULL;

	g_cla_HS_Lock_QDLoader_Port.Lock();
	try
	{
		swprintf_s(wcrUSB_PID_VID, _T("VID_12D1&PID_1443"));
		//
		*ip_Port_Quan = 0;
		iUsb_Dev_Quan = 0;
		iCnt = 0;

		//读PC上可能多个QDLoader下载设备列表
		this->m_cla_GFC.Get_Reg_Usb_Dev_List(wcrUSB_PID_VID, csrUsb_Dev_List, &iUsb_Dev_Quan);

		//
		for(i=0; i<iUsb_Dev_Quan; i++)
		{
			csFriendly_Name.Format(_T(""));
			
			//读取设备的“FriendlyName”（例如：Quectel DLoader Port (COM11)）
			if(this->m_cla_GFC.Get_Reg_Usb_Dev_Friendly_Name(wcrUSB_PID_VID, csrUsb_Dev_List[i], &csFriendly_Name) == false)
			{				
				//不连续，可能其中没有FriendlyName
				continue;								
			}


			//数值转换（提取COM口号码）
			csFriendly_Name.MakeUpper();
			csFriendly_Name.Replace(')', '\0');

			swprintf_s(wcrData, _T("%s"), csFriendly_Name);

			wcpPoint = wcsstr(wcrData, _T("(COM"));
			if (NULL != wcpPoint)
			{
				wcpPoint += 4;
				*(ip_QDLoader_Port_List + iCnt) = _wtoi(wcpPoint);
			}
			else
			{
				::AfxMessageBox(_T("COM NULL, Fail!"));
				throw(0);
			}
			


			//累加
			iCnt++;
		}//for(i=0; i<iUsb_Dev_Quan; i++

		//
		*ip_Port_Quan = iCnt;
	}
	catch(...)
	{
		g_cla_HS_Lock_QDLoader_Port.Unlock();
		return false;
	}
	g_cla_HS_Lock_QDLoader_Port.Unlock();
	return true;
}
bool CARM_OP_HUAWEI::Switch_EDL(int * ip_New_QDLoader_Port)
{
	//切换EDL模式，并探测、返回新下载用的QDLoader口
	int iNew_QDLoader_Port = 0;
	CString csData;

	int iQD_1(0),iQD_2(0),iQD_3(0),iQD_4(0);
	BOOL bFind=FALSE;

	//
	try
	{
		//显示

		csData.Format(_T("Switch EDL mode(Port%d) ......"),this->m_iCom_Port);
		this->Show_Status(csData);
		this->Show_Out(csData);
		this->Save_Trace_Log(csData);


		//代码锁定
		::g_cla_HS_Lock_Enter_EDL.Lock();

		//清空buff
		m_iCom_Quan_old = 0;
		memset(m_irCom_List_old, 0, sizeof(m_irCom_List_old));
		if(this->m_cla_GFC.Get_Reg_Com_Port_List(m_irCom_List_old, &m_iCom_Quan_old) == false)
		{		
			this->m_csError_Code.Format(_T("FAIL, Get_Reg_Com_Port_List, Read reg fail!"));
			throw(0);
		}



		//发送切换EDL模式的指令（AT+QDOWNLOAD=1）
		if (false ==Switch_HS_EDL())
		{
			//再进行一次
			if(false ==Switch_HS_EDL())
			{
				throw(0);
			}
		}

		//----------------------- 确认EDL用的“新生成的”QDLoader下载口（此处QDLoader下载口会“无法预知的变号”）-----------------------
		
		if(this->Update_HS_USB_QDLoader_Port(this->m_iCom_Port, &iNew_QDLoader_Port) == false)
		{	
			throw(2);
		}
		else
		{
			//正常的
			this->Show_Status(_T("Find QDLoader Port Success(First)"));
			this->Show_Out(_T("Find QDLoader Port Success(First)"));
		}
		//返回数据
		*ip_New_QDLoader_Port = iNew_QDLoader_Port;


		//显示
		this->Show_Status(_T("Switch EDL mode completely"));
		this->Show_Out(_T("Switch EDL mode completely"));
	}
	catch(...)
	{
		//代码解锁
		::g_cla_HS_Lock_Enter_EDL.Unlock();
		//
		return false;
	}


	//代码解锁
	::g_cla_HS_Lock_Enter_EDL.Unlock();
	//
	return true;
}

bool CARM_OP_HUAWEI::Switch_HS_EDL(void)
{
	CString csData;
	try
	{  
		//----------------------- 打开串口（第一次）----------------------- 
		csData.Format(_T("Open Port%d ......"),this->m_iCom_Port);
		this->Show_Status(csData);
		this->Show_Out(csData);
		this->Save_Trace_Log(csData);

		//检查串口是否可打开 
		this->m_cla_Com_Port.m_iPort = this->m_iCom_Port;
		this->m_cla_Com_Port.m_iBaudRate = 115200;
		//this->m_cla_Com_Port.m_dwDTR_CTRL = DTR_CONTROL_DISABLE;
		//this->m_cla_Com_Port.m_dwRTS_CTRL = RTS_CONTROL_DISABLE;

		if(false == this->m_cla_Com_Port.Port_Open(400)) 
		{
			time_t start,finish;
			int iTimeOut =9;//打开端口改为循环查找打开的方式
			try
			{
				//
				time( &start );
				do
				{
					Sleep( 200 );
					if(true == this->m_cla_Com_Port.Port_Open(400))
					{
						this->Show_Status(_T("Open Port Pass"));
						this->Show_Out(_T("Open Port Pass"));
						this->Save_Trace_Log(_T("Open Port Pass"));
						break;
					}
					//
					time( &finish );
					//
					if( (int)difftime( finish, start ) >= iTimeOut )
					{
						throw(0);
					}
				}
				while( true );
			}
			catch(...)
			{
				csData.Format(_T("Open Port fail"));
				this->Show_Status(csData);
				m_csError_Code = csData;
				this->Show_Out(csData);
				this->Save_Trace_Log(csData);
				throw(0);
			}

		}


		//----------------------- 发送切换EDL模式的指令（AT+QDOWNLOAD=1）-----------------------
		if(this->Send_HS_EDL_Mode_Cmd() == false)
		{
			throw(1);
		}
		//----------------------- 关闭串口 -----------------------
		this->m_cla_Com_Port.Port_Close();
		this->Show_Status(_T("Close DM Port"));
		this->Show_Out(_T("Close DM Port"));
		this->Save_Trace_Log(_T("Close DM Port"));
	}
	catch (...)
	{
		//关闭串口
		this->m_cla_Com_Port.Port_Close();
		return false;
	}
	return true;
}
bool CARM_OP_HUAWEI::Send_HS_EDL_Mode_Cmd(void)
{
	CString strData;

	try
	{
		//20180523	
		strData.Format(_T("Send edl mode cmd(Port%d) ....."),this->m_iCom_Port);
		this->Show_Status(strData);
		this->Show_Out(strData);
		this->Save_Trace_Log(strData);

		if(this->Set_QDOWNLOAD_Mode() == false)
		{
			throw(0);
		}

	}
	catch(...)
	{
		this->m_csError_Code += _T(" [Send_EDL_Mode]");
		this->Show_Out(_T("Send_EDL_Mode Fail"));
		this->Save_Trace_Log(_T("Send_EDL_Mode Fail"));
		return false;
	}

	this->Show_Status(_T("Send_EDL_Mode Pass"));
	this->Show_Out(_T("Send_EDL_Mode Pass"));
	this->Save_Trace_Log(_T("Send_EDL_Mode Pass"));
	return true;
}

bool CARM_OP_HUAWEI::Update_HS_USB_QDLoader_Port(int i_Old_Port, int * ip_New_QDLoader_Port)
{
	int irNew_Port[64], iNew_Quan, i, j,index(0);
	int irQDL_Port_List[64], iQDL_Port_Quan;//所有“QDLoader”口的记录


	try
	{
		iNew_Quan = 0;
		memset(irNew_Port, 0, sizeof(irNew_Port));

		//读取“新生成”的com port口列表
		if(this->Get_HS_New_Dynamic_Com_Port(irNew_Port, &iNew_Quan, 60) == false)//add 20180509
		{
			this->Show_Status(this->m_csError_Code);
			throw(0);
		}

		//最初的下载口消失了，需要重新确定可能的下载口
		iQDL_Port_Quan = 0;
		memset(irQDL_Port_List, 0, sizeof(irQDL_Port_List));


		//读PC机存在的所有“QDLoader”口记录
		this->Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(irQDL_Port_List, &iQDL_Port_Quan);


		//核对“新生成的COM口”是不是“QDLoader口”
		for(i=0; i<iNew_Quan; i++)
		{
			for(j=0; j<iQDL_Port_Quan; j++)
			{
				if(irNew_Port[i] == irQDL_Port_List[j])
				{
					
					*ip_New_QDLoader_Port = irNew_Port[i];
					return true;

				}						
			}//for(j=0; j<iQDL_Port_Quan; j++
		}//for(i=0; i<iNew_Quan; i++
	}
	catch(...)
	{
		this->m_csError_Code.Format(_T("FAIL, Update_USB_QDLoader_Port, No right port!"));
		return false;
	}

	
	return false;
}

bool CARM_OP_HUAWEI::Get_HS_New_Dynamic_Com_Port(int * ip_New_Port_List, int * ip_New_Port_Quan, int i_TimeOut)
{
	time_t t_Start, t_End;
	CString csNew, csData;
	int irCom_List_Min[64], iCom_Quan_Min, irList_Buff[64], iQuan_Buff, x, y, iTime;
	int irCom_List_New[64], iCom_Quan_New;
	int iGet_New_Port_Flag = 0;


	try
	{
		//清空buff
		iCom_Quan_Min = 0;
		memset(irCom_List_Min, 0, sizeof(irCom_List_Min));
		iQuan_Buff = 0;
		memset(irList_Buff, 0, sizeof(irList_Buff));
		iCom_Quan_New = 0;
		memset(irCom_List_New, 0, sizeof(irCom_List_New));
		csNew.Format(_T(""));

		Sleep(3000);

		//初始值“min状态”的buff
// 		if(this->m_cla_GFC.Get_Reg_Com_Port_List(irCom_List_Min, &iCom_Quan_Min) == false)
// 		{		
// 			this->m_csError_Code.Format(_T("FAIL, Get_New_Dynamic_Com_Port, Read reg fail!"));
// 			throw(0);
// 		}
		//
		iCom_Quan_Min = m_iCom_Quan_old;
		memcpy(irCom_List_Min, m_irCom_List_old, sizeof(irCom_List_Min));


		time(&t_Start);
		//
		do
		{
			//读出regedit中的COM列表
			if(this->m_cla_GFC.Get_Reg_Com_Port_List(irList_Buff, &iQuan_Buff) == false)
			{
				this->m_csError_Code.Format(_T("FAIL, Get_New_Dynamic_Com_Port, Read reg fail!"));
				throw(0);
			}
			//
			for(x=0; x<iQuan_Buff; x++)
			{
				for(y=0; y<iCom_Quan_Min; y++)
				{
					if(irList_Buff[x] == irCom_List_Min[y])
					{
						break;
					}
				}
				//发现全新的com口
				if(y >= iCom_Quan_Min)
				{
					irCom_List_New[iCom_Quan_New] = irList_Buff[x];
					iCom_Quan_New++;

					//
					csData.Format(_T("%d "), irList_Buff[x]);
					csNew += csData;

					//有新com port生成
					iGet_New_Port_Flag++;
				}
			}//for(x=0; x<iQuan_Buff; x++

			//系统暂停
			if(this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//检查是否超时
			time(&t_End);
			iTime = i_TimeOut - (int)difftime(t_End, t_Start);

			if(csNew.GetLength() > 0)
			{
				csData.Format(_T("Detect dynamic new com port( %s) ...... %d"), csNew, iTime);
			}
			else
			{
				csData.Format(_T("Detect dynamic new com port ...... %d"), iTime);
			}

			this->Show_Status(csData);

			if(iTime <= 0)
			{
				this->m_csError_Code.Format(_T("FAIL, Get_New_Dynamic_Com_Port, Timeout fail!"));
				throw(0);
			}
			if(iGet_New_Port_Flag >= 1)
			{
				break;//退出while
			}

			//每次读取注册表的延迟
			if(iGet_New_Port_Flag > 0)
			{
				Sleep(1000);
			}
			else
			{
				Sleep(400);
			}
		}
		while(true);


		//返回值的载入
		*ip_New_Port_Quan = iCom_Quan_New;
		//
		for(x=0; x<iCom_Quan_New; x++)
		{
			*(ip_New_Port_List + x) = irCom_List_New[x];
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}
bool CARM_OP_HUAWEI::Set_QDOWNLOAD_Mode(void)
{
	// TODO: Add your control notification handler code here
	CString csCOM,strATCom,csData,csTemp;
	CString info;

	mAT_FirewareName.Format(_T(""));

	CString csAT_Cmd;
	wchar_t wcrData[128];
	int iTimeOut;
	int iError = 0;

	//
	try
	{
		//清空 COM BUFF
		this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

		//-------------------------------------------------- 
		iTimeOut = 1;
		csAT_Cmd.Format( _T( "AT+QDLOAD=1" ) );

// 		//
// 		time( &start );
// 		//
// 		do
// 		{
			//清空 COM BUFF
			this->m_cla_Com_Port.Empty_Com_And_Rx_Buff();

			//发送
			this->m_cla_Com_Port.Write( csAT_Cmd );
			//
// 			memset( wcrData, 0, sizeof( wcrData ) );
// 			//Sleep( 200 );
// 			//读取
// 			this->m_cla_Com_Port.Read( wcrData, &iRec_Bits, 100 );//
// 			//
// 			if(wcsstr(wcrData, _T("OK")))
// 			{
// 				break;//pass, 退出while
// 			}
			//
// 			time( &finish );
// 			//
// 			if( (int)difftime( finish, start ) >= iTimeOut )
// 			{
// 				throw(0);
// 			}
// 		}
// 		while( true );
		//-------------------------
	}
	catch( ... )
	{
		//fail
		return false;
	}
	return true;

}
//-onlyburn:kv.bin
//-onlyburn:RadioConfig.tlv
//-onlyburn:acpu_3rd_sign_ecc.bin
//-onlyeraseall

int CARM_OP_HUAWEI::Do_Execute_HI2120()
{
	int iDl_Res = RTN_FAIL;
	int iQDLoader_Port = 0;
	CString csData;

	try
	{
		this->m_bRuning = true;
		iQDLoader_Port = this->m_iCom_Port;

		//使用burn.exe下载固件包中的usbloader.bin文件
		this->Show_Status(_T("Start download usbloader ......"));
		this->Show_Out(_T("Start download usbloader ......"));

		//
		if (false == this->DL_HiBurn_HI2120(iQDLoader_Port))
		{
			this->Show_Progress(1.0);

			// kill HiBurn.exe
			csData.Format(_T("HiBurn%d.exe"), m_iClass_Idx);
			m_cla_GFC.Kill_Process(csData);

			iDl_Res = RTN_FAIL;
			this->m_bRuning = false;
			return RTN_FAIL;
		}
		else
		{
			this->Show_Progress(1.0);

			this->Show_Status(_T("Download Firwmare Pass"));

			// kill HiBurn.exe
			csData.Format(_T("HiBurn%d.exe"), m_iClass_Idx);
			m_cla_GFC.Kill_Process(csData);
		}
		this->m_bRuning = false;

		return RTN_SUCCESS;
	}
	catch (...)
	{
		iDl_Res = RTN_FAIL;

		this->m_bRuning = false;

	}

	this->m_bRuning = false;

	return iDl_Res;
}
//---------------------------------------------------------
void Wait_DL(LPVOID x)
{
	CARM_OP_HUAWEI *pSv = (CARM_OP_HUAWEI *)x;

	DWORD exitCode = 0;

	WaitForSingleObject(pSv->m_hi_2120_handle, INFINITE);
	pSv->m_hi_2120_wait_flag = true;
}

//-onlyburn:kv.bin
//-onlyburn:RadioConfig.tlv
//-onlyburn:acpu_3rd_sign_ecc.bin
//-onlyeraseall
//---------------------------------------------------------
bool CARM_OP_HUAWEI::DL_HiBurn_HI2120(int  iQDLoader_Port)
{
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpBengin = NULL;
	wchar_t * wcpEnd = NULL;


	wchar_t * wcpHead = NULL;

	char * cpRead_Buff = NULL;
	double dNew_Percent = 0.0;

	time_t t_Start, t_End;
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csInfo;
	int iTemp;
	bool bSuccess_Flag = false;
	int iTimeout = 60;//300秒改为240秒

	double d_pos = 0.0;


	int iBenginPos = -1;
	int iBenginEnd = -1;
	CString strPos;
	CString strProgress;

	int iPos = 0;

	CString strFirmwarePath = m_str_fwpkg_FW.cs_Path;


	try
	{
		//检查下端口有没有出来
		this->Show_Status(_T("Check Port......"));
		this->Save_Trace_Log(_T("Check Port......"));

		if (this->Confirm_Dynamic_Com_Port(iQDLoader_Port, 30) == false)
		{
			this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port:%d"), this->m_iCom_Port);
			this->Show_Status(_T("Check Port Fail"));
			this->Save_Trace_Log(_T("Check Port Fail"));
			throw(0);
		}
		this->Show_Status(_T("Check Port Pass"));
		this->Save_Trace_Log(_T("Check Port Pass"));



		//显示
		this->Show_Status(_T("DL_HiBurn_HI2120......"));
		this->Save_Trace_Log(_T("DL_HiBurn_HI2120......"));

		// kill HiBurn.exe
		csData.Format(_T("HiBurn%d.exe"), m_iClass_Idx);
		m_cla_GFC.Kill_Process(csData);

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			this->m_csError_Code.Format(_T("FAIL, DL_HiBurn_HI2120, Create pipe fail!"));
			this->m_iError_Code = DL_HiBurn_HI2120_Create_Pipe_Fail;
			throw(1);
		}
		//------------------------------ 


		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() * 3 + 1024));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 1024);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 1024);

		//------------------------------ 打包dos指令 ------------------------------
		//HiBurn.exe - com:72 - bin : D : \BC280VAAR01A01_BETA1106.fwpkg - signalbaud:921600
		//swprintf(wcpCmd, _T("cmd.exe %cc %s\\HI2120\\%d\\HiBurn%d.exe -com:%d -bin:%s -signalbaud:921600  -console"), '/', this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);
		
#ifdef FW_MULTI
#ifndef FW_UPGRADE
		//20210420 增加一个冒号
		//swprintf(wcpCmd, _T("%s\\HI2120\\%d\\HiBurn%d.exe -com:%d -erasemode:1 -bin:%s -signalbaud:921600  -console"), this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);
		swprintf(wcpCmd, _T("%s\\HI2120\\%d\\HiBurn%d.exe -com:%d -erasemode:1 -bin:%s -signalbaud:921600  -console"), this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);


#endif
#else
		//swprintf(wcpCmd, _T("%s\\HI2120\\%d\\HiBurn%d.exe -com:%d -bin:%s -signalbaud:921600  -console"), this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);

		//HiBurn.exe - com:113 - bin : C : \Users\w00522586\Desktop\BC280VCNAAR01A01_BETA0118.fwpkg - onlyburn:params_area_ecc_combine.bin - onlyburn : sebl_hisi_sign_enc.bin - onlyburn : semain_hisi_sign_enc.bin - onlyburn : protocol_hisi_sign.bin - onlyburn : acpu_3rd_sign_ecc.bin

		swprintf(wcpCmd, _T("%s\\HI2120\\%d\\HiBurn%d.exe -com:%d -bin:%s -onlyburn:params_area_ecc_combine.bin -onlyburn:sebl_hisi_sign_enc.bin -onlyburn:semain_hisi_sign_enc.bin -onlyburn:protocol_hisi_sign.bin -onlyburn:acpu_3rd_sign_ecc.bin -signalbaud:921600  -console"), this->m_csCurr_Exe_Path, this->m_iClass_Idx, this->m_iClass_Idx, iQDLoader_Port, strFirmwarePath);

#endif
		this->Save_Trace_Log(wcpCmd);
		this->Show_Out(wcpCmd);

		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//创建子进程
		if (CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE)
		{
			this->m_csError_Code.Format(_T("FAIL, DL_HiBurn_HI2120, Create process fail!"));
			this->m_iError_Code = DL_HiBurn_HI2120_Create_Process_Fail;
			throw(2);
		}
		
		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//------------------------------ 
		//------------------------------ 循环读取process的处理信息 ------------------------------ 
		csTotal_Info.Format(_T(""));
		//开始计时
		time(&t_Start);

		//新建一个等待的子线程
		//////////////////////////////////////////////////////////////////////////
		this->m_hi_2120_handle = pi.hProcess;
		this->m_hi_2120_wait_flag = false;
		hWaitThread_Handle = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Wait_DL,
			(void *)this,
			0,
			&dwWait_ID);

		//////////////////////////////////////////////////////////////////////////
		//
		while (bSuccess_Flag == false)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 1024);
			ulActual_Read = 0;
			//------------------------------------------------
			if (true == this->m_hi_2120_wait_flag)
			{
				this->Save_Trace_Log(_T("hi_2120_wait_flag  ......"));
				this->Show_Out(csData);

				bSuccess_Flag = true;
				throw(4);
			}
			//------------------------------------------------

			if (TRUE == ::PeekNamedPipe(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, 0, 0))
			{
				//
				if (ulActual_Read > 0)
				{
					ReadFile(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, NULL);
					//注意：如果端口状态异常的话，程序会吊死在这里。一直无返回。


					//读到info数据了
					if (ulActual_Read > 0)
					{
						//字符转换
						memset(wcpData, 0, 1024);
						this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

						//处理数据
						csData.Format(_T("%s"), wcpData);

						this->Show_Out(csData);
						this->Save_Trace_Log(csData);

						csData.Replace(_T('\r'), _T(' '));
						csData.Replace(_T('\n'), _T(' '));
						csData.MakeLower();

						csTotal_Info += csData;


						//提示模块Reset
						if (wcsstr(csTotal_Info.GetBuffer(), _T("please reset device")) != NULL)
						{
							//显示
							this->Show_Status(_T("Reset ......"));
							this->Save_Trace_Log(_T("Reset ......"));
						}
						//--------------------------------------------
						//progress[0.33%]
						if (csTotal_Info.Find(_T("100.00")) >= 0)
						{
							this->Show_Progress(1.0);
							this->Show_Status(_T("DL BIN Pass"));
							this->Show_Progress(1.0);
							bSuccess_Flag = true;
							Sleep(1000);
							break;
						}


						wcpHead = wcsstr(csTotal_Info.GetBuffer(), _T("progress["));
						if (NULL != wcpHead)
						{
							csTotal_Info.Format(_T("%s"), wcpHead + 9);

							wcpEnd = wcsstr(csTotal_Info.GetBuffer(), _T("%]"));
						
							iPos = csTotal_Info.Find(_T("%]"));
							if (iPos >0)
							{
								strPos = csTotal_Info.Left(iPos);

								if (strPos.Find(_T("100"))>=0)
								{
									this->Show_Progress(1.0);
									this->Show_Status(_T("DL BIN Pass"));
									this->Show_Progress(1.0);
									bSuccess_Flag = true;
									Sleep(1000);
									break;
								}
								else
								{
									strProgress.Format(_T("progress[%s%%]"), strPos);
									//progress[0.57%]
									this->Show_Status(strProgress);
								}
								//字符转数字
								dNew_Percent = _wtof(strPos);

								dNew_Percent = dNew_Percent / 100.0;

								this->Show_Progress(dNew_Percent);

								csTotal_Info.Format(_T("%s"), wcpEnd + 2);
							}

						}
					}//if(ulActual_Read > 0
				}
				else
				{
					//
					//this->Show_Status(_T("Data Waiting..."));
				}
			}
			else
			{
				//
				//this->Show_Status(_T("PeekNamedPipe NULL..."));
			}

			//超时检测
			time(&t_End);
			//
			if ((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				bSuccess_Flag = true;
				this->m_csError_Code.Format(_T("FAIL, DL_HiBurn_HI2120, Time out!"));
				this->m_iError_Code = ERR_DL_USBLOADER_TIMEOUT;
				throw(4);
			}
		}//while(bSuccess_Flag == false
		//------------------------------
		//释放句柄的指针
		if (hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}
		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	}
	catch (...)
	{
		//显示失败信息
		this->Show_Status(this->m_csError_Code);
		this->Save_Trace_Log(m_csError_Code);



		//释放句柄的指针
		if (hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
		}
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		//释放内存
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		//监控线程结束后的返回信息
		DWORD exitCode = 0;

	    if (GetExitCodeProcess(pi.hProcess, &exitCode))
	    {
	      TRACE(_T("Exit code = %d/n"), exitCode);
		  switch (exitCode)
		  {
			case  ERR_TIMEOUT_WAIT_HISILICON://（2）	烧写时选择了错误的COM口（如选择了AT口）或烧写初始阶段未进行复位
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_HISILICON"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_HISILICON;
			break;
			case  ERR_SELOADER_ACK_IS_FAIL://			15
				this->m_csError_Code.Format(_T("ERR_SELOADER_ACK_IS_FAIL"));
				this->m_iError_Code = 0x03000000 | ERR_SELOADER_ACK_IS_FAIL;
			break;
			case  ERR_TIMEOUT_WAIT_BURN_START_C:		//4 // 复位后，设备向HiBurn发送起始信号CCC之前下电
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_BURN_START_C"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_BURN_START_C;
			break;
			case  ERR_TIMEOUT_WAIT_BURN_INIT_ACK:		//5 // 在HiBurn收到初始包的ACK回复之前下电
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_BURN_INIT_ACK"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_BURN_INIT_ACK;
			break;
			case  ERR_TIMEOUT_WAIT_BURN_INTER_ACK:	//6 //在HiBurn收到中间若干包的ACK回复之前下电
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_BURN_INTER_ACK"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_BURN_INTER_ACK;
			break;
			case  ERR_TIMEOUT_WAIT_BURN_EOT_C:		//7 // 在HiBurn收到结束信号C之前下电
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_BURN_EOT_C"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_BURN_EOT_C;
			break;
			case  ERR_TIMEOUT_WAIT_ZERO_ACK:			//8 //在HiBurn收到结束包的ACK之前下电
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_ZERO_ACK"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_ZERO_ACK;
			break;
			case  ERR_TIMEOUT_WAIT_SELOADER_ACK_TIME: // 14 // 在HiBurn收到SELOADER的ACK之前下电
				this->m_csError_Code.Format(_T("ERR_TIMEOUT_WAIT_SELOADER_ACK_TIME"));
				this->m_iError_Code = 0x03000000 | ERR_TIMEOUT_WAIT_SELOADER_ACK_TIME;
			break;
			case  ERR_OPEN_FILE:						//16  //固件包被占用；
				this->m_csError_Code.Format(_T("ERR_OPEN_FILE"));
				this->m_iError_Code = 0x03000000 | ERR_OPEN_FILE;
			break;
			case   ERR_FW_NOT_FIND:					//52
				this->m_csError_Code.Format(_T("ERR_FW_NOT_FIND"));
				this->m_iError_Code = 0x03000000 | ERR_FW_NOT_FIND;
			break;
			  
		  default:
			  
			  this->m_csError_Code.Format(_T("ERR_TIMEOUT_%d"), exitCode);
			  this->m_iError_Code = 0x03000000 | exitCode;
			  break;
		  }

		  this->Save_Trace_Log(this->m_csError_Code);
		  this->Show_Status(this->m_csError_Code);

	    }
	    else
	    {
	      TRACE(_T("GetExitCodeProcess() failed: %ld/n"), GetLastError());
	      ASSERT(0);
	    }

		//
		return false;
	}
	//监控线程结束后的返回信息
	DWORD exitCode = 0;
	if (GetExitCodeProcess(pi.hProcess, &exitCode))
	{
		TRACE(_T("Exit code = %d/n"), exitCode);
	}
	else
	{
		TRACE(_T("GetExitCodeProcess() failed: %ld/n"), GetLastError());
		ASSERT(0);
	}
	//
	return true;
}