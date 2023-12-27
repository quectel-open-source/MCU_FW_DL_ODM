#include "StdAfx.h"
#include "ARM_OP_EIGEN.h"
#include "ts_lan.h"



CARM_OP_EIGEN::CARM_OP_EIGEN(void)
: m_iCom_Port(0)
, m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_csCurr_Exe_Path(_T(""))
, m_iResetFlag(0)
, m_iProdu_Type(0)
, m_csSrcIni_File(_T(""))
, m_iBBchip_Curr(0)
{
	m_iOP_Robot = 0;
	m_Ts_Num = 0;
	m_iTimeInterval = 0;
}


CARM_OP_EIGEN::~CARM_OP_EIGEN(void)
{


}
void CARM_OP_EIGEN::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_EIGEN::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_EIGEN::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_EIGEN::Show_Progress(double d_Pgr_Percent)
{
	//此函数中应该包含进度条百分比的计算，然后在消息响应函数中完成具体步进点数的计算
	//Progress总长度是1000
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 1000.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_EIGEN::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}


bool CARM_OP_EIGEN::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CString strData;
	CFileFind cla_Finder; //文件查找类
	BOOL b_Found;

	try
	{
		//--------------------------------------- 开始解析信息，载入EIGEN下载文件 --------------------------------------- 
		//清空EIGEN下载用文件列表
		this->m_FWInfo.Clear();

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

				//20200806 flash_download.ini
				if (wcsstr(cs_FW_Name, _T(".INI")) != NULL)
				{
					m_csSrcIni_File.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());
	
				}

				

				// 判定是bin文件
				if (wcsstr(cs_FW_Name, _T(".BIN")) != NULL)
				{
					//判定AGENTBOOT.BIN文件
					if(wcsstr(cs_FW_Name, _T("AGENTBOOT.BIN")) != NULL)
					{
						m_FWInfo.strAgentBoot.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

						m_FWInfo.b_Enable_DL = true;
					}
					if (wcsstr(cs_FW_Name, _T("_HEAD")) == NULL)
					{
						//判定bootloader.bin文件
						if((wcsstr(cs_FW_Name, _T("BOOTLOADER_BC260")) != NULL)
							||(wcsstr(cs_FW_Name, _T("BOOTLOADER_BC950FCN")) != NULL)
							||(wcsstr(cs_FW_Name, _T("BOOTLOADER_BC660K")) != NULL)
							||(wcsstr(cs_FW_Name, _T("BOOTLOADER_BC95GF")) != NULL)
							||(wcsstr(cs_FW_Name, _T("BOOTLOADER_BC28F")) != NULL)
							)

						{
							m_FWInfo.strBootLoader.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

							m_FWInfo.b_Enable_DL = true;
						}
						else //判定system文件
						{
							if((wcsstr(cs_FW_Name, _T("BC260")) != NULL)
								||(wcsstr(cs_FW_Name, _T("BC950")) != NULL)
								||(wcsstr(cs_FW_Name, _T("BC660K")) != NULL)
								||(wcsstr(cs_FW_Name, _T("BC95GF")) != NULL)
								||(wcsstr(cs_FW_Name, _T("BC28F")) != NULL)
								)
							{
								m_FWInfo.strSystem.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

								m_FWInfo.b_Enable_DL = true;
							}
						}
					}
#ifdef FW_FACTORY
					if (PRODU_TYPE_MOB != this->m_iProdu_Type)
					{
						//判定MergeRfTable.bin文件
						if(wcsstr(cs_FW_Name, _T("MERGERFTABLE")) != NULL)
						{
							m_FWInfo.strRF.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

							m_FWInfo.b_Enable_DL = true;
						}
					}
					else
					{
						//...
					}
#endif
				}

			}
		}//while(b_Found

		//
		cla_Finder.Close();
		//

		// save parameters in config.ini
		this->Save_Config();

		if (m_FWInfo.strBootLoader.IsEmpty())
		{
			strData = _T("bootloader bin file");
			m_FWInfo.b_Enable_DL = false;
		}
		else if (m_FWInfo.strSystem.IsEmpty())
		{
			strData = _T("system bin file");
			m_FWInfo.b_Enable_DL = false;
		}
#ifdef FW_FACTORY
		else 
		{
			if ((PRODU_TYPE_MOB != this->m_iProdu_Type) && (m_FWInfo.strRF.IsEmpty()))
			{
				strData = _T("RF bin file");
				m_FWInfo.b_Enable_DL = false;
			}
		}
#endif
		if(m_FWInfo.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no %s.file!"), strData);
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

int CARM_OP_EIGEN::Do_Execute(void)
{
	CString strData;
	try
	{
		m_bRuning = true;

		this->m_csError_Code.Format(_T(""));
		this->m_iError_Code = 0;

		// kill flashtoolCLI.exe

		strData.Format(_T("flashtoolcli%d.exe"), m_iClass_Idx);
		m_cla_GFC.Kill_Process(strData);

		//比如像QFLASH的端口在主界面的情形需要再更新下配置文件
#ifndef FW_MULTI	
		// save parameters in config.ini
		this->Save_Config();
#endif
	

		// 1. 探测连接
		if(this->DL_Probe(strData) == false)
		{
			throw(1);
		}

#ifdef FW_FACTORY
		if (PRODU_TYPE_MOB != this->m_iProdu_Type)
		{
			// 2. 全擦
			if(this->DL_EraseAll(strData) == false)
			{
				throw(1);
			}
		}
#endif

		//3. 下载固件Download the boot loader and system
		if(this->DL_Bootloader_System(strData) == false)
		{
			throw(3);
		}

#ifdef FW_FACTORY
		if (PRODU_TYPE_MOB != this->m_iProdu_Type)
		{
			// 4. 下载RF Download the flex file 
			if (this->DL_FlexFile(1, strData) == false)
			{
				throw(4);
			}
		
			if (this->DL_FlexFile(2, strData) == false)
	 		{
	 			throw(4);
	 		}
		}
#endif

#ifndef FW_FACTORY
		// 5. 擦除NV区域
		if(this->DL_EraseNV(strData) == false)
		{
			throw(1);
		}
// 		//6. 升级完重启
// 		if(this->DL_Reset(strData) == false)
// 		{
// 			throw(1);
// 		}


#endif


	}
	catch (...)
	{
		m_bRuning = false;
		return RTN_FAIL;
	}

	m_bRuning = false;
	return RTN_SUCCESS;
}
//自动线程控电源上电
void CARM_OP_EIGEN::Set_PowerOn_SPC(int i_Index)
{
	this->Open_Power_SPC(i_Index);
	//::SendMessage(this->m_hWnd_View, WM_PowerOn_SPC, (WPARAM)i_Index, (LPARAM)i_Index);
}
//每片模块下载前都必须给每个通道的程控电源上电
void  CARM_OP_EIGEN::Open_Power_SPC(int iIndex)
{
#ifdef	FW_MULTI
#ifndef FW_UPGRADE

	switch (iIndex)
	{
	case 1:
	{
		//打开电源
		this->m_cla_PS_1->Power_Off(1);

		this->Show_Out(_T("----Power_Off----"));

		Sleep(2000);

		//上电
		//程控电源上电
		if (this->m_cla_PS_1->m_bCtrl_PS == true)
		{
			this->m_cla_PS_1->POWER_ON_V1(1);

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
		if (this->m_cla_PS_2->m_bCtrl_PS == true)
		{
			this->m_cla_PS_2->POWER_ON_V1(1);

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
		if (this->m_cla_PS_3->m_bCtrl_PS == true)
		{
			this->m_cla_PS_3->POWER_ON_V1(1);

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
		if (this->m_cla_PS_4->m_bCtrl_PS == true)
		{
			this->m_cla_PS_4->POWER_ON_V1(1);

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
bool CARM_OP_EIGEN::DL_Probe(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 15;//延时5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//显示
		this->Show_Status(_T("Probe ......"));
		this->Save_Trace_Log(_T("Probe ......"));

		int iTimeInterval = m_iTimeInterval;

		if (0 == iTimeInterval)
		{
			iTimeInterval = 4000;//
		}

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_Probe, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --cfgfile config.ini --port="COM39" probe
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --cfgfile %s\\EFlashTool\\%d\\config.ini  --port=\"COM%d\" probe \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe,m_csCurr_Exe_Path, m_iClass_Idx, m_iCom_Port);

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
			m_csError_Code = _T("FAIL, DL_Probe, Create process fail");
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);

		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//开始计时
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//处理数据
				strInfo.Format(_T("%s"), wcpData);

				this->Show_Out(strInfo);
				this->Save_Trace_Log(strInfo);
			}
			//提示模块Reset
			if(wcsstr(strInfo, _T("BootPreemptDet start")) != NULL)//20201213
			{
				//显示
				this->Show_Status(_T("Reset ......"));
				this->Save_Trace_Log(_T("Reset ......"));

				if (LOAD_AUTO_ROBOT == m_iOP_Robot) //m_iOP_Robot =0为手动模式下载
				{		
					this->Save_Trace_Log(_T("DC Reset AUTO_ROBOT......"));

					//向总控PC发送DC重启指令
					//bDC_Reset = ::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC( m_Ts_Num, _T("REON") );
					::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC(m_Ts_Num, _T("OFF"));
					Sleep(iTimeInterval);
					::TS_LAN_SEND_DC_LAN_CMD_TO_MC_PC(m_Ts_Num, _T("ON"));
				}
				else  if (LOAD_AUTO_ROBOT_SPC == m_iOP_Robot) //全自动SPC
				{
					this->Save_Trace_Log(_T("DC Reset ROBOT_SPC......"));

					this->Set_PowerOn_SPC(this->m_iClass_Idx);
				}
				else
				{
					//重新上下电
					//...
				}


			}
			//检查是否有错误报出
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//去掉不想显示的字符
				strData.Replace(']', ' ');//去掉不想显示的字符
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Probe fail!"));
				this->m_iError_Code = ERR_DL_PROBE_TIME_FAIL;
				throw(0);
			}


			//检查是否pass
			if(wcsstr(strInfo, _T("Burn OK")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//退出while
			}

			//超时检测
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				m_csError_Code = _T("FAIL, DL_Probe Time out!");
				this->m_iError_Code = ERR_DL_PROBE_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);
		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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

	return true;
}

bool CARM_OP_EIGEN::DL_EraseAll(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 30;//延时5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//显示
		this->Show_Status(_T("erase flash ......"));
		this->Save_Trace_Log(_T("erase flash ......"));

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_EraseAll, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --skipconnect 1 --cfgfile config.ini --port="COM39" flasherase 0x0 0x400000
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1  --cfgfile %s\\EFlashTool\\%d\\config.ini --port=\"COM%d\" flasherase 0x0 0x400000 \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe,m_csCurr_Exe_Path, m_iClass_Idx, m_iCom_Port);

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
			m_csError_Code = _T("FAIL, DL_EraseAll, Create process fail");
			throw(0);
		}
		this->Save_Trace_Log(wcpCmd);

		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//开始计时
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//处理数据
				strInfo.Format(_T("%s"), wcpData);
// 				strInfo.Replace(_T('\r'), _T(' '));
// 				strInfo.Replace(_T('\n'), _T(' '));
				this->Show_Out(strInfo);
				this->Save_Trace_Log(strInfo);
				//无延迟，则看不出显示信息的变化
			}
			//检查是否有错误报出
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//去掉不想显示的字符
				strData.Replace(']', ' ');//去掉不想显示的字符
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("Burn Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_EraseAll fail!"));
				this->m_iError_Code = ERR_DL_ERASEALL_FAIL;
				throw(3);
			}


			//检查是否pass
			if(wcsstr(strInfo, _T("Erase OK")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//退出while
			}

			//超时检测
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				m_csError_Code = _T("FAIL, DL_EraseAll Time out!");
				this->m_iError_Code = ERR_DL_ERASEALL_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);

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

		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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

	return true;
}

bool CARM_OP_EIGEN::DL_Bootloader_System(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpNextToken = NULL;
	wchar_t * wcpPercent = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo, strEach, strBinFile, csTotal_Info;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 60;//延时5s
	int iTemp = 0;
	int nPos = 0;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;

	try
	{
		//显示
		this->Show_Status(_T("Download bootloader and system file ......"));
		this->Save_Trace_Log(_T("Download bootloader and system file ......"));

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --skipconnect 1 --cfgfile config.ini --port="COM39" burn
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1 --cfgfile %s\\EFlashTool\\%d\\config.ini --port=\"COM%d\" burn \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe,m_csCurr_Exe_Path, m_iClass_Idx, m_iCom_Port);

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
			m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM,, Create process fail");
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);

		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//开始计时
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//处理数据
				strInfo.Format(_T("%s"), wcpData);

				//提取重要的“进度百分比信息”
				//提取第一个值(0)
				wcpEach = wcstok_s(wcpData, _T("\r\n"), &wcpNextToken);
				//
				while(wcpEach != NULL)
				{
					//是否有“percent 开始”关键词
					strBinFile.Format(_T("{%s-percent files transferred"), m_strBootloaderFile);
					wcpPercent = wcsstr(wcpEach, strBinFile);
					if (wcpPercent == NULL)
					{
						strBinFile.Format(_T("{%s-percent files transferred"),m_strSystemFile);
						wcpPercent = wcsstr(wcpEach, strBinFile);
					}
					//
					if(wcpPercent != NULL)
					{
						//备份单条数据
						strEach.Format(_T("%s"), wcpPercent);

						//提取百分比数据
						wcpPercent += strBinFile.GetLength();

						//是否有“percent 结束”关键词
						if(wcsstr(wcpPercent, _T("%")) != NULL)
						{
							//发现百分比数据的“结尾”
							strData.Format(_T("%s"), wcpPercent);
							strData.Replace('%', '\0');//去掉结尾

							//字符转数字
							dNew_Percent = _wtof(strData);

							//刷新百分比数据
							if(dNew_Percent > dCurr_Percent)
							{
								//赋值新百分比数据
								dCurr_Percent = dNew_Percent;

 
 								//显示“单条信息数据”
								this->Show_Out(strInfo);
								//无延迟，则看不出显示信息的变化
//								Sleep(20);


								//显示进度条
								if(wcsstr(strInfo, m_strBootloaderFile) != NULL)
								{
#ifdef FW_FACTORY
									this->Show_Progress( (0.1 * dCurr_Percent / 100.0) );
#else
									this->Show_Progress( (0.2 * dCurr_Percent / 100.0) );
#endif
									strData.Format(_T("DL_BOOTLOADER_SYSTEM(%.0f%%), %s"), dCurr_Percent, m_strBootloaderFile);
									this->Show_Status(strData);
									if (wcsstr(strInfo, _T("100")) != NULL)
									{
										dCurr_Percent = 0.0;
									}
								}
								else if (wcsstr(strInfo, m_strSystemFile) != NULL)
								{
#ifdef FW_FACTORY
									this->Show_Progress( (0.1 + 0.85 * dCurr_Percent / 100.0) );
#else
									this->Show_Progress( (0.2 + 0.8 * dCurr_Percent / 100.0) );
#endif
									strData.Format(_T("DL_BOOTLOADER_SYSTEM(%.0f%%), %s"), dCurr_Percent, m_strSystemFile);
									this->Show_Status(strData);
								}
							}
						}//if(wcsstr(wcpPercent, _T("%")) != NULL
					}//if(wcpPercent != NULL

					//提取下一个值
					wcpEach = wcstok_s(NULL, _T("\r\n"), &wcpNextToken);
				}//while(wcpEach != NULL
 			}
			
			this->Save_Trace_Log(strInfo);


			//汇总数据
			if ((dCurr_Percent > 90.0)
				|| (dCurr_Percent<1.0))//
			{
				csTotal_Info += strInfo;
			}
			else
			{
				csTotal_Info.Format(_T("%s"), strInfo);
			}

			//检查是否有错误报出
			if (wcsstr(csTotal_Info, _T("[Error Info]")) != NULL)
			{
				nPos = csTotal_Info.Find(_T("[Error Info]"));

				strInfo = csTotal_Info.Right(csTotal_Info.GetLength() - nPos);

				strData = strInfo.Right(strInfo.GetLength()- 13);

				strData.Replace('[', ' ');//去掉不想显示的字符
				strData.Replace(']', ' ');//去掉不想显示的字符
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_FAIL_EXT;
				throw(0);
			}

			if (wcsstr(csTotal_Info, _T("Burn Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_BOOTLOADER_SYSTEM fail!"));
				this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_FAIL;
				throw(3);
			}


			//检查是否pass
			if (wcsstr(csTotal_Info, _T("Burn OK")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//退出while
			}

			//超时检测
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM Time out!");
				this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);

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

		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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

	return true;
}
bool CARM_OP_EIGEN::DL_FlexFile(int iIndex, const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 10;//延时5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//显示
		strData.Format(_T("Download flexfile%d file"), iIndex-1);
		this->Show_Status(strData);
		this->Save_Trace_Log(strData);

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code.Format(_T("FAIL, DL_FlexFile%d, Create pipe fail!"), iIndex-1);
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1 --cfgfile %s\\EFlashTool\\%d\\config.ini --port=\"COM%d\" burnone flexfile%d \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe, m_csCurr_Exe_Path, m_iClass_Idx,m_iCom_Port, iIndex-1);

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
			m_csError_Code.Format(_T("FAIL, DL_FlexFile%d, Create process fail!"), iIndex-1);
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);

		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//开始计时
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//处理数据
				strInfo.Format(_T("%s"), wcpData);
				this->Save_Trace_Log(strInfo);
			}
			//检查是否有错误报出
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//去掉不想显示的字符
				strData.Replace(']', ' ');//去掉不想显示的字符
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("Burn Failed")) != NULL)
			{
				m_csError_Code.Format(_T("FAIL, DL_FlexFile%d fail!"), iIndex-1);
				this->m_iError_Code = ERR_DL_FLEX_FAIL;
				throw(0);
			}

			//检查是否pass
			if(wcsstr(strInfo, _T("Burn OK")) != NULL)
			{
				if (iIndex == 1)
				{
					this->Show_Progress(1.0);
				}
				else
				{
					this->Show_Progress(1.0);
				}
				bSuccess_Flag = TRUE;//pass
				break;//退出while
			}

			//超时检测
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				m_csError_Code.Format(_T("FAIL, DL_FlexFile%d Time out!"), iIndex-1);
				this->m_iError_Code = ERR_DL_FLEX_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);

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



		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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


	return true;

}
void CARM_OP_EIGEN::Save_Config()
{
	CString strData, strPort;
	int nPos;
	//20200806 先增加固件包中的flash_download.ini拷贝到config.ini
	//1.删除工具目录下的老的配置文件////////////////////////////////////////////////////////////////////////////////
	BOOL bCopy = FALSE;

	unsigned long ul_File_Size;

	if(this->Get_File_Size(m_cla_ConfigFile.m_csIniFileName, &ul_File_Size) == true)
	{
		::SetFileAttributes(m_cla_ConfigFile.m_csIniFileName, FILE_ATTRIBUTE_NORMAL);  

		::DeleteFile(m_cla_ConfigFile.m_csIniFileName); 

		Sleep(2000);
	}
	//2.将固件包中的INI配置文件复制到工具目录下////////////////////////////////////////////////////////////////////////////////

	CStdioFile cla_File_Dest;
	CString csData_Buff1;

	if (cla_File_Dest.Open(m_cla_ConfigFile.m_csIniFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) != NULL)
	{
		cla_File_Dest.SeekToBegin();
		//[config]
		csData_Buff1 = _T("[config]\r\n");
		cla_File_Dest.WriteString(csData_Buff1);
		//line_0_com = COM29
		strData.Format(_T("line_%d_com = COM%d\r\n"), m_iClass_Idx - 1, m_iCom_Port);
		csData_Buff1 = strData;
		cla_File_Dest.WriteString(csData_Buff1);
		//agbaud = 921600
		csData_Buff1.Format(_T("agbaud = %d\r\n"), m_iBaud_Rate);
		cla_File_Dest.WriteString(csData_Buff1);
		//;bootloader.bin file infomation
		csData_Buff1.Format(_T(";bootloader.bin file infomation\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//[bootloader]
		csData_Buff1.Format(_T("[bootloader]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//blpath = .\image_ec616s\bootloader.bin		
		csData_Buff1.Format(_T("blpath = %s\r\n"), m_FWInfo.strBootLoader);
		cla_File_Dest.WriteString(csData_Buff1);
		//blloadskip = 0
		csData_Buff1.Format(_T("blloadskip = 0\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//;system.bin file infomation
		csData_Buff1.Format(_T(";system.bin file infomation\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//[system]
		csData_Buff1.Format(_T("[system]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//syspath = .\image_ec616s\app - demo - flash.bin
		csData_Buff1.Format(_T("syspath = %s\r\n"), m_FWInfo.strSystem);
		cla_File_Dest.WriteString(csData_Buff1);
		//sysloadskip = 0
		csData_Buff1.Format(_T("sysloadskip = 0\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//;control such as reset before download
		csData_Buff1.Format(_T(";control such as reset before download\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//[control]
		csData_Buff1.Format(_T("[control]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
//#ifdef FW_FACTORY
		if (BB_EC617 == m_iBBchip_Curr)
		{
			//detect = 2
			//csData_Buff1.Format(_T("detect = 2\r\n"));
			csData_Buff1.Format(_T("detect = 1\r\n"));
			cla_File_Dest.WriteString(csData_Buff1);

			csData_Buff1.Format(_T("atreset = at+ecrst=delay,500\r\n"));
			cla_File_Dest.WriteString(csData_Buff1);
			
			//#else
			//		//detect = 1
			//		csData_Buff1.Format(_T("detect = 1\r\n"));
			//		cla_File_Dest.WriteString(csData_Buff1);
			//		//atreset = at + ecrst = delay, 200
			//		csData_Buff1.Format(_T("atreset=at+ecrst=delay,200\r\n"));
			//		cla_File_Dest.WriteString(csData_Buff1);
			//#endif

			//prempt_detect_time = 6
			csData_Buff1.Format(_T("prempt_detect_time = 6\r\n"));
			cla_File_Dest.WriteString(csData_Buff1);
		}
		//msg_waittime = 2
		csData_Buff1.Format(_T("msg_waittime = 2\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//max_preamble_cnt = 8
		csData_Buff1.Format(_T("max_preamble_cnt = 8\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//lpc_recover_en = 0
		csData_Buff1.Format(_T("lpc_recover_en = 0\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//; cfg reset to 2, skip reset pin
		csData_Buff1.Format(_T(";cfg reset to 2, skip reset pin\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);

		if (BB_EC617 == m_iBBchip_Curr)
		{
			//pullup_qspi = 1-------EC616S
			csData_Buff1.Format(_T("pullup_qspi = 1\r\n"));
			cla_File_Dest.WriteString(csData_Buff1);
		}

		//[flexfile0]
		csData_Buff1.Format(_T("[flexfile0]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//filepath = .\rfCaliTb_ec616s\MergeRfTable.bin
		csData_Buff1.Format(_T("filepath = %s\r\n"), m_FWInfo.strRF);
		cla_File_Dest.WriteString(csData_Buff1);
		//burnaddr = 0x3A4000
		csData_Buff1.Format(_T("burnaddr = 0x3A4000\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//[flexfile1]
		csData_Buff1.Format(_T("[flexfile1]\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);
		//filepath = .\rfCaliTb_ec616s\MergeRfTable.bin
		csData_Buff1.Format(_T("filepath = %s\r\n"), m_FWInfo.strRF);
		cla_File_Dest.WriteString(csData_Buff1);
		//burnaddr = 0x16000
		csData_Buff1.Format(_T("burnaddr = 0x16000\r\n"));
		cla_File_Dest.WriteString(csData_Buff1);

		cla_File_Dest.Close();

	}
	else
	{
		//
		strData.Format(_T("%s Open Fail"), m_cla_ConfigFile.m_csIniFileName);
		Show_Out(strData);

	}


	//3.根据实际固件文件路径修改工具包下的配置文件////////////////////////////////////////////////////////////////////////////////
	nPos = m_FWInfo.strBootLoader.ReverseFind('\\');
	if (nPos != -1)
	{
		m_strBootloaderFile = m_FWInfo.strBootLoader.Right(m_FWInfo.strBootLoader.GetLength() - nPos - 1);
	}
	nPos = m_FWInfo.strSystem.ReverseFind('\\');
	if (nPos != -1)
	{
		m_strSystemFile = m_FWInfo.strSystem.Right(m_FWInfo.strSystem.GetLength() - nPos - 1);
	}	
}

bool CARM_OP_EIGEN::DL_EraseNV(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 30;//延时5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//显示
		this->Show_Status(_T("erase flash NV......"));
		this->Save_Trace_Log(_T("erase flash NV......"));

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_EraseNV, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --port="COM39" flasherase 0x350000  0x5400
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1 --cfgfile %s\\EFlashTool\\%d\\config.ini --port=\"COM%d\" flasherase 0x350000  0x54000 \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe,m_csCurr_Exe_Path, m_iClass_Idx, m_iCom_Port);

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
			m_csError_Code = _T("FAIL, DL_EraseNV, Create process fail");
			throw(0);
		}
		this->Save_Trace_Log(wcpCmd);

		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//开始计时
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//处理数据
				strInfo.Format(_T("%s"), wcpData);
				// 				strInfo.Replace(_T('\r'), _T(' '));
				// 				strInfo.Replace(_T('\n'), _T(' '));
				this->Show_Out(strInfo);
				this->Save_Trace_Log(strInfo);
				//无延迟，则看不出显示信息的变化
			}
			//检查是否有错误报出
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//去掉不想显示的字符
				strData.Replace(']', ' ');//去掉不想显示的字符
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("Burn Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_EraseNV fail!"));
				throw(3);
			}


			//检查是否pass
			if(wcsstr(strInfo, _T("Erase success")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//退出while
			}

			//超时检测
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				m_csError_Code = _T("FAIL, DL_EraseNV Time out!");
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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


		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);
		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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


	return true;
}

bool CARM_OP_EIGEN::DL_Reset(const CString& strCmdExe)
{
	BOOL bRet = TRUE;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo;
	time_t t_Start, t_End;
	BOOL bSuccess_Flag = FALSE;
	int iTimeout = 30;//延时5s
	int iTemp = 0;
	int nPos = 0;

	try
	{
		//显示
		this->Show_Status(_T("DL_Reset......"));
		this->Save_Trace_Log(_T("DL_Reset....."));

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if(CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE) 
		{
			m_csError_Code = _T("FAIL, DL_Reset, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, MAX_PATH*2);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, MAX_PATH*2);
		//////////////////////////////////////////////////////////////////////////
		// FlashToolCLI.exe --port="COM39" flasherase 0x350000  0x5400
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc \"%s\\EFlashTool\\%d\\%s --skipconnect 1 --cfgfile %s\\EFlashTool\\%d\\config.ini  --port=\"COM%d\" sysreset \""), '/',m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe, m_csCurr_Exe_Path, m_iClass_Idx,m_iCom_Port);

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
			m_csError_Code = _T("FAIL, DL_Reset, Create process fail");
			throw(0);
		}
		this->Save_Trace_Log(wcpCmd);

		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if(hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//开始计时
		time(&t_Start);
		while(bSuccess_Flag == FALSE)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 512);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 256, &ulActual_Read, NULL);


			//读到info数据了
			if(ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 512);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//处理数据
				strInfo.Format(_T("%s"), wcpData);
				// 				strInfo.Replace(_T('\r'), _T(' '));
				// 				strInfo.Replace(_T('\n'), _T(' '));
				this->Show_Out(strInfo);
				this->Save_Trace_Log(strInfo);
				//无延迟，则看不出显示信息的变化
			}
			//检查是否有错误报出
			if(wcsstr(strInfo, _T("[Error Info]")) != NULL)
			{
				nPos = strInfo.Find(_T("[Error Info]"));
				strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
				strData.Replace('[', ' ');//去掉不想显示的字符
				strData.Replace(']', ' ');//去掉不想显示的字符
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if(wcsstr(strInfo, _T("Burn Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Reset fail!"));
				this->m_iError_Code = ERR_DL_RESET_FAIL;

				throw(3);
			}


			//检查是否pass
			if(wcsstr(strInfo, _T("Erase success")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//退出while
			}

			//超时检测
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				m_csError_Code = _T("FAIL, DL_Reset Time out!");
				this->m_iError_Code = ERR_DL_RESET_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
		m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
		m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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


		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);
		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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


	return true;
}

bool CARM_OP_EIGEN::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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
