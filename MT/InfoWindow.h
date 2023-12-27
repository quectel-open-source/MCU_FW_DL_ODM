#pragma once
#include "afxcmn.h"


// CInfoWindow dialog

class CInfoWindow : public CDialog
{
	DECLARE_DYNAMIC(CInfoWindow)

public:
	CInfoWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfoWindow();

// Dialog Data
	enum { IDD = IDD_DIALOG_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
protected:
	virtual void PostNcDestroy();
public:
	CRichEditCtrl m_ctr_redtShowInfo;
	void Show_RichEdit_Info(wchar_t * wcpMsg, int iOP);
	virtual BOOL OnInitDialog();
};
