
// ScribblenewView.h : CScribblenewView ���O������
//

#pragma once


class CScribblenewView : public CScrollView
{
protected: // �ȱq�ǦC�ƫإ�
	CScribblenewView();
	DECLARE_DYNCREATE(CScribblenewView)

// �ݩ�
public:
	CScribblenewDoc* GetDocument() const;
	virtual void OnInitialUpdate();
	CDC MemDC; 
	CRgn Canvas ;
protected:
	CStroke*    m_pStrokeCur;   // stroke in progress
	CPoint      m_ptPrev;       // last mouse point in stroke 
private:
	CDC m_dcMemory;
	CBitmap bmpCanvas;
	CBitmap pic;
	CImage OrgImg;
	CImage DispImg;
	CImage AnsImg;
	CImage Y;
	CImage U;
	CImage V;
	bool m_bShowImage;
	bool SetState;
	CString CTitle;

// �@�~
public:

// �мg
public:
	virtual void OnDraw(CDC* pDC);  // �мg�H�yø���˵�
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �{���X��@
public:
	virtual ~CScribblenewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ͪ��T�������禡
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* pHint);
	void ResyncScrollSizes();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // ScribblenewView.cpp ������������
inline CScribblenewDoc* CScribblenewView::GetDocument() const
   { return reinterpret_cast<CScribblenewDoc*>(m_pDocument); }
#endif

