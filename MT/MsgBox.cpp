// MsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "MsgBox.h"


// CMsgBox dialog

IMPLEMENT_DYNAMIC(CMsgBox, CDialog)

CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBox::IDD, pParent)
{

}

CMsgBox::~CMsgBox()
{
}

void CMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PASS, m_ctr_staPass);
	DDX_Control(pDX, IDC_STATIC_FAIL, m_ctr_staFail);
	DDX_Control(pDX, IDC_EDIT_SHOW, m_ctr_edtShow);
}


BEGIN_MESSAGE_MAP(CMsgBox, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMsgBox message handlers

void CMsgBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	this->KillTimer((int)nIDEvent);

	if(nIDEvent == 2)
	{
//		DestroyWindow();
		CDialog::OnOK();
	}

	CDialog::OnTimer(nIDEvent);
}

void CMsgBox::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
	delete this;
}