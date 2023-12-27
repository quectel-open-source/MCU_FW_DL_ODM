// ListProgress.cpp : implementation file
//

#include "stdafx.h"
#include "ListProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CListProgress

CListProgress::CListProgress(int nItem)
:m_nItem(nItem)
{
    m_nPos				= 0;
    m_nStepSize			= 1;
    m_nMax				= 100;
    m_nMin				= 0;
    m_bShowText			= TRUE;
    m_strText			= _T("");
	m_strState			= _T("");
	m_colFore			= ::GetSysColor(COLOR_HIGHLIGHT);
	m_colBk				= ::GetSysColor(COLOR_WINDOW);
	m_colTextFore		= ::GetSysColor(COLOR_HIGHLIGHT);
	m_colTextBk			= ::GetSysColor(COLOR_WINDOW);
	m_bWaitBeRemoved	= FALSE;
	m_bBootComplete		= FALSE;

    m_nBarWidth			= -1;
}

CListProgress::~CListProgress()
{
}


BEGIN_MESSAGE_MAP(CListProgress, CProgressCtrl)
	//{{AFX_MSG_MAP(CListProgress)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
  //  ON_MESSAGE(WM_SETTEXT, OnSetText)
  //  ON_MESSAGE(WM_GETTEXT, OnGetText)
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListProgress message handlers


BOOL CListProgress::OnEraseBkgnd(CDC* /*pDC*/) 
{	
 	return TRUE;
}

void CListProgress::OnSize(UINT nType, int cx, int cy) 
{
	CProgressCtrl::OnSize(nType, cx, cy);
	
    m_nBarWidth	= -1;   // Force update if SetPos called
}

void CListProgress::OnPaint() 
{
    if (m_nMin >= m_nMax) 
        return;

    CRect LeftRect, RightRect, ClientRect;
    GetClientRect(ClientRect);

    double Fraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));

	CDC dc;
	CPaintDC PaintDC(this); // device context for painting
    dc.Attach(PaintDC.GetSafeHdc());
	
    LeftRect = RightRect = ClientRect;

    LeftRect.right = LeftRect.left + (int)((LeftRect.right - LeftRect.left)*Fraction);
    dc.FillSolidRect(LeftRect, m_colFore);

    RightRect.left = LeftRect.right;
    dc.FillSolidRect(RightRect, m_colBk);

    if (m_bShowText)
    {
        CString str, text;

    	if (!m_strState.IsEmpty())
 		{
 			str.Format(_T("%s "), (LPCTSTR)m_strState);
 		}		
		
        dc.SetBkMode(TRANSPARENT);

        CRgn rgn;
        rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_colTextBk);

        dc.DrawText(str, ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        rgn.DeleteObject();
        rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
        dc.SelectClipRgn(&rgn);
        dc.SetTextColor(m_colTextFore);

        dc.DrawText(str, ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void CListProgress::SetForeColour(COLORREF col)
{
	m_colFore = col;
}

void CListProgress::SetBkColour(COLORREF col)
{
	m_colBk = col;
}

void CListProgress::SetTextForeColour(COLORREF col)
{
	m_colTextFore = col;
}

void CListProgress::SetTextBkColour(COLORREF col)
{
	m_colTextBk = col;
}

COLORREF CListProgress::GetForeColour()
{
	return m_colFore;
}

COLORREF CListProgress::GetBkColour()
{
	return m_colBk;
}

COLORREF CListProgress::GetTextForeColour()
{
	return m_colTextFore;
}

COLORREF CListProgress::GetTextBkColour()
{
	return m_colTextBk;
}

void CListProgress::SetShowText(BOOL bShow)
{ 
    if (::IsWindow(m_hWnd) && m_bShowText != bShow)
        Invalidate();

    m_bShowText = bShow;
}


void CListProgress::SetRange(int nLower, int nUpper)
{
    m_nMax = nUpper;
    m_nMin = nLower;
}

int CListProgress::SetPos(int nPos) 
{	
    if (!::IsWindow(m_hWnd))
        return -1;

    int nOldPos = m_nPos;
    m_nPos = nPos;

    CRect rect;
    GetClientRect(rect);

    double Fraction = (double)(m_nPos - m_nMin) / ((double)(m_nMax - m_nMin));
    int nBarWidth = (int) (Fraction * rect.Width());

    if (nBarWidth != m_nBarWidth)
    {
        m_nBarWidth = nBarWidth;
        RedrawWindow();
    }

    return nOldPos;
}

int CListProgress::StepIt() 
{	
   return SetPos(m_nPos + m_nStepSize);
}

int CListProgress::OffsetPos(int nPos)
{
    return SetPos(m_nPos + nPos);
}

int CListProgress::SetStep(int nStep)
{
    int nOldStep = m_nStepSize;
    m_nStepSize = nStep;
    return nOldStep;
}

void CListProgress::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CProgressCtrl::OnRButtonDown(nFlags, point);
}
