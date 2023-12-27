#pragma once
#include "afxwin.h"


// CDlg_Password dialog

class CDlg_Password : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Password)

public:
	CDlg_Password(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Password();

// Dialog Data
	enum { IDD = IDD_DIALOG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CFont m_cla_fontShow_1;
	CString m_csPassword;
	CEdit m_ctr_edtPassword;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CString m_csPassword_standard;
};
