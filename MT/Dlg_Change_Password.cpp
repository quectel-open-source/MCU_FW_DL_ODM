// Dlg_Change_Password.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "Dlg_Change_Password.h"


// CDlg_Change_Password dialog

IMPLEMENT_DYNAMIC(CDlg_Change_Password, CDialog)

CDlg_Change_Password::CDlg_Change_Password(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Change_Password::IDD, pParent)
	, m_csOld_Password(_T(""))
	, m_csNew_Password(_T(""))
	, m_csConfirm_Password(_T(""))
{

}

CDlg_Change_Password::~CDlg_Change_Password()
{
}

void CDlg_Change_Password::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OLD_PASSWORD, m_ctr_edtOld_Password);
	DDX_Text(pDX, IDC_EDIT_OLD_PASSWORD, m_csOld_Password);
	DDX_Control(pDX, IDC_EDIT_NEW_PASSWORD, m_ctr_edtNew_Password);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD, m_csNew_Password);
	DDX_Control(pDX, IDC_EDIT_CONFIRM_PASSWORD, m_ctr_edtConfirm_Password);
	DDX_Text(pDX, IDC_EDIT_CONFIRM_PASSWORD, m_csConfirm_Password);
	DDX_Control(pDX, IDC_STATIC_OLD_PASSWORD, m_ctr_staOld_Password);
	DDX_Control(pDX, IDC_STATIC_NEW_PASSWORD, m_ctr_staNew_Password);
	DDX_Control(pDX, IDC_STATIC_CONFIRM_PASSWORD, m_ctr_staConfirm_Password);
	DDX_Control(pDX, IDOK, m_ctr_btnChange_Password_OK);
	DDX_Control(pDX, IDCANCEL, m_ctr_btnChange_Password_Cancel);
}


BEGIN_MESSAGE_MAP(CDlg_Change_Password, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_Change_Password::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_Change_Password message handlers

void CDlg_Change_Password::OnBnClickedOk()
{
	this->UpdateData(TRUE);

	OnOK();
}

BOOL CDlg_Change_Password::OnInitDialog()
{
	CDialog::OnInitDialog();

	//修改密码界面字体（这个字体的设置不能放在主程序View类中）
	this->m_cla_fontDlg_3.CreateFont(21, 7, 0, 0, FW_SEMIBOLD, false, false, false, DEFAULT_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, _T("Verdana"));

	this->m_ctr_edtOld_Password.SetFont(&this->m_cla_fontDlg_3);
	this->m_ctr_edtNew_Password.SetFont(&this->m_cla_fontDlg_3);
	this->m_ctr_edtConfirm_Password.SetFont(&this->m_cla_fontDlg_3);
	this->m_ctr_staOld_Password.SetFont(&this->m_cla_fontDlg_3);
	this->m_ctr_staNew_Password.SetFont(&this->m_cla_fontDlg_3);
	this->m_ctr_staConfirm_Password.SetFont(&this->m_cla_fontDlg_3);
	this->m_ctr_btnChange_Password_OK.SetFont(&this->m_cla_fontDlg_3);
	this->m_ctr_btnChange_Password_Cancel.SetFont(&this->m_cla_fontDlg_3);

	//清空界面
	this->m_ctr_edtOld_Password.SetWindowTextW(_T(""));
	this->m_ctr_edtNew_Password.SetWindowTextW(_T(""));
	this->m_ctr_edtConfirm_Password.SetWindowTextW(_T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
