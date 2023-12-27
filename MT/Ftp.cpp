#include "stdafx.h"
#include "Ftp.h"

CStringW CFtp::CStrA2CStrW(const CStringA &cstrSrcA)
{
	if (cstrSrcA.GetLength() <= 0)
	{
		return L"";
	}
	int len = MultiByteToWideChar(CP_UTF8, 0, LPCSTR(cstrSrcA), -1, NULL, 0);
	wchar_t *wstr = new wchar_t[len];
	memset(wstr, 0, len*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, LPCSTR(cstrSrcA), -1, wstr, len);
	CStringW cstrDestW = wstr;
	delete[] wstr;
	wstr = NULL;
	return cstrDestW;
}

CFtp::CFtp()
{
	p_InternetSession = NULL;
	p_FtpConnection = NULL;
	m_b_FTP_Connect = false;
}


CFtp::~CFtp()
{
	DisConnect_FTP_Server();
}
/********************************************************************************************
void CFtp::DisConnect_FTP_Server()
Description:
断开FTP连接
Parameters:
Returns:
********************************************************************************************/
void CFtp::DisConnect_FTP_Server()
{
	try
	{
		//断开FTP
		if (p_FtpConnection != NULL)
		{
			p_FtpConnection->Close();
			//delete
			delete p_FtpConnection;
			p_FtpConnection = NULL;
		}
		if (p_InternetSession != NULL)
		{
			delete p_InternetSession;
			p_InternetSession = NULL;
		}
	}
	catch (CInternetException * pEx)//error:can not connect to specific ftp
	{
		pEx->Delete();
	}
	m_b_FTP_Connect = false;
}
/********************************************************************************************
bool CFtp::Connect_FTP_Server(CString sServer, CString sUserName, CString sPassWord, int iPort)
Description:
建立FTP连接
Parameters:
1.sServer:服务器地址
2.sUserName:用户名
3.sPassWord:密码
4.iPort:Ftp端口
Returns:
********************************************************************************************/
bool CFtp::Connect_FTP_Server()
{
	try
	{
		//创建一个CInternetSession对象
		if (p_InternetSession != NULL)
		{
			DisConnect_FTP_Server();	
		}
		p_InternetSession = new CInternetSession();
		//创建CFtpConnection对象并返回一个指向该对象的指针
		p_FtpConnection = p_InternetSession->GetFtpConnection(
			m_FtpInfo.sServerName, m_FtpInfo.sUserName, m_FtpInfo.sPassword, m_FtpInfo.iPort, true);
	}
	catch (CInternetException * pEx)//error:can not connect to specific ftp
	{
		pEx->Delete();
		//
		DisConnect_FTP_Server();
	}
	if (p_FtpConnection == NULL)
	{
		return false;
	}
	m_b_FTP_Connect = true;
	return true;
}
/********************************************************************************************
bool CFtp::Create_FTP_Path(CString sPath)
Description:
创建FTP路径
Parameters:
1.sPath:FTP路径
Returns:
********************************************************************************************/
bool CFtp::Create_FTP_Path(CString sFTPPath, CString &sError)
{
	int nDirIndex = -1;
	CString sDirectory = L"//", sPath = sFTPPath;
	if (sPath.GetLength()<3 || sPath.Left(2)!=L"//")
	{
		return false;
	}
	if (!Set_FTP_Path(sPath))
	{
		if (sPath.Right(1) != L"/")
		{
			sPath += L"/";
		}
		sPath = sPath.Right(sPath.GetLength() - 2);
		for (int i = 2;; i++)
		{
			nDirIndex = sPath.Find(L"/");
			if (nDirIndex < 0)
			{
				break;
			}
			sDirectory += sPath.Left(nDirIndex + 1);
			if (!Set_FTP_Path(sDirectory))
			{
				if (!p_FtpConnection->CreateDirectory(sDirectory))
				{
					sError.Format(L"CreateDirectory:(%s)Fail", sDirectory);
					return false;
				}
			}
			sPath = sPath.Right(sPath.GetLength() - nDirIndex - 1);
		}
	}
	return true;
}
/********************************************************************************************
bool CFtp::Delete_FTP_Path(CString sPath)
Description:
删除FTP路径
Parameters:
1.sPath:FTP路径
Returns:
********************************************************************************************/
bool CFtp::Delete_FTP_Path(CString sPath)
{
	if (!p_FtpConnection->Remove(sPath))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Get_Current_FTP_Path(CString& sPath)
Description:
获取当前FTP所要工作的路径
Parameters:
1.sPath:FTP工作路径
Returns:
********************************************************************************************/
bool CFtp::Get_Current_FTP_Path(CString& sPath)
{
	if (!p_FtpConnection->GetCurrentDirectory(sPath))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Set_FTP_Path(CString& sPath)
Description:
设置当前FTP所要工作的路径
Parameters:
1.sPath:FTP工作路径
Returns:
********************************************************************************************/
bool CFtp::Set_FTP_Path(CString sPath)
{
	if (!p_FtpConnection->SetCurrentDirectory(sPath))
	{
		Connect_FTP_Server();
		if (!p_FtpConnection->SetCurrentDirectory(sPath))
		{
			return false;
		}
	}
	return true;
}
/********************************************************************************************
bool CFtp::Upload_File(CString sLocalFilePath, CString sFtpName)
Description:
上传本地文件到FTP服务器，路径为Set_FTP_Path，所设置的路径
Parameters:
1.sLocalFilePath:本地文件全路径名
2.sFtpName:服务器文件名
Returns:
********************************************************************************************/
bool CFtp::Upload_File(CString sLocalFilePath, CString sFtpFileName)
{
	if (!p_FtpConnection->PutFile(sLocalFilePath, sFtpFileName))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Download_File(CString sLocalFilePath, CString sFtpName)
Description:
从FTP服务器下载文件到本地，路径为Set_FTP_Path，所设置的路径
Parameters:
1.sFtpFileName:服务器文件名
2.sLocalFilePath:本地文件全路径名
Returns:
********************************************************************************************/
bool CFtp::Download_File(CString sFtpFileName, CString sLocalFilePath)
{
	if (!p_FtpConnection->GetFile(sFtpFileName, sLocalFilePath, FALSE, FILE_ATTRIBUTE_NORMAL, INTERNET_FLAG_RELOAD, 1))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool CFtp::Delete_File(CString sFileName)
Description:
删除文件
Parameters:
1.sFtpName:服务器文件名
Returns:
********************************************************************************************/
bool CFtp::Delete_File(CString sFtpFileName)
{
	if (!p_FtpConnection->Remove(sFtpFileName))
	{
		return false;
	}
	return true;	
}
/********************************************************************************************
bool CFtp::Rename_File(CString sFileName, CString sFileNewName)
Description:
重命名文件
Parameters:
1.sFtpFileName:服务器文件名
2.sFtpFileNewName:新的文件名
Returns:
********************************************************************************************/
bool CFtp::Rename_File(CString sFtpFileName, CString sFtpFileNewName)
{
	if (!p_FtpConnection->Rename(sFtpFileName, sFtpFileNewName))
	{
		return false;
	}
	return true;
}
/********************************************************************************************
ULONG CFtp::Get_ServerFile_Size(CString sFtpFileName)
Description:
获取Server文件大小
Parameters:
1.sFtpFileName:服务器文件名
Returns:
文件大小
********************************************************************************************/
ULONGLONG CFtp::Get_ServerFile_Size(CString sFtpFileName)
{
	ULONGLONG ull_FileSize = 0;
	CFtpFileFind fFinder(p_FtpConnection);
	BOOL bFind = fFinder.FindFile(sFtpFileName);
	if (bFind)
	{
		fFinder.FindNextFile();
		ull_FileSize = fFinder.GetLength();
	}
	fFinder.Close();
	return ull_FileSize;
}
bool CFtp::Get_File_Size(const CString cs_Path, unsigned long& ulp_Size)
{
	HANDLE hFile_Handle = INVALID_HANDLE_VALUE;

	try
	{
		//打开
		hFile_Handle = ::CreateFile(cs_Path,
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
		ulp_Size = ::GetFileSize(hFile_Handle, NULL);

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
/********************************************************************************************
bool CFtp::Get_Files_From_Current_Path()
Description:
获取当前目录下的所有文件名
Parameters:

Returns:
文件名存储在m_sFileList里面
********************************************************************************************/
bool CFtp::Get_Files_From_Current_Path(CString sKey, CString sFileReject)
{
	CStringA sFileNameA = "";
	CString sFileName = L"", sFileLength = L"";
	//先清空掉
	m_sFileList[0].RemoveAll();
	m_sFileList[1].RemoveAll();
	CFtpFileFind fFinder(p_FtpConnection);
	BOOL bFind = fFinder.FindFile(TEXT("*"));
	while (bFind)
	{
		bFind = fFinder.FindNextFile();
		if (fFinder.IsDots() || fFinder.IsDirectory())
		{
			continue;
		}
		sFileNameA = fFinder.GetFileName();
		sFileName = CStrA2CStrW(sFileNameA);
		if ((sFileName.Find(sKey) >= 0 || sKey.IsEmpty()) && sFileName != sFileReject)
		{
			m_sFileList[0].Add(sFileName);
			sFileLength.Format(L"%ld", fFinder.GetLength());
			m_sFileList[1].Add(sFileLength);
		}	
	}
	fFinder.Close();
	if (m_sFileList[0].GetCount() <= 0)
	{
		return false;
	}
	return true;
}
/********************************************************************************************
bool Find_File_In_Current_Path()
Description:查询文件是否存在
Parameters:

Returns:
TRUE/FAIL
********************************************************************************************/
bool CFtp::Find_File_In_Current_Path(CString sFileName)
{
	CFtpFileFind fFinder(p_FtpConnection);
	BOOL bFind = fFinder.FindFile(sFileName);
	if (bFind)
	{
		return true;
	}
	return false;
}