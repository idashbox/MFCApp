
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
	ON_COMMAND(ID_GRAPH_SAVE_SPANNING_TREE, &CMFCAppView::OnGraphSaveSpanningTree)
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
		auto spanningTree = pDoc->GetGraph().findSpanningTree();
		pDoc->SetSpanningTree(spanningTree);
		AfxMessageBox(_T("Остовное дерево найдено!"));
	}
	catch (const std::exception& e) {
		AfxMessageBox(CString("Ошибка: ") + e.what());
	}
}

void CMFCAppView::OnGraphSaveSpanningTree() {
	CMFCAppDoc* pDoc = GetDocument();
	if (!pDoc->HasSpanningTree()) {
		AfxMessageBox(_T("Сначала найдите остовное дерево!"));
		return;
	}

	CFileDialog dlg(FALSE, _T("txt"), _T("spanning_tree.txt"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Текстовые файлы (*.txt)|*.txt||"));

	if (dlg.DoModal() == IDOK) {
		try {
			std::string filename = CT2CA(dlg.GetPathName());
			pDoc->GetGraph().saveToFile(filename, pDoc->GetSpanningTree());
			AfxMessageBox(_T("Файл успешно сохранен!"));
		}
		catch (const std::exception& e) {
			AfxMessageBox(CString("Ошибка сохранения: ") + e.what());
		}
	}
}

void CMFCAppView::OnDraw(CDC* pDC) {
	CMFCAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CFont font;
	font.CreatePointFont(100, _T("Arial"));
	CFont* pOldFont = pDC->SelectObject(&font);

	CRect rect;
	GetClientRect(&rect);
	int y = 20;

	// Отрисовка исходного графа
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOut(10, y, _T("Исходный граф:"));
	y += 30;

	for (const auto& [vertex, neighbors] : pDoc->GetGraph().getAdjacencyList()) {
		CString str;
		str.Format(_T("Вершина %d:"), vertex);
		pDC->TextOut(20, y, str);

		CString connections;
		for (int n : neighbors) {
			CString temp;
			temp.Format(_T("%d "), n);
			connections += temp;
		}
		pDC->TextOut(40, y + 20, connections);
		y += 40;
	}

	// Отрисовка остовного дерева
	if (pDoc->HasSpanningTree()) {
		y += 40;
		pDC->SetTextColor(RGB(0, 0, 255));
		pDC->TextOut(10, y, _T("Остовное дерево:"));
		y += 30;

		for (const auto& edge : pDoc->GetSpanningTree()) {
			CString str;
			str.Format(_T("%d - %d"), edge.first, edge.second);
			pDC->TextOut(20, y, str);
			y += 20;
		}
	}

	pDC->SelectObject(pOldFont);
}
#endif //_DEBUG


// Обработчики сообщений CMFCAppView
