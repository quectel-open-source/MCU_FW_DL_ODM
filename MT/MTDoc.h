
// MTDoc.h : interface of the CMTDoc class
//

#pragma once


class CMTDoc : public CDocument
{
protected: // create from serialization only
	CMTDoc();
	DECLARE_DYNCREATE(CMTDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


