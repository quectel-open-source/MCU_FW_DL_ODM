// logview.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "logview.h"


// logview dialog

IMPLEMENT_DYNAMIC(logview, CDialog)

logview::logview(CWnd* pParent /*=NULL*/)
	: CDialog(logview::IDD, pParent)
{

}

logview::~logview()
{
}

void logview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_SHOW_INFO, m_ctrl_log);
	DDX_Control(pDX, IDC_RICHEDIT2_SHOW_1, m_ctrl_log1);
	DDX_Control(pDX, IDC_RICHEDIT2_SHOW_2, m_ctrl_log2);
	DDX_Control(pDX, IDC_RICHEDIT2_SHOW_3, m_ctrl_log3);
	DDX_Control(pDX, IDC_RICHEDIT2_SHOW_4, m_ctrl_log4);
}


BEGIN_MESSAGE_MAP(logview, CDialog)
	ON_EN_CHANGE(IDC_RICHEDIT2_SHOW_INFO, &logview::OnEnChangeRichedit2ShowInfo)
	ON_MESSAGE(WM_REDIT_INFO_MSG, OnGetMsg_Show_REdit_Info)
	ON_MESSAGE(WM_LOG_INFO_MSG, OnGetMsg_log_Info)

	

END_MESSAGE_MAP()


// logview message handlers

void logview::OnEnChangeRichedit2ShowInfo()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

LRESULT logview::OnGetMsg_Show_REdit_Info(WPARAM wParam, LPARAM lParam)
{

	//20200403
	CTime aTime = CTime::GetCurrentTime();
	CString csData;

	int y = aTime.GetYear();
	int m = aTime.GetMonth();
	int d = aTime.GetDay();
	int h = aTime.GetHour();
	int min = aTime.GetMinute();
	int s = aTime.GetSecond();

	csData.Format(_T("%d-%d-%d_%d:%d:%d <--> %s"), y, m, d, h, min, s, (wchar_t *)wParam);

	this->m_ctrl_log.SetSel(1000000000, 1000000000);

	this->m_ctrl_log.SetSel(-1, -1);

	this->m_ctrl_log.ReplaceSel(csData); 

	if((bool)lParam)
	{
		this->m_ctrl_log.ReplaceSel(_T("\r\n"));
	}

	this->m_ctrl_log.LineScroll(1, 0);

	return LRESULT();
}


LRESULT logview::OnGetMsg_log_Info(WPARAM wParam, LPARAM lParam)
{
	CTime aTime = CTime::GetCurrentTime();
	CString csData;

	int y = aTime.GetYear();
	int m = aTime.GetMonth();
	int d = aTime.GetDay();
	int h = aTime.GetHour();
	int min = aTime.GetMinute();
	int s = aTime.GetSecond();

	csData.Format(_T("%d-%d-%d_%d:%d:%d <--> %s"), y, m, d, h, min, s, (wchar_t *)wParam);

	switch((int)lParam)
	{
		case 0:

			break;
		case 1:
			//this->m_ctrlOut1.SetSel(1000000000, 1000000000);

			this->m_ctrl_log1.SetSel(-1, -1);

			this->m_ctrl_log1.ReplaceSel(csData); 

			this->m_ctrl_log1.ReplaceSel(_T("\r\n"));

			this->m_ctrl_log1.LineScroll(1, 0);

			break;
		case 2:
			//this->m_ctrlOut2.SetSel(1000000000, 1000000000);

			this->m_ctrl_log2.SetSel(-1, -1);

			this->m_ctrl_log2.ReplaceSel(csData); 

			this->m_ctrl_log2.ReplaceSel(_T("\r\n"));

			this->m_ctrl_log2.LineScroll(1, 0);
			break;
		case 3:
			//this->m_ctrlOut3.SetSel(1000000000, 1000000000);

			this->m_ctrl_log3.SetSel(-1, -1);

			this->m_ctrl_log3.ReplaceSel(csData); 

			this->m_ctrl_log3.ReplaceSel(_T("\r\n"));

			this->m_ctrl_log3.LineScroll(1, 0);
			break;
		case 4:
			//this->m_ctrlOut4.SetSel(1000000000, 1000000000);

			this->m_ctrl_log4.SetSel(-1, -1);

			this->m_ctrl_log4.ReplaceSel(csData); 

			this->m_ctrl_log4.ReplaceSel(_T("\r\n"));

			this->m_ctrl_log4.LineScroll(1, 0);
			break;

		default:
			break;
	}

	return LRESULT();
}


BOOL logview::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
