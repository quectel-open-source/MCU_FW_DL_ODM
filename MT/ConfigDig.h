#pragma once
#include "afxwin.h"
#include "inifile.h"


// CConfigDig dialog

class CConfigDig : public CDialog
{
	DECLARE_DYNAMIC(CConfigDig)

public:
	CConfigDig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigDig();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iIdxComPort;
	CComboBox m_ctr_cbxComPort;
	int m_iIdxBaudrate;
	CComboBox m_ctr_cbxBaudrate;
	CString m_csGenPassword;
	CIniFile m_cla_iniFile;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
};
