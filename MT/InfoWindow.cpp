// InfoWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "InfoWindow.h"


// CInfoWindow dialog

IMPLEMENT_DYNAMIC(CInfoWindow, CDialog)

CInfoWindow::CInfoWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoWindow::IDD, pParent)
{

}

CInfoWindow::~CInfoWindow()
{
}

void CInfoWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_SHOW, m_ctr_redtShowInfo);
}


BEGIN_MESSAGE_MAP(CInfoWindow, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CInfoWindow message handlers

void CInfoWindow::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnCancel();
//	CDialog::OnClose();
}

void CInfoWindow::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
	delete this;
}

void CInfoWindow::Show_RichEdit_Info(wchar_t * wcpMsg, int iOP)
{
	this->m_ctr_redtShowInfo.SetSel(1000000000, 1000000000);
	
	this->m_ctr_redtShowInfo.SetSel(-1, -1);
	this->m_ctr_redtShowInfo.ReplaceSel(wcpMsg);
	if(iOP == 1)
	{
		this->m_ctr_redtShowInfo.ReplaceSel(_T("\r\n"));
	}
	this->m_ctr_redtShowInfo.LineScroll(1, 0);
}

BOOL CInfoWindow::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// xjli
	const CWnd * pWndInsertAfter;
	pWndInsertAfter = &wndBottom;
	SetWindowPos(pWndInsertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//SWP_NOZORDER);//SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
