
// MTDoc.cpp : implementation of the CMTDoc class
//

#include "stdafx.h"
#include "MT.h"

#include "MTDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMTDoc

IMPLEMENT_DYNCREATE(CMTDoc, CDocument)

BEGIN_MESSAGE_MAP(CMTDoc, CDocument)
END_MESSAGE_MAP()


// CMTDoc construction/destruction

CMTDoc::CMTDoc()
{
	// TODO: add one-time construction code here

}

CMTDoc::~CMTDoc()
{
}

BOOL CMTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMTDoc serialization

void CMTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMTDoc diagnostics

#ifdef _DEBUG
void CMTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMTDoc commands
