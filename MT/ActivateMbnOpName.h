#pragma once
#include "afxwin.h"
#include "inifile.h"


// CActivateMbnOpName dialog

class CActivateMbnOpName : public CDialog
{
	DECLARE_DYNAMIC(CActivateMbnOpName)

public:
	CActivateMbnOpName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CActivateMbnOpName();

// Dialog Data
	enum { IDD = IDD_DIALOG_ACT_MBN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CIniFile m_cla_iniFile_Ext;
	BOOL m_bAUTOSEL_MBN;
};
