#pragma once
#include "afxwin.h"


// CDlg_Change_Password dialog

class CDlg_Change_Password : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Change_Password)

public:
	CDlg_Change_Password(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Change_Password();

// Dialog Data
	enum { IDD = IDD_DIALOG_CHANGE_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctr_edtOld_Password;
	CString m_csOld_Password;
	CEdit m_ctr_edtNew_Password;
	CString m_csNew_Password;
	CEdit m_ctr_edtConfirm_Password;
	CString m_csConfirm_Password;
	CStatic m_ctr_staOld_Password;
	CStatic m_ctr_staNew_Password;
	CStatic m_ctr_staConfirm_Password;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CButton m_ctr_btnChange_Password_OK;
	CButton m_ctr_btnChange_Password_Cancel;
	CFont m_cla_fontDlg_3;
};
