
// MFCAppView.cpp: реализация класса CMFCAppView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MFCApp.h"
#endif

#include "MFCAppDoc.h"
#include "MFCAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCAppView

IMPLEMENT_DYNCREATE(CMFCAppView, CView)

BEGIN_MESSAGE_MAP(CMFCAppView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCAppView::OnFilePrintPreview)
	ON_COMMAND(ID_GRAPH_FIND_SPANNING_TREE, &CMFCAppView::OnGraphFindSpanningTree)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CMFCAppView

CMFCAppView::CMFCAppView() noexcept
{
	// TODO: добавьте код создания

}

CMFCAppView::~CMFCAppView()
{
}

BOOL CMFCAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


// Печать CMFCAppView


void CMFCAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CMFCAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CMFCAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}

void CMFCAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CMFCAppView

#ifdef _DEBUG
void CMFCAppView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCAppDoc* CMFCAppView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCAppDoc)));
	return (CMFCAppDoc*)m_pDocument;
}

void CMFCAppView::OnGraphFindSpanningTree() {
	CMFCAppDoc* pDoc = GetDocument();
	try {
		auto tree = pDoc->GetGraph().findSpanningTree();
		CFileDialog dlg(FALSE, _T("txt"), _T("spanning_tree.txt"));
		if (dlg.DoModal() == IDOK) {
			// Исправленное преобразование:
			std::string filename = CT2CA(dlg.GetPathName());
			pDoc->GetGraph().saveToFile(filename, tree);
			AfxMessageBox(_T("Дерево сохранено!"));
		}
	}
	catch (const std::exception& e) {
		AfxMessageBox(CString("Ошибка: ") + e.what());
	}
}
void CMFCAppView::OnDraw(CDC* pDC) {
	CMFCAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	int y = 20;
	for (const auto& entry : pDoc->GetGraph().getAdjacencyList()) {
		int v = entry.first;
		const auto& neighbors = entry.second;

		CString str;
		str.Format(_T("Вершина %d:"), v);
		pDC->TextOut(10, y, str);
		y += 20;

		for (int n : neighbors) {
			str.Format(_T("    Связана с %d"), n);
			pDC->TextOut(30, y, str);
			y += 20;
		}
	}
}
#endif //_DEBUG


// Обработчики сообщений CMFCAppView
