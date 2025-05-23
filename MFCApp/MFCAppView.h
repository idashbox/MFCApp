
// MFCAppView.h: интерфейс класса CMFCAppView
//

#pragma once


class CMFCAppView : public CView
{
protected: // создать только из сериализации
	CMFCAppView() noexcept;
	DECLARE_DYNCREATE(CMFCAppView)

// Атрибуты
public:
	CMFCAppDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CMFCAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnGraphFindSpanningTree();
	afx_msg void OnGraphSaveSpanningTree();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в MFCAppView.cpp
inline CMFCAppDoc* CMFCAppView::GetDocument() const
   { return reinterpret_cast<CMFCAppDoc*>(m_pDocument); }
#endif

