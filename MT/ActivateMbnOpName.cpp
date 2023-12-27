// ActivateMbnOpName.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "ActivateMbnOpName.h"


// CActivateMbnOpName dialog

IMPLEMENT_DYNAMIC(CActivateMbnOpName, CDialog)

CActivateMbnOpName::CActivateMbnOpName(CWnd* pParent /*=NULL*/)
	: CDialog(CActivateMbnOpName::IDD, pParent)
	, m_bAUTOSEL_MBN(FALSE)
{

}

CActivateMbnOpName::~CActivateMbnOpName()
{
}

void CActivateMbnOpName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_C_AUTOSEL_MBN, m_bAUTOSEL_MBN);
}


BEGIN_MESSAGE_MAP(CActivateMbnOpName, CDialog)
	ON_BN_CLICKED(IDOK, &CActivateMbnOpName::OnBnClickedOk)
END_MESSAGE_MAP()


// CActivateMbnOpName message handlers

void CActivateMbnOpName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	OnOK();

}
