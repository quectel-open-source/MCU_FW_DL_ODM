#pragma once
#include "afxwin.h"
#include "inifile.h"

// CWorkNum dialog

class CWorkNum : public CDialog
{
	DECLARE_DYNAMIC(CWorkNum)

public:
	CWorkNum(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWorkNum();

// Dialog Data
	enum { IDD = IDD_DIALOG_WORK_NUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_StrWorkNum;
	CString m_strlLine;
	CEdit m_ctrlLine;
	CIniFile m_cla_iniFile_Ext;

	virtual BOOL OnInitDialog();
	bool IncludeChinese(char * str);
};
