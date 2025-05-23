
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
    ON_WM_SIZE()
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

    CRect rect;
    GetClientRect(&rect);
    int width = rect.Width();
    int height = rect.Height();

    // Конфигурация отрисовки
    const int NODE_RADIUS = 25;
    const int SPACING = 100;
    const COLORREF NODE_COLOR = RGB(100, 200, 100);
    const COLORREF EDGE_COLOR = RGB(50, 50, 50);
    const COLORREF SPANNING_EDGE_COLOR = RGB(200, 50, 50);

    // Рассчет позиций нод
    std::map<int, CPoint> nodePositions;
    int count = 0;
    int cols = sqrt(pDoc->GetGraph().getAdjacencyList().size()) + 1;

    for (const auto& [vertex, _] : pDoc->GetGraph().getAdjacencyList()) {
        int row = count / cols;
        int col = count % cols;
        nodePositions[vertex] = CPoint(
            SPACING + col * (width - 2 * SPACING) / cols,
            SPACING + row * (height - 2 * SPACING) / cols
        );
        count++;
    }

    // Отрисовка ребер исходного графа
    CPen edgePen(PS_SOLID, 1, EDGE_COLOR);
    CPen* oldPen = pDC->SelectObject(&edgePen);

    for (const auto& [vertex, neighbors] : pDoc->GetGraph().getAdjacencyList()) {
        CPoint start = nodePositions[vertex];
        for (int neighbor : neighbors) {
            CPoint end = nodePositions[neighbor];
            pDC->MoveTo(start);
            pDC->LineTo(end);
        }
    }

    // Отрисовка остовного дерева
    if (pDoc->HasSpanningTree()) {
        CPen spanPen(PS_SOLID, 3, SPANNING_EDGE_COLOR);
        pDC->SelectObject(&spanPen);

        for (const auto& edge : pDoc->GetSpanningTree()) {
            CPoint start = nodePositions[edge.first];
            CPoint end = nodePositions[edge.second];
            pDC->MoveTo(start);
            pDC->LineTo(end);
        }
    }

    // Отрисовка нод
    CBrush nodeBrush(NODE_COLOR);
    CBrush* oldBrush = pDC->SelectObject(&nodeBrush);

    for (const auto& [vertex, pos] : nodePositions) {
        // Рисуем круг
        pDC->Ellipse(
            pos.x - NODE_RADIUS,
            pos.y - NODE_RADIUS,
            pos.x + NODE_RADIUS,
            pos.y + NODE_RADIUS
        );

        // Текст ноды
        CString label;
        label.Format(_T("%d"), vertex);
        pDC->SetBkMode(TRANSPARENT);
        pDC->TextOut(
            pos.x - NODE_RADIUS / 2,
            pos.y - NODE_RADIUS / 2,
            label
        );
    }

    // Восстановление контекста
    pDC->SelectObject(oldPen);
    pDC->SelectObject(oldBrush);
}

void CMFCAppView::OnSize(UINT nType, int cx, int cy) {
    CView::OnSize(nType, cx, cy);
    Invalidate(); // Перерисовать при изменении размеров
}
#endif //_DEBUG


// Обработчики сообщений CMFCAppView
