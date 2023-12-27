#pragma once
// ListProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListProgress window

class CListProgress : public CProgressCtrl
{
// Construction
public:
	CListProgress(int);

// Attributes
public:

protected:
    int			m_nItem,
		        m_nPos, 
				m_nStepSize, 
				m_nMax, 
				m_nMin;
    CString		m_strText,
				m_strState;
    BOOL		m_bShowText,
				m_bBootComplete,
				m_bWaitBeRemoved;
    int			m_nBarWidth;
	COLORREF	m_colFore,
				m_colBk,
				m_colTextFore,
				m_colTextBk;

// Operations
public:
    int			SetPos(int nPos);
    int			StepIt();
    void		SetRange(int nLower, int nUpper);
    int			OffsetPos(int nPos);
    int			SetStep(int nStep);
	void		SetForeColour(COLORREF col);
	void		SetBkColour(COLORREF col);
	void		SetTextForeColour(COLORREF col);
	void		SetTextBkColour(COLORREF col);
	COLORREF	GetForeColour();
	COLORREF	GetBkColour();
	COLORREF	GetTextForeColour();
	COLORREF	GetTextBkColour();

    void		SetShowText(BOOL bShow);
	void		SetProcState(const CString& state) {m_strState = state;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListProgress)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListProgress();
	int GetIndex(){ return m_nItem;}
	void SetIndex(int nIdx){ m_nItem = nIdx;}

	// Generated message map functions
protected:
	//{{AFX_MSG(CListProgress)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
 //   afx_msg LRESULT OnSetText(WPARAM,LPARAM);
  //  afx_msg LRESULT OnGetText(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

