#include "StdAfx.h"
#include "ARM_OP_BEKEN.h"
#include "ts_lan.h"



CARM_OP_BEKEN::CARM_OP_BEKEN(void)
	: m_iCom_Port(0)
	, m_bRuning(false)
	, m_iStop_Flag(-1)
	, m_csError_Code(_T(""))
	, m_iClass_Idx(0)
	, m_csCurr_Exe_Path(_T(""))
	, m_iResetFlag(0)
	, m_iProdu_Type(0)
{
	m_iOP_Robot = 0;
	m_Ts_Num = 0;
	m_iTimeInterval = 0;
}

void CARM_OP_BEKEN::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_BEKEN::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_BEKEN::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_BEKEN::Show_Progress(double d_Pgr_Percent)
{
	//此函数中应该包含进度条百分比的计算，然后在消息响应函数中完成具体步进点数的计算
	//Progress总长度是100
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 100.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_BEKEN::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

bool CARM_OP_BEKEN::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CString strData;
	CFileFind cla_Finder; //文件查找类
	BOOL b_Found;

	try
	{
		//--------------------------------------- 开始解析信息，载入BEKEN下载文件 --------------------------------------- 
		//清空EBKEN下载用文件列表
		this->m_FWInfo.Clear();

		//提取软件路径
		if (this->m_cla_GFC.Get_File_Directory(wcp_FW_File_Path, &cs_FW_Dir) == false)
		{
			this->m_csError_Code.Format(_T("ERROR, Get fls file directory fail!"));
			throw(0);
		}

		//寻找文件开始
		cs_Finder_Dir.Format(_T("%s\\*"), cs_FW_Dir);

		b_Found = cla_Finder.FindFile(cs_Finder_Dir);

		while (b_Found)
		{
			b_Found = cla_Finder.FindNextFile();

			if (cla_Finder.IsDots())
			{
				continue;//找到的是当前目录或上级目录则跳过
			}

			if (!cla_Finder.IsDirectory())	//如果不是文件夹，便是文件
			{
				cs_FW_Name.Format(_T("%s"), cla_Finder.GetFileName());//获取找到的文件名

				cs_FW_Name.MakeUpper();

				// 判定是bin文件
				if (wcsstr(cs_FW_Name, _T(".BIN")) != NULL)
				{
					//判定FC41DAAR01A01.BIN文件
					if (wcsstr(cs_FW_Name, _T("FC41D")) != NULL)
					{
						m_FWInfo.strBootLoader.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

						m_FWInfo.b_Enable_DL = true;
					}
				

				}

			}
		}//while(b_Found

		//
		cla_Finder.Close();

		if (m_FWInfo.strBootLoader.IsEmpty())
		{
			strData = _T("bootloader bin file");
			m_FWInfo.b_Enable_DL = false;
		}

		if (m_FWInfo.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no %s.file!"), strData);
			throw(0);
		}

	}
	catch (...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	return true;
}
int CARM_OP_BEKEN::Do_Execute(void)
{
	CString strData,strInfo;
	try
	{
		m_bRuning = true;

		this->m_csError_Code.Format(_T(""));
		this->m_iError_Code = 0;

	
		strData.Format(_T("bk_writer%d.exe"), m_iClass_Idx);
		m_cla_GFC.Kill_Process(strData);

		//20210521
		strInfo.Format(_T("Port(%d) find..."), this->m_iCom_Port);
		this->Show_Out(strInfo);
		if (this->Confirm_Dynamic_Com_Port(this->m_iCom_Port, 35) == false)//
		{
			strInfo.Format(_T("Port(%d) find Fail"), this->m_iCom_Port);
			this->Show_Out(strInfo);
			this->m_csError_Code = strInfo;
			this->m_iError_Code = ERR_DL_FIND_PORT;
			throw(1);
		}

		if (this->DL(strData) == false)
		{
			throw(1);
		}

	}
	catch (...)
	{
		m_bRuning = false;
		return RTN_FAIL;
	}

	m_bRuning = false;
	return RTN_SUCCESS;
}

bool CARM_OP_BEKEN::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
{
	time_t t_Start, t_End;
	CString csA, csB;
	int j = 0;

	try
	{
		csB.Format(_T(" ."));

		time(&t_Start);
		csA.Format(_T("Wait port(%d) is normal..."), i_Tgt_Com_Port);
		this->Show_Status(csA);
		this->Show_Out(csA);
		Save_Trace_Log(csA);

		do
		{
			//读出regedit中的COM列表
			if (this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == true)
			{
				//找到串口了
				//等待5秒后继续检查一遍
				Sleep(2000);
				if (this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == false)
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
			if (this->m_iStop_Flag > 0)
			{
				this->m_csError_Code.Format(_T("Operation has been stopped!"));
				throw(0);
			}

			//检查是否超时
			time(&t_End);
			if ((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port %d timeout!"), i_Tgt_Com_Port);
				throw(0);
			}

			//进度简易显示
			if ((j % 10) == 0)
			{
				csA.Format(_T("Detect dynamic com port %d"), i_Tgt_Com_Port);
			}

			csA += csB;
			this->Show_Status(csA);
			j++;

			//每次读取注册表的延迟
			Sleep(800);
		} while (true);
	}
	catch (...)
	{
		this->Show_Status(m_csError_Code);
		this->Show_Out(m_csError_Code);
		Save_Trace_Log(m_csError_Code);
		return false;
	}

	return false;
}
//自动线程控电源上电
void CARM_OP_BEKEN::Set_PowerOn_SPC(int i_Index)
{
	this->Open_Power_SPC(i_Index);
	//::SendMessage(this->m_hWnd_View, WM_PowerOn_SPC, (WPARAM)i_Index, (LPARAM)i_Index);
}
//每片模块下载前都必须给每个通道的程控电源上电
void  CARM_OP_BEKEN::Open_Power_SPC(int iIndex)
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
bool CARM_OP_BEKEN::DL(const CString& strCmdExe)
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
	int iTimeout = 120;//延时5s
	int iTemp = 0;
	int nPos = 0;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;

	try
	{
		//显示
		this->Show_Status(_T("DL Start......"));
		this->Save_Trace_Log(_T("DL Start ......"));

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL Start, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 4095);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 4095);
		//////////////////////////////////////////////////////////////////////////
		//bk_writer.20210322.exe -tBK7231N -pCOM223@115200 -fF:\06-Factory_Tools\FC41D\exe\FC41DAAR01A01.bin@0 -e2 -R
		swprintf_s(wcpCmd, nLen, _T("cmd.exe  %cc  \"%s\\Beken\\%d\\%s -tBK7231N -pCOM%d@%d  -f%s@0 -e2 -R\""), '/',
			m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe, m_iCom_Port, m_iBaud_Rate, m_FWInfo.strBootLoader);
		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本跟当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//创建子进程
		if (CreateProcess(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL Start, Create process fail");
			throw(0);
		}

		this->Save_Trace_Log(wcpCmd);

		//把管道的写句柄先关闭，这样ReadFile函数就不会阻塞。
		if (hWrite_Pipe != NULL)
		{
			CloseHandle(hWrite_Pipe);
			hWrite_Pipe = NULL;
		}

		strData.Empty();
		//开始计时
		time(&t_Start);
		this->Show_Status(_T("Erasing Flash ..."));
		while (bSuccess_Flag == FALSE)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 4095);
			ulActual_Read = 0;
			ReadFile(hRead_Pipe, cpRead_Buff, 2048, &ulActual_Read, NULL);

			//读到info数据了
			if (ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 4095);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);
				this->Show_Out(wcpData);

				//处理数据
				strInfo.Format(_T("%s"), wcpData);
				//提取重要的“进度百分比信息”
				//提取第一个值(0)  Writing Flash
				wcpEach = wcstok(wcpData, _T("\r\n"));
			
				while (wcpEach != NULL)
				{
					wcpPercent = wcsstr(wcpEach, _T("Writing Flash"));

					if (wcpPercent != NULL)
					{
						//备份单条数据
						strEach.Format(_T("%s"), wcpPercent);

						//提取百分比数据
					    wcpPercent += strlen("Erasing Flash ... ");

						//是否有“percent 结束”关键词
						if (wcsstr(wcpPercent, _T("%")) != NULL)
						{
							//发现百分比数据的“结尾”
							strData.Format(_T("%s"), wcpPercent);
							strData.Replace('%', '\0');//去掉结尾
							//字符转数字
							dNew_Percent = _wtof(strData);

							//刷新百分比数据
							if (dNew_Percent > dCurr_Percent)
							{
								//赋值新百分比数据
								dCurr_Percent = dNew_Percent;

								//显示“单条信息数据”
//								this->Show_Out(strEach);
								this->Show_Status(strEach);
								//无延迟，则看不出显示信息的变化
								Sleep(20);
								//显示进度条
								this->Show_Progress((1.0 * (dCurr_Percent / 100.0)));
							}
						}
					}
					wcpEach = wcstok(NULL, _T("\r\n"));
				}
			}

			this->Save_Trace_Log(strInfo);


			//汇总数据
			if ((dCurr_Percent > 90.0)
				|| (dCurr_Percent < 1.0))//
			{
				csTotal_Info += strInfo;
			}
			else
			{
				csTotal_Info.Format(_T("%s"), strInfo);
			}

			//检查是否有错误报出
			if (wcsstr(strInfo, _T("Writing Flash Failed")) != NULL)
			{
				nPos = strInfo.Find(_T("Writing Flash Failed"));
				strData = strInfo.Right(strInfo.GetLength() - nPos);
				strData.Replace('-', ' ');//去掉不想显示的字符
				if (!strData.IsEmpty())
				{
					this->m_csError_Code.Format(_T("Fail,%s"), strData);
				}
				throw(0);
			}

			if (wcsstr(strInfo, _T("Failed")) != NULL)
			{
				this->m_csError_Code.Format(_T("FAIL, DL_Probe fail!"));
				this->m_iError_Code = ERR_DL_PROBE_TIME_FAIL;
				throw(0);
			}


			//检查是否pass  //Writing Flash OK
			if (wcsstr(strInfo, _T("Writing Flash OK")) != NULL)
			{
				bSuccess_Flag = TRUE;//pass
				break;//退出while
			}

			//超时检测
			time(&t_End);
			//
			if ((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
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

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);

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

		this->Save_Trace_Log(m_csError_Code);
		this->Show_Status(m_csError_Code);
		return false;
	}
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpCmd);
	m_cla_GFC.BUFF_WCHAR_FREE(&wcpData);
	m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

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

	this->Show_Progress(1.0);
	return true;
}
bool CARM_OP_BEKEN::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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

		if (hFile_Handle == INVALID_HANDLE_VALUE)
		{
			throw(0);
		}

		//获取文件的BYTE数
		*ulp_Size = ::GetFileSize(hFile_Handle, NULL);

		//关闭
		::CloseHandle(hFile_Handle);
		hFile_Handle = INVALID_HANDLE_VALUE;
	}
	catch (...)
	{
		return false;
	}

	return true;
}

CARM_OP_BEKEN::~CARM_OP_BEKEN()
{
}
