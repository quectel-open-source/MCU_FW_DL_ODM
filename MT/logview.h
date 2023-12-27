#pragma once
#include "afxcmn.h"
#include "dlg_fw_multi.h"


// logview dialog

class logview : public CDialog
{
	DECLARE_DYNAMIC(logview)

public:
	logview(CWnd* pParent = NULL);   // standard constructor
	virtual ~logview();

// Dialog Data
	enum { IDD = IDD_DIA_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl m_ctrl_log;
	afx_msg void OnEnChangeRichedit2ShowInfo();

	LRESULT OnGetMsg_Show_REdit_Info(WPARAM wParam, LPARAM lParam);

	CDlg_FW_Multi m_dlg_Upgrade_FW_Multi;

	virtual BOOL OnInitDialog();
	CRichEditCtrl m_ctrl_log1;
	CRichEditCtrl m_ctrl_log2;
	CRichEditCtrl m_ctrl_log3;
	CRichEditCtrl m_ctrl_log4;

	LRESULT OnGetMsg_log_Info(WPARAM wParam, LPARAM lParam);
};
