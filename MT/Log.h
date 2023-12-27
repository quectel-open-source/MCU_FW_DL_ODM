// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__2D6EBE00_108A_43B3_9045_57F4E93F1DDD__INCLUDED_)
#define AFX_LOG_H__2D6EBE00_108A_43B3_9045_57F4E93F1DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "singletondestroyer.h"

class CLogWnd;

class CLog : public CStdioFile 
{
public:
	CLog(CLogWnd*);
	virtual ~CLog();
	static CLog* Instance(CLogWnd* = NULL);
	static void Destroy();
	
private:

	CLogWnd* m_pLogWnd;

private:
	BOOL IsLogEnable();
	BOOL FullPath( const CString &fileName, CString *fullPath );

public:

	static CLog* m_pThis;
	static BOOL  m_bLogEnable;

public:

	void Record(int nDev, UINT nID, ...);
	void Record(int nDev,LPCTSTR lpszFormat, ...);
	void Record(int nDev,const CString msg, BOOL bCommand = true );
};

#endif // !defined(AFX_LOG_H__2D6EBE00_108A_43B3_9045_57F4E93F1DDD__INCLUDED_)
