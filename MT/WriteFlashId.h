#pragma once


// CWriteFlashId dialog

class CWriteFlashId : public CDialog
{
	DECLARE_DYNAMIC(CWriteFlashId)

public:
	CWriteFlashId(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteFlashId();

// Dialog Data
	enum { IDD = IDD_DIALOG_FLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
