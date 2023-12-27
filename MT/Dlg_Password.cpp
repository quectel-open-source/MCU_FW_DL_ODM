// Dlg_Password.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "Dlg_Password.h"


// CDlg_Password dialog

IMPLEMENT_DYNAMIC(CDlg_Password, CDialog)

CDlg_Password::CDlg_Password(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Password::IDD, pParent)
	, m_csPassword(_T(""))
{

}

CDlg_Password::~CDlg_Password()
{
}

void CDlg_Password::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_csPassword);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_ctr_edtPassword);
}


BEGIN_MESSAGE_MAP(CDlg_Password, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_Password::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_Password message handlers

BOOL CDlg_Password::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	this->m_cla_fontShow_1.CreateFont(18, 7, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS, _T("Verdana"));

	this->m_ctr_edtPassword.SetFont(&this->m_cla_fontShow_1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_Password::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);

	this->m_csPassword.MakeUpper();

	if(this->m_csPassword == m_csPassword_standard)
	{
		this->m_csPassword.Format(_T(""));
		this->m_ctr_edtPassword.SetWindowText(_T(""));

		OnOK();
	}
	else
	{
		this->m_csPassword.Format(_T(""));
		this->m_ctr_edtPassword.SetWindowText(_T(""));
	}
}
