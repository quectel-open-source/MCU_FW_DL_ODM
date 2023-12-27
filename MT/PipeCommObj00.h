#pragma once

#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>

#ifdef DLL_IMPLEMENT
#define DLL_IMP _declspec(dllexport)
#else
#define DLL_IMP _declspec(dllimport)
#endif

#define	PIPECOMM_IAC		(u_char)255
#define	PIPECOMM_DONT		(u_char)254
#define	PIPECOMM_DO			(u_char)253
#define	PIPECOMM_WONT		(u_char)252
#define	PIPECOMM_WILL		(u_char)251
#define TELOPT_ECHO			(u_char)1

#define PIPECOMM_PORT		1

#define MIN_MSG_SIZE		1024
#define DEFAULT_MSG_SIZE	65536
#define DEFAULT_HOST		"localhost"
#define DEFAULT_PROMPT		"[$%#>] "
#define DEFAULT_LOGINTIME	5.000
#define	DEFAULT_WAITTIME	5.500

#define MSGCHK_SIZE			5000
#define COMMAND_LENGTH		5000

struct PROID2WND
{
	DWORD dwPid;
	HWND hWnd;
};

class CPipeCommObj00
{
public:
	DLL_IMP CPipeCommObj00();
	DLL_IMP ~CPipeCommObj00(void);
	//¿‡–Õ
	
private:
	int ThreadFlag;
	CStringA sThread_Log;
	HANDLE Thread;
	HWND mProcessWind;
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	BOOL ExtractStr(CStringA sData, INT nIndex, CStringA &sOutput);
public:
	CStringA m_Rst;
	virtual CStringA GetObjectInfo();
	virtual VOID ViewObjectInfo(){}

	
	DLL_IMP	BOOL _InitPipeComm(const CStringA sInput = CStringA(), CStringA &sOutput = CStringA());
	DLL_IMP BOOL _InitPipeComm1(const CStringA sInput = CStringA(), CStringA &sOutput = CStringA());
	DLL_IMP BOOL _SendStr(const CStringA sInput = CStringA(), CStringA &sOutput = CStringA());
	DLL_IMP BOOL _SendStr1(const CStringA sInput = CStringA(), CStringA &sOutput = CStringA());
	DLL_IMP BOOL _ReceiveStr(const CStringA sInput = CStringA(), CStringA &sOutput = CStringA());
	DLL_IMP BOOL _Terminate_Process(const CStringA sInput = CStringA(), CStringA &sOutput = CStringA());

private:
	int GetPipeBuffer(char *pBuf);
	void PrintLog(CStringA sLog);
	HANDLE hP1Read,hP1Write;
	HANDLE hP2Read,hP2Write;
	CStdioFile	*pFile;
	STARTUPINFO	si;
	PROCESS_INFORMATION	pi;
	//
	BOOL bIsConnect;
	BOOL bPrintLog;
	CStringA m_RootKeyWord;
};
