// ConfigDig.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "ConfigDig.h"


// CConfigDig dialog

IMPLEMENT_DYNAMIC(CConfigDig, CDialog)

CConfigDig::CConfigDig(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDig::IDD, pParent)
	, m_iIdxComPort(0)
	, m_iIdxBaudrate(0)
	, m_csGenPassword(_T(""))
{

}

CConfigDig::~CConfigDig()
{
}

void CConfigDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_COMPORT, m_iIdxComPort);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_ctr_cbxComPort);
	DDX_CBIndex(pDX, IDC_COMBO_BAUDRATE, m_iIdxBaudrate);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_ctr_cbxBaudrate);
	DDX_Text(pDX, IDC_EDIT_MAINPASSWORD, m_csGenPassword);
}


BEGIN_MESSAGE_MAP(CConfigDig, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigDig::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDig message handlers

BOOL CConfigDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDig::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}

void CConfigDig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);

	OnOK();
}
