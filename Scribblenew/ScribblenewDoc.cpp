
// ScribblenewDoc.cpp : CScribblenewDoc ���O����@
//

#include "stdafx.h"
// SHARED_HANDLERS �i�H�w�q�b��@�w���B�Y�ϩM�j�M�z�����B�z�`����
// ATL �M�פ��A�ä��\�P�ӱM�צ@�Τ��{���X�C
#ifndef SHARED_HANDLERS
#include "Scribblenew.h"
#endif
#include <afxwinappex.h>
#include "ScribblenewDoc.h"
#include "PenWidthsDlg.h"
#include <propkey.h>
#include "ScribblenewView.h"
#include "CanvasDlg.h"
#include <afxwin.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CScribblenewDoc

IMPLEMENT_DYNCREATE(CScribblenewDoc, CDocument)

BEGIN_MESSAGE_MAP(CScribblenewDoc, CDocument)
	ON_COMMAND(ID_PEN_CLEARALL, &CScribblenewDoc::OnPenClearall)
	ON_COMMAND(ID_PEN_THICKLINE, &CScribblenewDoc::OnPenThickline)
//	ON_UPDATE_COMMAND_UI(ID_PEN_THICKLINE, &CScribblenewDoc::OnUpdatePenThickline)
	ON_UPDATE_COMMAND_UI(ID_PEN_CLEARALL, &CScribblenewDoc::OnUpdatePenClearall)
	ON_COMMAND(ID_PEN_WIDTHS, &CScribblenewDoc::OnPenWidths)
	ON_COMMAND(ID_PEN_Color, &CScribblenewDoc::OnPenColor)
//	ON_COMMAND(IDD_CANVAS, &CScribblenewDoc::OnIddCanvas)
//	ON_COMMAND(ID_FILE_NEW, &CScribblenewDoc::OnFileNew)
ON_UPDATE_COMMAND_UI(ID_PEN_THICKLINE, &CScribblenewDoc::OnUpdatePenThickline)
END_MESSAGE_MAP()


// CScribblenewDoc �غc/�Ѻc

CScribblenewDoc::CScribblenewDoc()
{
	// TODO: �b���[�J�@���غc�{���X

}

CScribblenewDoc::~CScribblenewDoc()
{
}

BOOL CScribblenewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// when open new file get default setting
	CString SettingName("Setting");
	CSize RegistrySize = CSize(AfxGetApp()->GetProfileInt(SettingName, CString("Canvas Width"), 800), AfxGetApp()->GetProfileInt(SettingName, CString("Canvas Height"), 600));
	COLORREF BackgroundColor = AfxGetApp()->GetProfileInt(SettingName, CString("Background Color"), RGB(255, 255, 255) );

    //call Canvas DLG
	CanvasDlg dlg(NULL, RegistrySize, BackgroundColor); 
	if(dlg.DoModal() == IDOK )
	{
		 m_FilePath = dlg.m_FilePath;
		
		 if (m_FilePath.IsEmpty())
		 {
			 m_IfUseImage = FALSE;
		 }
		 else
		 {
			 m_IfUseImage = TRUE;
		 }

		m_sizeDoc = CSize(dlg.m_CanvasWidthV, dlg.m_CanvasHeightV);
		m_BackgroundColor = dlg.m_BackgroundColor;
		InitDocument();
		return TRUE;
	}
	
	return FALSE;
}


// CScribblenewDoc �ǦC��

void CScribblenewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �b���[�J�x�s�{���X
		ar << (WORD)m_nThinWidth;
		ar << (WORD)m_nThickWidth;
		ar << m_sizeDoc;
		ar << (COLORREF)m_BackgroundColor;
		ar  << m_FilePath;
	}
	else
	{
		// TODO: �b���[�J���J�{���X
		WORD w;
		ar >> w;
		m_nThinWidth = w;
		ar >> w;
		m_nThickWidth = w;
		ar >> m_sizeDoc;
		COLORREF c;
		ar >> c;
		m_BackgroundColor = c;
		ar >> m_FilePath;
	}

	m_strokeList.Serialize(ar); //
}

#ifdef SHARED_HANDLERS

// �Y�Ϫ��䴩
void CScribblenewDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �ק�o�q�{���X�Hø�s��󪺸��
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �j�M�B�z�`�����䴩
void CScribblenewDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// �]�w�Ӧۤ���ƪ��j�M���e�C
	// ���e�������ӥH ";" �j�}

	// �d��:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CScribblenewDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CScribblenewDoc �E�_

#ifdef _DEBUG
void CScribblenewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScribblenewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CScribblenewDoc �R�O
BOOL CScribblenewDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	InitDocument(); 
	return TRUE;
}

void CScribblenewDoc::DeleteContents() 
{
	while (!m_strokeList.IsEmpty())
	{
		delete m_strokeList.RemoveHead(); //remove head stroke
	}
	CDocument::DeleteContents();
}

void CScribblenewDoc::InitDocument()  //pen data setting
{   
	
	//m_nPenWidth = 2; //  2 pixel pen width
	//m_nThinWidth = 2;   //  thin pen is 2 pixels wide
	//m_nThickWidth = 4;  //  thick pen is 4 pixels wide
	//m_PenColor = RGB(0, 0, 0); // default pen color is black
	//ReplacePen();	
	
    m_bThickPen = FALSE;

	CString SettingName("Setting");
	m_nThinWidth  = AfxGetApp()->GetProfileInt(SettingName, CString("Thin Width"), 2);
	m_nThickWidth = AfxGetApp()->GetProfileInt(SettingName, CString("Thick Width"), 4);

	ReplacePen();
	//write setting information in registry
	AfxGetApp()->WriteProfileInt(SettingName, CString("Canvas Width"), m_sizeDoc.cx);
	AfxGetApp()->WriteProfileInt(SettingName, CString("Canvas Height"), m_sizeDoc.cy);
	AfxGetApp()->WriteProfileInt(SettingName, CString("Background Color"), m_BackgroundColor);

	//recall m_penCur will cause crash
  //  m_penCur.CreatePen(PS_SOLID, m_nPenWidth, RGB(0,0,0));
}

CStroke* CScribblenewDoc::NewStroke()
{
	//creat new CStroke and add into list tail
	//add pen color item, so we can save color
	CStroke* pStrokeItem = new CStroke(m_nPenWidth,m_PenColor);
	if (pStrokeItem == nullptr)
	{
		return FALSE ;
	}
	m_strokeList.AddTail(pStrokeItem);
	SetModifiedFlag();

	return pStrokeItem;
}


///////////////////////////////////////Cstroke///////////////////////////////////

IMPLEMENT_SERIAL(CStroke, CObject, 2)
	CStroke::CStroke()
{
	// This empty constructor should be used by serialization only
}

CStroke::CStroke(UINT nPenWidth, COLORREF PenColor) // add color item
{
	m_nPenWidth = nPenWidth;
	m_PenColor =  PenColor; 

	m_rectBounding.SetRectEmpty();
}

void CStroke::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_rectBounding;
		ar << (WORD)m_nPenWidth;  //save
		ar << (COLORREF)m_PenColor;//save

		m_pointArray.Serialize(ar);
	}
	else
	{    //load
		ar >> m_rectBounding;

		WORD w;
		ar >> w;
		m_nPenWidth = w;

		COLORREF c;
		ar >> c;
		m_PenColor = c;

		m_pointArray.Serialize(ar);
	}
}

BOOL CStroke::DrawStroke(CDC* pDC)
{
	CPen penStroke;
	if (!penStroke.CreatePen(PS_SOLID, m_nPenWidth, m_PenColor)) //draw with our width and color
		return FALSE;
	CPen* pOldPen = pDC->SelectObject(&penStroke);

	//repaint from stroke
	pDC->MoveTo(m_pointArray[0]);
	for (int i=1; i < m_pointArray.GetSize(); i++)
	{
		pDC->LineTo(m_pointArray[i]);
	}

	pDC->SelectObject(pOldPen);
	return TRUE;
}

void CScribblenewDoc::OnPenClearall()
{
	// TODO: Add your command handler code here
	DeleteContents();
	SetModifiedFlag();

	UpdateAllViews(NULL); 
}


void CScribblenewDoc::OnPenThickline()
{
	// TODO: Add your command handler code here
	m_bThickPen = !m_bThickPen;

	//change pen between thick and thin
	ReplacePen();
}

// replace current pen width
void CScribblenewDoc::ReplacePen(){
	m_nPenWidth = m_bThickPen? m_nThickWidth : m_nThinWidth;
	//change current pen to new width pen
	m_penCur.DeleteObject(); 
	m_penCur.CreatePen(PS_SOLID, m_nPenWidth, m_PenColor); //after change pen setting, creat a new pen to show
}

//void CScribblenewDoc::OnUpdatePenThickline(CCmdUI *pCmdUI)
//{
//	// TODO: Add your command update UI handler code here
// 	pCmdUI->SetCheck(m_bThickPen);
//}


void CScribblenewDoc::OnUpdatePenClearall(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_strokeList.IsEmpty());

}


void CScribblenewDoc::OnPenWidths()
{
	// TODO: Add your command handler code here
	CPenWidthsDlg dlg;
	// Initialize dialog data
	dlg.m_nThinWidth = m_nThinWidth;
	dlg.m_nThickWidth = m_nThickWidth;

	//if press IDOK
	if (dlg.DoModal() == IDOK)
	{
		// get dlg setting value
		m_nThinWidth = dlg.m_nThinWidth;
		m_nThickWidth = dlg.m_nThickWidth;
		//update pen width in registry
		CString SettingName("Setting");
		AfxGetApp()->WriteProfileInt(SettingName, CString("Thin Width"), m_nThinWidth);
		AfxGetApp()->WriteProfileInt(SettingName, CString("Thick Width"), m_nThickWidth);


		//Update pen
		ReplacePen();
	}

}

void CStroke::FinishStroke()
{
	
	//when after painting(LButtonUp) calculate the repainting rect.
	if (m_pointArray.GetSize()==0)//no  painting
	{
		m_rectBounding.SetRectEmpty();
		return;
	}
	CPoint pt = m_pointArray[0];
	m_rectBounding = CRect(pt.x, pt.y, pt.x, pt.y);

	for (int i=1; i < m_pointArray.GetSize(); i++)
	{
		
		pt = m_pointArray[i];
		m_rectBounding.left   = min(m_rectBounding.left, pt.x);
		m_rectBounding.right  = max(m_rectBounding.right, pt.x);
		m_rectBounding.top    = max(m_rectBounding.top, pt.y);
		m_rectBounding.bottom = min(m_rectBounding.bottom, pt.y);
	}

	//add pen width
	m_rectBounding.InflateRect(CSize(m_nPenWidth, m_nPenWidth));
	return;
}



void CScribblenewDoc::OnPenColor() //pick color
{
	// TODO: Add your command handler code here
	CColorDialog dlg(m_PenColor);
	if(dlg.DoModal() == IDOK) //allowed user pick on color
	{
		m_PenColor = dlg.GetColor();

		ReplacePen(); 
	}
}



void CScribblenewDoc::OnUpdatePenThickline(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
    pCmdUI->SetCheck(m_bThickPen);
}
