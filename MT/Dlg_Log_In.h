#pragma once
#include "afxwin.h"
#include "Gen_Header.h"


// CDlg_Log_In dialog

class CDlg_Log_In : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Log_In)

public:
	CDlg_Log_In(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Log_In();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOG_IN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CStatic m_ctr_staLogInName;
	CEdit m_ctr_edtLogInName;
	CStatic m_ctr_staLogInPassword;
	CEdit m_ctr_edtLogInPassword;
	CString m_csLogInName;
	CString m_csLogInPassword;
	HWND m_hWnd_Main_View;
	CButton m_ctr_btnLogInOut;
	afx_msg void OnBnClickedButtonLogIn();
	CButton m_ctr_btnChange_Password;
	afx_msg void OnBnClickedButtonChangePassword();
	virtual BOOL OnInitDialog();
	CFont m_cla_fontDlg_1;
};
