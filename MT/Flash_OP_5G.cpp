#include "StdAfx.h"
#include "Flash_OP_5G.h"


CFlash_OP_5G::CFlash_OP_5G(void)
{
	m_iCom_Port = 0;
	m_iBaud_Rate = 0;

	m_bRuning = false;
	m_iClass_Idx = 0;
	m_csError_Code.Format(_T(""));
	m_iError_Code = 0;
	m_bPLC_Flag = false;
	m_iPLC_Port = 0;
	m_iComMethod = 0;
	m_iResetFlag = 0;
	m_iClearNVFlag = 0;
	m_bSaveDebugLog = false;
}

CFlash_OP_5G::~CFlash_OP_5G(void)
{
	//

}
//cb Show_State
static void __stdcall cb_show_state_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP_5G * pSv = (CFlash_OP_5G *)p_usr_Arg;

	pSv->Show_Status(wcp_State);
}
static void __stdcall cb_show_out_r(const wchar_t * wcp_State, void * p_usr_Arg)
{
	CFlash_OP_5G * pSv = (CFlash_OP_5G *)p_usr_Arg;

	pSv->Show_Out(wcp_State);
}

void CFlash_OP_5G::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP_5G::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}
void CFlash_OP_5G::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

void CFlash_OP_5G::Show_Progress(int i_Pgr_Percent)
{
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)i_Pgr_Percent, (LPARAM)this->m_iClass_Idx);
}

int CFlash_OP_5G::Do_Execute()
{
	CString strData;
	try
	{
		m_bRuning = true;

		this->m_csError_Code.Format(_T(""));
		this->m_iError_Code = 0;

		// kill flashtoolCLI.exe

		strData.Format(_T("ThinModemFlashTool%d.exe"), m_iClass_Idx);
		m_cla_GFC.Kill_Process(strData);


		//1. 下载固件Download the boot loader and system
		if (this->DL_Bootloader_System(strData) == false)
		{
			throw(3);
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

//MT6880_openwrt_scatter.xml
bool CFlash_OP_5G::Load_Scatter_Xml_Data(const wchar_t * wcp_Xml_File, const wchar_t * wcp_Dir_Path)
{
	CStdioFile cla_XML_File;
	CString csData, csrBuff_List1[8], csrBuff_List2[8], csFW_Image, csFW_Name;
	int iQuan1, iQuan2, i,j, iIndex(0);
	try
	{
		for (j = 0; j<MAX_BIN_FILE; j++)
		{
			this->m_strr_Bin_File_List[j].cs_Name.Format(_T(""));
			this->m_strr_Bin_File_List[j].cs_Path.Format(_T(""));
			this->m_strr_Bin_File_List[j].b_Enable_DL = false;
		}

		//打开 MT6880_openwrt_scatter.xml 文件
		if (cla_XML_File.Open(wcp_Xml_File, (CFile::modeRead | CFile::modeNoTruncate | CFile::typeText)) == NULL)
		{
			//打开目标文件失败
			this->Show_Out(wcp_Xml_File);
			this->m_csError_Code.Format(_T("ERROR, Load_Scatter_Xml_Data, Open xml file fail!"));
			::AfxMessageBox(this->m_csError_Code);
			throw(0);
		}

		//定位到头
		cla_XML_File.SeekToBegin();


		//
		csFW_Image.Format(_T(""));
		csFW_Name.Format(_T(""));


		CString csData_partition_name, csData_file_name;
		iIndex = 0;
		bool bSucess = false;

		//循环读取数据
		while (cla_XML_File.ReadString(csData_partition_name) != NULL)
		{
			//<partition_name>sspm_1< / partition_name>
			//<file_name>sspm.img< / file_name>

			//先判定是否有关键词
			if (wcsstr(csData_partition_name, _T("<partition_name>")) == NULL)
			{
				//无关键词
				continue;
			}
			//继续读一行
			if (cla_XML_File.ReadString(csData_file_name) != NULL)
			{
				//先判定是否有关键词
				if (wcsstr(csData_file_name, _T("<file_name>")) == NULL)
				{
					//无关键词
					continue;
				}
			}

			//分段提取
			this->m_cla_GFC.WCSTOK_STRING_T_C(csData_partition_name, _T("<>"), csrBuff_List1, &iQuan1);

			//
			if (iQuan1 >= 8)
			{
				this->m_csError_Code.Format(_T("ERROR, Load_Scatter_Xml_Data, Buff overload fail!"));
				::AfxMessageBox(this->m_csError_Code);
				throw(0);
			}

			//分段提取
			this->m_cla_GFC.WCSTOK_STRING_T_C(csData_file_name, _T("<>"), csrBuff_List2, &iQuan2);

			//找关键词的“开始”
			for (i = 0; i<iQuan1; i++)
			{
				if (wcsstr(csrBuff_List1[i], _T("partition_name")) != NULL)
				{
					//image
					csFW_Image.Format(_T("%s"), csrBuff_List1[i + 1]);
					//成功提取image，退出循环
					for (j = 0; j < iQuan2; j++)
					{
						if (wcsstr(csrBuff_List2[i], _T("file_name")) != NULL)
						{
							csFW_Name.Format(_T("%s"), csrBuff_List2[i + 1]);
							bSucess = true;
							break;
						}
					}
					//
					if (true == bSucess)
					{
						break;
					}
					
				}
			}//for(i=0; i<iQuan; i++

			//
			if (i >= iQuan1)
			{
				this->m_csError_Code.Format(_T("ERROR, Load_Scatter_Xml_Data, Decode data fail!"));
				::AfxMessageBox(this->m_csError_Code);
				throw(0);
			}

			//判断两个关键词是否都有数据了
			if (((csFW_Image.GetLength() > 0) && (csFW_Name.GetLength() > 0))
				&& (csFW_Name.Find(_T("NONE")) == -1))
			{
				this->m_strr_Bin_File_List[iIndex].cs_Name.Format(_T("%s"), csFW_Image);
				this->m_strr_Bin_File_List[iIndex].cs_Path.Format(_T("%s\\%s"), wcp_Dir_Path, csFW_Name);
				this->m_strr_Bin_File_List[iIndex].b_Enable_DL = true;
				iIndex++;
				//清空
				csFW_Image.Format(_T(""));
				csFW_Name.Format(_T(""));
			}
		}//while(cla_XML_File.ReadString(csData) != NULL
		//关闭 xml 文件
		cla_XML_File.Close();
	}
	catch (...)
	{
		//关闭 xml 文件
		cla_XML_File.Close();

		//
		return false;
	}

	return true;
}
//.....
#if 0
//cb Show_REdit_Info
static void __stdcall cb_show_redit_info_r( const wchar_t * wcp_Msg, void * p_usr_Arg )
{
	//WIN32
	wprintf( _T( "[REDIT_INFO]%s\n" ), wcp_Msg );
	//因为管道缓冲区还没有满，不会立即刷新。在有些情况下，程序根本不会退出，除非发生错误，这种情况下几乎就永远不能及时得到输出了。
	//调用fflush强制刷新输出。
	//对标准输出流的清理，但是它并不是把数据丢掉，而是及时地打印数据到屏幕上。
	fflush( stdout );
}
//setvbuf(stdout, NULL, _IONBF, 0);// 防止打印堵塞



#endif
bool CFlash_OP_5G::DL_Bootloader_System(const CString& strCmdExe)
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
	const  wchar_t * wcpHead = NULL;
	char * cpRead_Buff = NULL;
	unsigned long ulActual_Read;
	int nLen = 0;
	CString strData, strInfo, strSubInfo,strEach, strBinFile;
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

		SECURITY_DESCRIPTOR secutityDese;
		InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&secutityDese, TRUE, NULL, FALSE);

		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM, Create pipe fail!");
			throw(0);
		}
		//------------------------------ 

		nLen = m_csCurr_Exe_Path.GetLength() + MAX_PATH+1024;
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, nLen);
		m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 1024);
		m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		//////////////////////////////////////////////////////////////////////////
		// ThinModemFlashTool1.exe -c format-download -f flash.xml
		// ThinModemFlashTool1.exe -c fireware-upgrade -f flash.xml
		// ThinModemFlashTool1.exe -c download -f flash.xml
//#ifdef FW_MULTI
		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc %s\\MTK\\%d\\%s -c format-download -f \"%s\" -r"), '/', m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe, m_csFlash_Path);
//#else
//		swprintf_s(wcpCmd, nLen, _T("cmd.exe %cc %s\\MTK\\%d\\%s -c fireware-upgrade -f \"%s\" -r"), '/', m_csCurr_Exe_Path, m_iClass_Idx, strCmdExe, m_csFlash_Path);
//#endif
		//------------------------------ 创建 process ------------------------------ 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si);//这个函数用来取得当前进程的StartupInfo,我们新建的进程基本根当前进程的StartupInfo差不多
		si.hStdError = hWrite_Pipe;//错误输出的句柄
		si.hStdOutput = hWrite_Pipe;//标准输出的句柄
		si.wShowWindow = SW_HIDE;//制定了新进程创建时窗口的现实状态，这个属性设 SW_HIDE 隐藏新建的DOS进程
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//用来制定 STARTUPINFO里这一堆参数那个有效的
		//创建子进程
		if (CreateProcessW(NULL, wcpCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == FALSE)
		{
			m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM, Create process fail");
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
		while (bSuccess_Flag == FALSE)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 1024);
			ulActual_Read = 0;
			//
			if (NULL == hRead_Pipe)
			{
				ulActual_Read = 0;
			}

			if (TRUE == ::PeekNamedPipe(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, 0, 0))
			{
				ReadFile(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, NULL);
			}
			
			//读到info数据了
			if (ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 1024);
				m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//处理数据
				strSubInfo.Format(_T("%s"), wcpData);
				this->Save_Trace_Log(strSubInfo);
				strInfo += strSubInfo;

				//提示模块Reset
				wcpHead = wcsstr(strInfo, _T("scan device START"));
				if (wcpHead != NULL)
				{
					//显示
					this->Show_Progress((10));

					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("scan device START"))));

					this->Show_Status(_T("Reset ......"));
					this->Save_Trace_Log(_T("Reset ......"));
				}
				//1,scan device succeed
				wcpHead = wcsstr(strInfo, _T("scan device succeed"));
				if (wcpHead != NULL)
				{
					//显示
					this->Show_Progress((20));
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("scan device succeed"))));

					this->Show_Status(_T("scan device succeed"));
					//this->Save_Trace_Log(_T("scan device succeed"));
				}
				//2,connect brom START
				wcpHead = wcsstr(strInfo, _T("connect brom START"));
				if (wcpHead != NULL)
				{
					//显示
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("connect brom START"))));
					this->Show_Status(_T("connect brom START"));
					//this->Save_Trace_Log(_T("connect brom START"));
				}
				
				//connect brom successed
				wcpHead = wcsstr(strInfo, _T("connect brom successed"));
				if (wcpHead != NULL)
				{
					//显示
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("connect brom successed"))));
					this->Show_Status(_T("connect brom successed"));
					//this->Save_Trace_Log(_T("connect brom successed"));
				}
				//enter DA mode
				wcpHead = wcsstr(strInfo, _T("DA data..."));
				if (wcpHead != NULL)
				{
					//显示
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("DA data..."))));
					this->Show_Status(_T("DA data..."));
					//this->Save_Trace_Log(_T("DA data..."));
				}
				//enter DA mode successed
				wcpHead = wcsstr(strInfo, _T("enter DA mode successed"));
				if (wcpHead != NULL)
				{
					//显示
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("enter DA mode successed"))));
					this->Show_Progress((30));
					this->Show_Status(_T("enter DA mode successed"));
					//this->Save_Trace_Log(_T("enter DA mode successed"));
				}
				//flash has been formatted
				wcpHead = wcsstr(strInfo, _T("flash has been formatted"));
				if (wcpHead != NULL)
				{
					//显示
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("flash has been formatted"))));
					this->Show_Status(_T("flash has been formatted"));
					//this->Save_Trace_Log(_T("flash has been formatted"));
				}
				//Erase
				wcpHead = wcsstr(strInfo, _T("Erase"));
				if (wcsstr(strInfo, _T("Erase")) != NULL)
				{
					//显示
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("Erase"))));
					this->Show_Status(_T("Erase..."));
					//this->Save_Trace_Log(_T("Erase..."));
				}
				//[rootfs
				wcpHead = wcsstr(strInfo, _T("rootfs"));
				if (wcpHead != NULL)
				{
					//显示
					strInfo.Format(_T("%s"), (wcpHead + sizeof(_T("rootfs"))));
					this->Show_Progress((60));
					this->Show_Status(_T("rootfs..."));
					//this->Save_Trace_Log(_T("rootfs..."));
				}
				//All command exec done
				//检查是否pass
				if (wcsstr(strInfo, _T("All command exec done")) != NULL)
				{
					this->Save_Trace_Log(_T("All command exec done"));
					this->Show_Progress((100));
					bSuccess_Flag = TRUE;//pass
					break;//退出while
				}
				
				//检查是否有错误报出
				if (wcsstr(strInfo, _T("[Error Info]")) != NULL)
				{
					nPos = strInfo.Find(_T("[Error Info]"));
					strData = strInfo.Right(strInfo.GetLength() - nPos - 13);
					strData.Replace('[', ' ');//去掉不想显示的字符
					strData.Replace(']', ' ');//去掉不想显示的字符
					if (!strData.IsEmpty())
					{
						this->m_csError_Code.Format(_T("Fail,%s"), strData);
					}
					//this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_FAIL_EXT;
					throw(0);
				}

				if (wcsstr(strInfo, _T("Burn Failed")) != NULL)
				{
					this->m_csError_Code.Format(_T("FAIL, DL_BOOTLOADER_SYSTEM fail!"));
					//this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_FAIL;
					throw(3);
				}


				//检查是否pass
				if (wcsstr(strInfo, _T("FLASH-ALL command execute successed")) != NULL)
				{
					this->Show_Progress((90));
					this->Show_Status(_T("FLASH-ALL command execute successed,wait..."));
				}

			}
			//超时检测
			time(&t_End);
			//
			if ((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				this->Show_Progress((100));

				m_csError_Code = _T("FAIL, DL_BOOTLOADER_SYSTEM Time out!");
				//this->m_iError_Code = ERR_DL_BOOTLOADER_SYSTEM_TIME_OUT;
				throw(0);
			}
		}

	}
	catch (...)
	{
		this->Show_Progress((100));

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

		return false;
	}

	this->Show_Progress((100));

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

	return true;
}









