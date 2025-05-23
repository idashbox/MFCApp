
// MFCAppDoc.h: интерфейс класса CMFCAppDoc 
//


#pragma once
#include "Graph.h"


class CMFCAppDoc : public CDocument
{
private:
	Graph m_Graph;
	std::vector<std::pair<int, int>> m_SpanningTree;
protected: // создать только из сериализации
	CMFCAppDoc() noexcept;
	DECLARE_DYNCREATE(CMFCAppDoc)

// Атрибуты
public:

// Операции
public:
	Graph& GetGraph() { return m_Graph; }
	const Graph& GetGraph() const { return m_Graph; }
	void SetSpanningTree(const std::vector<std::pair<int, int>>& tree) {
		m_SpanningTree = tree;
		SetModifiedFlag(TRUE);
		UpdateAllViews(NULL);
	}
	const std::vector<std::pair<int, int>>& GetSpanningTree() const { return m_SpanningTree; }
	bool HasSpanningTree() const { return !m_SpanningTree.empty(); }

// Переопределение
public:
	virtual BOOL OnNewDocument();
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CMFCAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
