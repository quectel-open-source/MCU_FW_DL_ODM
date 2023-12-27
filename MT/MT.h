
// MT.h : main header file for the MT application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// CMTApp:
// See MT.cpp for the implementation of this class
//

class CMTApp : public CWinAppEx
{
public:
	CMTApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

	bool m_bExitFlag;
};

extern CMTApp theApp;
