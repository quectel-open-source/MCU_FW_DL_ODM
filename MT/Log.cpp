// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

/******************************************************************************
**  COPYRIGHT (C) 2008 by Gregory Stern; All rights reserved.
*******************************************************************************/


#include "stdafx.h"
//#include "sdefines.h"
#include "Log.h"
#include "LogWnd.h"
#include "MPMDexception.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLog* CLog::m_pThis;
BOOL  CLog::m_bLogEnable = TRUE;


#define LINE_DOWN	_T("\r\n======================================================================\r\n")
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog(CLogWnd* pLogWnd)
{
	if (pLogWnd)
		m_pLogWnd = pLogWnd;



	TCHAR szFullExeName[1024];
	DWORD nResult = ::GetModuleFileName(NULL, szFullExeName, 1024);
	CString strFullFileName = szFullExeName;
	CString LogPath = strFullFileName.Left(strFullFileName.ReverseFind('\\') + 1);
	LogPath += _T("MPMDownloader.log"); 

 

	CFileException e;
	if ( !Open( LogPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeWrite, &e))
	{
		DWORD err = GetLastError();
		throw CMPMDexception(_T("Open MPMDownloader.log "));
	}

	if (GetLength())
	{
		SeekToEnd();
	}

	CString Pattern;
	CTime tm = CTime::GetCurrentTime();

	WriteString(LINE_DOWN);

	WriteString(Pattern);	
}

CLog::~CLog()
{
	if (m_bLogEnable == 0)
	{
		return;
	}
	WriteString(LINE_DOWN);
	Flush();
	Close();
	delete m_pThis;
	m_pThis = NULL;
}

CLog* CLog::Instance(CLogWnd* pCLogWnd)
{
	if ( m_pThis == NULL )
	{
		try
		{
			m_pThis = new CLog(pCLogWnd);
		}
		catch(CMPMDexception e)
		{
			if (m_pThis)
			{
				delete m_pThis;
			}
			throw(e);
		} 
	}
	return m_pThis;
}


void CLog::Destroy()
{
	if (m_pThis)
	{
		delete m_pThis;
		m_pThis = NULL;
	}
}


BOOL CLog::IsLogEnable()
{
	if (m_pLogWnd && IsWindow(m_pLogWnd->m_hWnd) && m_pLogWnd->IsVisible())
		return TRUE;
	return FALSE;
}

BOOL CLog::FullPath( const CString &fileName, CString *fullPath )
{
    BOOL success = FALSE;
    TCHAR *filePtr;
    TCHAR buf[1024];

    if ( SearchPath( NULL, fileName, NULL, 1024, buf, &filePtr ) != 0 )
    {
        *fullPath = buf;

        // we found it
        success = TRUE;
    }

    return( success );
}

void CLog::Record(int nDev, const CString msg, BOOL bCommand )
{
	if (!IsLogEnable())
		return;
	
	CString Record;

	TCHAR lpTimeStr[40], lpTimeFormatStr[40];

	//获取当地的时间。

	SYSTEMTIME stLocal;

	::GetLocalTime(&stLocal);


	//显示时间的间隔。

	tsprintf(lpTimeFormatStr, 40, _T("%d/%02d/%02d %d:%02d:%02d:%d \n"),                      

		stLocal.wYear, stLocal.wMonth, stLocal.wDay,

		stLocal.wHour, stLocal.wMinute, stLocal.wSecond,stLocal.wMilliseconds);


	int rc = GetTimeFormat( LOCALE_USER_DEFAULT,	TIME_FORCE24HOURFORMAT,	
		&stLocal, lpTimeFormatStr, lpTimeStr, 40);

	if (bCommand)
	{
		Record.Format(_T("Device: %d, Command - \t"), nDev);
	}
	else
	{
		Record.Format(_T("Device: %d, Response - \t"), nDev);
	}
	
	Record += msg;
	Record += _T("\t\t[");
	Record += lpTimeStr;
	Record += _T(']');
	Record += _T("\r\n");
	
	m_pLogWnd->FillLogWindow(Record);
	WriteString(Record);
}

void CLog::Record(int nDev, LPCTSTR lpszFormat, ...)
{
	if (!IsLogEnable())
		return;
	
	ASSERT(AfxIsValidString(lpszFormat));

	CString Record, Msg;
	
	TCHAR lpTimeStr[40], lpTimeFormatStr[40];

	va_list argList;
	va_start(argList, lpszFormat);
	Msg.FormatV(lpszFormat, argList);
	va_end(argList);
	
	//获取当地的时间。

	SYSTEMTIME stLocal;

	::GetLocalTime(&stLocal);


	//显示时间的间隔。

		tsprintf(lpTimeFormatStr, 40, _T("%d/%02d/%02d %d:%02d:%02d:%d \n"),                      

		stLocal.wYear, stLocal.wMonth, stLocal.wDay,

		stLocal.wHour, stLocal.wMinute, stLocal.wSecond,stLocal.wMilliseconds);



	//sprintf(lpTimeFormatStr,"YYYY-MM-DD hh':'mm':'ss':'%03d", sysTime.wMilliseconds);	
	int rc = GetTimeFormat( LOCALE_USER_DEFAULT,	TIME_FORCE24HOURFORMAT,	
		&stLocal, lpTimeFormatStr, lpTimeStr, 40);


	if (nDev)
	{
		Record.Format(_T("[%s] - Device: %d, %s\r\n"),lpTimeStr, nDev, (LPCTSTR)Msg);
	}
	else
	{
		Record.Format(_T("[%s] - %s\r\n"), lpTimeStr, (LPCTSTR)Msg);
	}

	m_pLogWnd->FillLogWindow(Record);
	WriteString(Record);
}

void CLog::Record(int nDev, UINT nID, ...)
{
	if (!IsLogEnable())
		return;
	
	CString Record, Msg;

	TCHAR lpTimeStr[40], lpTimeFormatStr[40], pstrFormat[MAX_PATH];
    AfxLoadString(nID, pstrFormat);

    va_list args;
    va_start(args, nID);
    Msg.FormatV(pstrFormat, args);
    va_end(args);

	//获取当地的时间。

	SYSTEMTIME stLocal;

	::GetLocalTime(&stLocal);


	//显示时间的间隔。

	tsprintf(lpTimeFormatStr, 40, _T("%d/%02d/%02d %d:%02d:%02d:%d \n"),                      
		stLocal.wYear, stLocal.wMonth, stLocal.wDay,
		stLocal.wHour, stLocal.wMinute, stLocal.wSecond,stLocal.wMilliseconds);



	//sprintf(lpTimeFormatStr,"YYYY-MM-DD hh':'mm':'ss':'%03d", sysTime.wMilliseconds);	
	int rc = GetTimeFormat( LOCALE_USER_DEFAULT,	TIME_FORCE24HOURFORMAT,	
		                    &stLocal, lpTimeFormatStr, lpTimeStr, 40);
	
	if (nDev)
	{
		Record.Format(_T("[%s] - Device ID: %d, %s\r\n"),lpTimeStr, nDev, (LPCTSTR)Msg);
	}
	else
	{
		Record.Format(_T("[%s] - %s\r\n"), lpTimeStr, (LPCTSTR)Msg);
	}

	m_pLogWnd->FillLogWindow(Record);
	WriteString(Record);
}
