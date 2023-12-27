#pragma once
#include <afxinet.h> 

struct FTP_INFO
{
	FTP_INFO& operator=(const FTP_INFO& ftpInfo)
	{
		sServerName = ftpInfo.sServerName;
		sUserName = ftpInfo.sUserName;
		sPassword = ftpInfo.sPassword;
		iPort = ftpInfo.iPort;
		return *this;
	}
	CString sServerName;		//FTP服务器IP
	CString sUserName;			//FTP服务器访问用户名
	CString sPassword;			//FTP服务器访问密码
	DWORD iPort;				//FTP服务器端口
};

class CFtp
{
public:
	CFtp();
	virtual ~CFtp();
protected:
	CInternetSession * p_InternetSession;
	CFtpConnection * p_FtpConnection;
public:
	bool m_b_FTP_Connect;
	FTP_INFO m_FtpInfo;
	bool Connect_FTP_Server();
	void DisConnect_FTP_Server();
	bool Create_FTP_Path(CString sFTPPath, CString &sError);
	bool Delete_FTP_Path(CString sPath);
	bool Set_FTP_Path(CString sPath);
	bool Get_Current_FTP_Path(CString& sPath);
	bool Upload_File(CString sLocalFilePath, CString sFtpFileName);
	bool Download_File(CString sFtpFileName, CString sLocalFilePath);
	bool Delete_File(CString sFtpFileName);
	bool Rename_File(CString sFtpFileName, CString sFtpFileNewName);
	ULONGLONG Get_ServerFile_Size(CString sFtpFileName);
	bool Get_File_Size(const CString cs_Path, unsigned long& ulp_Size);
	CStringArray m_sFileList[2];
	bool Get_Files_From_Current_Path(CString sKey = _T(""), CString sFileReject = _T(""));
	bool Find_File_In_Current_Path(CString sFileName);
	//void Sort_FileList_Max_To_Min();
	CStringW CStrA2CStrW(const CStringA &cstrSrcA);
};

