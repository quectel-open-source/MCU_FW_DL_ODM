#include "StdAfx.h"
#include "ARM_OP_SILABS.h"
#include "ts_lan.h"


#include "USBDeviceDetection.h"


CARM_OP_SILABS::CARM_OP_SILABS(void)
: m_iCom_Port(0)
,m_bRuning(false)
, m_iStop_Flag(-1)
, m_csError_Code(_T(""))
, m_iClass_Idx(0)
, m_Ts_Num(0)
, m_iOP_TYPE(0)
, m_csCurr_Exe_Path(_T(""))

, mAT_FirewareName(_T(""))
{
	m_iError_Code =0;

	m_PassNumb=0;
	m_ErrorNumb=0;

	mAT_IMEI.Format(_T(""));

	m_iCom_Port = 0;
	m_iSec_Port = 0;

	m_iAT_Port = 0;

}

CARM_OP_SILABS::~CARM_OP_SILABS(void)
{
}
void CARM_OP_SILABS::Show_Status(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

void CARM_OP_SILABS::Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN)
{
	::SendMessage(this->m_hWnd_View, WM_REDIT_INFO_MSG, (WPARAM)wcp_Msg, (LPARAM)b_RN);

}
void CARM_OP_SILABS::Show_Out(const wchar_t * wcp_Status)
{
	::SendMessage(this->m_hWnd_View, WM_OUT_STATUS_MSG, (WPARAM)wcp_Status, (LPARAM)this->m_iClass_Idx);
}

int CARM_OP_SILABS::Do_Execute()
{
	return RTN_SUCCESS;
}
int CARM_OP_SILABS::Do_Execute_MCU(void)
{
	HANDLE hRead_Pipe = NULL;	//指向读端句柄的指针
	HANDLE hWrite_Pipe = NULL;	//指向写端句柄的指针
	SECURITY_ATTRIBUTES sa;//指向安全属性结构的指针
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wchar_t * wcpCmd = NULL;
	wchar_t * wcpData = NULL;
	char * cpRead_Buff = NULL;
	wchar_t * wcpEach = NULL;
	wchar_t * wcpPercent = NULL;
	wchar_t * wcpPercent1 = NULL;


	time_t t_Start, t_End;
	int iTimeout = 400;//没有数据吐出的时间超时  //40
	unsigned long ulActual_Read;
	CString csTotal_Info, csData, csTemp, csEach;
	int iTemp;
	bool bSuccess_Flag = false;
	double dCurr_Percent = 0.0;
	double dNew_Percent = 0.0;


	try
	{
		//显示
		this->Show_Status(_T("Download MCU Fw file ......"));
		this->Save_Trace_Log(_T("Download MCU Fw file ......"));
		this->Show_Out(_T("Download MCU Fw file ..."));


		//------------------------------ 创建 pipe ------------------------------ 
		//Windows中使用较多的管道也是匿名管道，它通过API函数CreatePipe创建
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;//使用系统默认的安全描述符
		sa.bInheritHandle = TRUE;//一定要为TRUE，不然句柄不能被继承

		if (CreatePipe(&hRead_Pipe, &hWrite_Pipe, &sa, 0) == FALSE)
		{
			this->m_csError_Code.Format(_T("FAIL, DL_FW, Create pipe fail!"));
			this->m_iError_Code = DL_FW_MCU_FAIL1;
			throw(1);
		}
		//------------------------------ 


		//分配内存
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpCmd, (this->m_csCurr_Exe_Path.GetLength() + 1024));
		this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpData, 2048);
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, 2048);


		//------------------------------ 打包dos指令 ------------------------------
		swprintf(wcpCmd, _T("cmd.exe %cc %s\\Jlink\\%d\\JLink.exe  \"%s\\Jlink\\%d\\eraseAndFlash.jlink\""),
			'/', this->m_csCurr_Exe_Path, m_iClass_Idx, this->m_csCurr_Exe_Path, m_iClass_Idx);

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
			this->m_csError_Code.Format(_T("FAIL, DL_FW, Create process fail!"));
			this->m_iError_Code = DL_FW_MCU_FAIL2;
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
		//
		while (bSuccess_Flag == false)
		{
			//读pipe管道
			memset(cpRead_Buff, 0, 2048);
			ulActual_Read = 0;
			//
			ReadFile(hRead_Pipe, cpRead_Buff, 1024, &ulActual_Read, NULL);
			//


			//读到info数据了
			if (ulActual_Read > 0)
			{
				//字符转换
				memset(wcpData, 0, 2048);
				this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpData, &iTemp);

				//处理数据
				csData.Format(_T("%s"), wcpData);

				this->Save_Trace_Log(csData);

				csData.Replace(_T('\r'), _T(' '));
				csData.Replace(_T('\n'), _T(' '));

				csData.MakeLower();

				csTotal_Info += csData;

				//是否有“Script file read successfully 开始”关键词
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("script file read successfully"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.1;
					csTemp.Format(_T("script file ok"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.1);

					csTemp.Format(_T("script file read successfully"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);

				}

				//是否有“Connecting to J-Link via USB...O.K”关键词
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("connecting to j-link via usb...o.k"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.2;
					csTemp.Format(_T("connecting usb ok"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.2);

					csTemp.Format(_T("connecting to j-link via usb...o.k"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}
				//Erasing device
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("erasing device"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.3;
					csTemp.Format(_T("erasing..."));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.3);

					csTemp.Format(_T("erasing device"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}
				//Erasing done
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("erasing done"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.5;
					csTemp.Format(_T("erasing ok"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.5);

					csTemp.Format(_T("erasing done"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}
				//Downloading file
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("downloading file"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.6;
					csTemp.Format(_T("downloading file..."));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.6);

					csTemp.Format(_T("downloading file"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}

				//error...
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("error"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download error"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.9);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, error!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, error!"));
					this->m_iError_Code = DL_FW_MCU_FAIL4;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//退出while
				}

				//fail...
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("fail"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download fail"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.9);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, fail!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, fail!"));
					this->m_iError_Code = DL_FW_MCU_FAIL4;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//退出while
				}

				//Failed to open file
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("failed to open file"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.7;
					csTemp.Format(_T("failed to open file!"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.7);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, Failed to open file!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, Failed to open file!"));
					this->m_iError_Code = DL_FW_MCU_FAIL4;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//退出while
				}


				//Total time needed    OK
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("total time needed"));
				//
				if ((wcpPercent != NULL) && (NULL != wcsstr(csTotal_Info.GetBuffer(), _T("ok"))))
				{
					dCurr_Percent = 0.8;
					csTemp.Format(_T("download file ok"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.8);

					csTemp.Format(_T("total time needed"));

					wcpPercent += csTemp.GetLength();

					csTotal_Info.Format(_T("%s"), wcpPercent);
				}

				//Script processing completed
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("script processing completed"));

				wcpPercent1 = wcsstr(csTotal_Info.GetBuffer(), _T("o.k."));
				//
				if ((wcpPercent != NULL) && (wcpPercent1 != NULL))
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download ok"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.9);

					csTemp.Format(_T("script processing completed"));

					bSuccess_Flag = true;//pass
					break;//退出while

				}
				//cannot...
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("cannot"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download fail"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.9);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, cannot link!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, cannot link!"));
					this->m_iError_Code = DL_FW_MCU_FAIL3;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//退出while

				}
				//Unknown command line option
				wcpPercent = wcsstr(csTotal_Info.GetBuffer(), _T("unknown"));
				//
				if (wcpPercent != NULL)
				{
					dCurr_Percent = 0.9;
					csTemp.Format(_T("download fail"));
					this->Show_Status(csTemp);
					//显示进度条
					this->Show_Progress(0.9);

					this->m_csError_Code.Format(_T("FAIL, DL_FW, Unknown command line option!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, Unknown command line option!"));
					this->m_iError_Code = DL_FW_MCU_FAIL4;
					throw(4);

					bSuccess_Flag = false;//fail
					break;//退出while

				}
				//超时检测
				time(&t_End);
				//
				if ((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
				{
					this->m_csError_Code.Format(_T("FAIL, DL_FW, Time out!"));
					this->Save_Trace_Log(_T("FAIL, DL_FW, Time out!"));
					this->m_iError_Code = DL_FW_MCU_FAIL5;
					throw(4);
				}
			}

		}//while(bSuccess_Flag == false
		//------------------------------


		//释放句柄的指针
		if (hRead_Pipe != NULL)
		{
			CloseHandle(hRead_Pipe);
			hRead_Pipe = NULL;
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

		this->Show_Progress(1.0);
		//
		return RTN_FAIL;
	}

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

	this->Show_Progress(1.0);
	//
	return RTN_SUCCESS;
}
void CARM_OP_SILABS::Show_Progress(double d_Pgr_Percent)
{
	//此函数中应该包含进度条百分比的计算，然后在消息响应函数中完成具体步进点数的计算
	//Progress总长度是100
	::SendMessage(this->m_hWnd_View, WM_PROGRESS_ENTIRE_MSG, (WPARAM)(d_Pgr_Percent * 100.0), (LPARAM)this->m_iClass_Idx);
}


void CARM_OP_SILABS::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)this->m_iClass_Idx);
}

bool CARM_OP_SILABS::FindHexFile(CString lpPath)
{
	CString strSubPath, strNewPath, cs_Name;

	//WIN32_FIND_DATA结构描述了一个由FindFirstFile, 
	//FindFirstFileEx, 或FindNextFile函数查找到的文件信息
	WIN32_FIND_DATA FindFileData;

	//通过FindFirstFile()函数根据当前的文件存放路径
	//查找该文件来把待操作文件的相关属性读取到WIN32_FIND_DATA结构中去
	strNewPath.Format(_T("%s*.*"), lpPath);
	HANDLE hFind = ::FindFirstFile(strNewPath, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return false;  //如果hFind的句柄值无效，返回0
	}

	while (TRUE)
	{
		//将dwFileAttributes和FILE_ATTRIBUTE_DIRECTORY做位"与"运算来判断所找到的项目是不是文件夹，
		//这段程序的目的是查找文件夹下子文件夹中的内容
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				strSubPath.Format(_T("%s%s\\"), lpPath, FindFileData.cFileName);
				if (NULL == wcsstr(strSubPath, _T("DBG")))
				{
					FindHexFile(strSubPath);
				}	
			}
		}
		else
		{
			//rail_empty.hex
			cs_Name.Format(_T("%s"), FindFileData.cFileName);
			//
			if ((NULL != wcsstr(cs_Name,_T("_rail_")))  
				&& (NULL != wcsstr(cs_Name, _T(".hex"))))
			{
				m_FWInfo.cs_Name.Format(_T("%s"), cs_Name);

				m_FWInfo.cs_Path.Format(_T("%s%s"), lpPath, cs_Name);

				m_FWInfo.b_Enable_DL = true;

				break;
			}
		}
		if (!FindNextFile(hFind, &FindFileData))
			break;//如果没有找到下一个文件，结束本次循环
	}
	FindClose(hFind);

	return true;
}
bool CARM_OP_SILABS::LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path)
{
	CString cs_Finder_Dir, cs_FW_Name, cs_FW_Path, cs_FW_Dir;
	CString strData;
	CFileFind cla_Finder; //文件查找类
	BOOL b_Found;

	try
	{
		//--------------------------------------- 开始解析信息，载入ZG100S下载文件 --------------------------------------- 
		//清空ZG100S下载用文件列表
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

				// 判定是hex文件
				if (wcsstr(cs_FW_Name, _T(".HEX")) != NULL)
				{
					//判定RAIL文件  rail_empty.hex 根据客户需求改这里关键字
					//if (wcsstr(cs_FW_Name, _T("RAIL")) != NULL)//20220422
					{
						m_FWInfo.strHexFile.Format(_T("%s\\%s"), cs_FW_Dir, cla_Finder.GetFileName());

						m_FWInfo.b_Enable_DL = true;
					}
				}

			}
		}
		cla_Finder.Close();

		if (m_FWInfo.strHexFile.IsEmpty())
		{
			strData = _T(".hex file");
			m_FWInfo.b_Enable_DL = false;
		}

		if (m_FWInfo.b_Enable_DL == false)
		{
			this->m_csError_Code.Format(_T("error,load_fw_file,There is no %s.file!"), strData);
			throw(0);
		}
#if 1

		CStdioFile Cmd_File;
		CString   strCmd_file_name, csData;
		unsigned long ul_File_Size;

		strCmd_file_name.Format(_T("%s\\Jlink\\%d\\eraseAndFlash.jlink"), this->m_csCurr_Exe_Path, m_iClass_Idx);

		if (this->Get_File_Size(strCmd_file_name, &ul_File_Size) == true)
		{
			::SetFileAttributes(strCmd_file_name, FILE_ATTRIBUTE_NORMAL);

			::DeleteFile(strCmd_file_name);

			Sleep(2000);
		}

		if (Cmd_File.Open(strCmd_file_name, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate | CFile::typeText) != NULL)
		{


			Cmd_File.SeekToEnd();

			csData.Format(_T("device EFR32BG21BXXXF1024\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("usb 0\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("si 1\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("speed 9600\n"));
			Cmd_File.WriteString(csData);

			//csData.Format(_T("erase\n"));
			//Cmd_File.WriteString(csData);

			//csData.Format(_T("sleep 400\n"));
			//Cmd_File.WriteString(csData);


			//csData.Format(_T("erase\n"));
			//Cmd_File.WriteString(csData);

			csData.Format(_T("sleep 400\n"));
			Cmd_File.WriteString(csData);


			csData.Format(_T("loadfile \"%s\"\n"), this->m_FWInfo.strHexFile);
			Cmd_File.WriteString(csData);
			this->Show_Out(this->m_FWInfo.strHexFile);
			//-----------------------------------------------
			csData.Format(_T("sleep 400\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("loadfile \"%s\"\n"), this->m_FWInfo.strHexFile);
			Cmd_File.WriteString(csData);
			this->Show_Out(this->m_FWInfo.strHexFile);

			csData.Format(_T("sleep 100\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("r\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("g\n"));
			Cmd_File.WriteString(csData);

			csData.Format(_T("q\n"));
			Cmd_File.WriteString(csData);


			Cmd_File.Close();

			this->Show_Out(_T("CMD File Save OK"));

		}
		else
		{
			this->m_csError_Code.Format(_T("CMD File(%s) Open Fail!"), strCmd_file_name);
			this->Show_Out(this->m_csError_Code);
			//throw(1);

		}

#endif


	}
	catch (...)
	{
		this->Show_Status(this->m_csError_Code);
		return false;
	}

	return true;
}
bool CARM_OP_SILABS::Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size)
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
//bool CARM_OP_SILABS::Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut)
//{
//	time_t t_Start, t_End;
//	CString csA, csB;
//	int j = 0;
//
//	try
//	{
//		csB.Format(_T(" ."));
//
//		time(&t_Start);
//		csA.Format(_T("Wait port(%d) is normal..."), i_Tgt_Com_Port);
//		this->Show_Status(csA);
//		this->Show_Out(csA);
//		Save_Trace_Log(csA);
//
//		do
//		{
//			//读出regedit中的COM列表
//			if (this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == true)
//			{
//				//找到串口了
//				//等待5秒后继续检查一遍
//				Sleep(5000);
//				if (this->m_cla_GFC.Confirm_Reg_Com_Port(i_Tgt_Com_Port) == false)
//				{
//					//继续					
//				}
//				else
//				{
//					this->Show_Status(_T("Get Port Pass"));
//					this->Show_Out(_T("Get Port Pass"));
//					Save_Trace_Log(_T("Get Port Pass"));
//					return true;
//				}
//
//			}
//
//			//系统暂停
//			if (this->m_iStop_Flag > 0)
//			{
//				this->m_csError_Code.Format(_T("Operation has been stopped!"));
//				throw(0);
//			}
//
//			//检查是否超时
//			time(&t_End);
//			if ((int)difftime(t_End, t_Start) >= i_TimeOut)
//			{
//				this->m_csError_Code.Format(_T("FAIL, Confirm dynamic com port %d timeout!"), i_Tgt_Com_Port);
//				//this->m_iError_Code  = Confirm_Dynamic_Com_Port_fail1;
//				throw(0);
//			}
//
//			//进度简易显示
//			if ((j % 10) == 0)
//			{
//				csA.Format(_T("Detect dynamic com port %d"), i_Tgt_Com_Port);
//			}
//
//			csA += csB;
//			this->Show_Status(csA);
//			j++;
//
//			//每次读取注册表的延迟
//			Sleep(800);
//		} while (true);
//	}
//	catch (...)
//	{
//		this->Show_Status(m_csError_Code);
//		this->Show_Out(m_csError_Code);
//		Save_Trace_Log(m_csError_Code);
//		return false;
//	}
//
//	return false;
//}
