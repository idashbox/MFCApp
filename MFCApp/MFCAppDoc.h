
// MFCAppDoc.h: интерфейс класса CMFCAppDoc 
//


#pragma once
#include "Graph.h"


class CMFCAppDoc : public CDocument
{
private:
	Graph m_Graph;
protected: // создать только из сериализации
	CMFCAppDoc() noexcept;
	DECLARE_DYNCREATE(CMFCAppDoc)

// Атрибуты
public:

// Операции
public:
	Graph& GetGraph() { return m_Graph; }
	const Graph& GetGraph() const { return m_Graph; }

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
