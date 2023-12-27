// WriteFlashId.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "WriteFlashId.h"


// CWriteFlashId dialog

IMPLEMENT_DYNAMIC(CWriteFlashId, CDialog)

CWriteFlashId::CWriteFlashId(CWnd* pParent /*=NULL*/)
	: CDialog(CWriteFlashId::IDD, pParent)
{

}

CWriteFlashId::~CWriteFlashId()
{
}

void CWriteFlashId::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWriteFlashId, CDialog)
	ON_BN_CLICKED(IDOK, &CWriteFlashId::OnBnClickedOk)
END_MESSAGE_MAP()


// CWriteFlashId message handlers

void CWriteFlashId::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
