#pragma once
#include "afxwin.h"


// CMsgBox dialog

class CMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CMsgBox)

public:
	CMsgBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMsgBox();

// Dialog Data
	enum { IDD = IDD_DIALOG_MSGBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctr_staPass;
	CStatic m_ctr_staFail;
	CEdit m_ctr_edtShow;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void PostNcDestroy();
};
//