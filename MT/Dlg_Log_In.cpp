// Dlg_Log_In.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "Dlg_Log_In.h"


// CDlg_Log_In dialog

IMPLEMENT_DYNAMIC(CDlg_Log_In, CDialog)

CDlg_Log_In::CDlg_Log_In(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Log_In::IDD, pParent)
	, m_csLogInName(_T(""))
	, m_csLogInPassword(_T(""))
{

}

CDlg_Log_In::~CDlg_Log_In()
{
}

void CDlg_Log_In::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOG_IN_NAME, m_ctr_staLogInName);
	DDX_Control(pDX, IDC_EDIT_LOG_IN_NAME, m_ctr_edtLogInName);
	DDX_Control(pDX, IDC_STATIC_LOG_IN_PASSWORD, m_ctr_staLogInPassword);
	DDX_Control(pDX, IDC_EDIT_LOG_IN_PASSWORD, m_ctr_edtLogInPassword);
	DDX_Text(pDX, IDC_EDIT_LOG_IN_NAME, m_csLogInName);
	DDX_Text(pDX, IDC_EDIT_LOG_IN_PASSWORD, m_csLogInPassword);
	DDX_Control(pDX, IDC_BUTTON_LOG_IN, m_ctr_btnLogInOut);
	DDX_Control(pDX, IDC_BUTTON_CHANGE_PASSWORD, m_ctr_btnChange_Password);
}


BEGIN_MESSAGE_MAP(CDlg_Log_In, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_Log_In::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LOG_IN, &CDlg_Log_In::OnBnClickedButtonLogIn)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_PASSWORD, &CDlg_Log_In::OnBnClickedButtonChangePassword)
END_MESSAGE_MAP()


// CDlg_Log_In message handlers

void CDlg_Log_In::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->OnBnClickedButtonLogIn();
//	OnOK();
}

void CDlg_Log_In::OnBnClickedButtonLogIn()
{
	//登录状态改变后，主界面的分页界面会起变化，所以登录操作牵动主界面操作
	this->UpdateData(TRUE);
	::SendMessage(this->m_hWnd_Main_View, WM_LOG_CHANGE_MSG, (WPARAM)0, (LPARAM)0);
}

void CDlg_Log_In::OnBnClickedButtonChangePassword()
{
	::SendMessage(this->m_hWnd_Main_View, WM_CHANGE_PASSWORD_MSG, (WPARAM)0, (LPARAM)0);
}

BOOL CDlg_Log_In::OnInitDialog()
{
	CDialog::OnInitDialog();

	//
	this->m_cla_fontDlg_1.CreateFont(25, 10, 0, 0, FW_SEMIBOLD, false, false, false, DEFAULT_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, _T("Verdana"));

	//登录界面
	this->m_ctr_staLogInName.SetFont(&this->m_cla_fontDlg_1);
	this->m_ctr_staLogInPassword.SetFont(&this->m_cla_fontDlg_1);
	this->m_ctr_edtLogInName.SetFont(&this->m_cla_fontDlg_1);
	this->m_ctr_edtLogInPassword.SetFont(&this->m_cla_fontDlg_1);
	this->m_ctr_btnLogInOut.SetFont(&this->m_cla_fontDlg_1);
	this->m_ctr_btnChange_Password.SetFont(&this->m_cla_fontDlg_1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
