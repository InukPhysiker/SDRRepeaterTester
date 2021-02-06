#pragma once

//
//	CMyMFCListCtrl
//
class CMyMFCListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMyMFCListCtrl)

public:
	CMyMFCListCtrl();
	virtual ~CMyMFCListCtrl();

	void		Autosize(const int nPadding = 0);
	void		AutosizeUseHeader(const int nPadding = 0);
	void		AutosizeColumnsUseHeader(const int nColumn, const int nPadding = 0);
	void		AutosizeLastColumn();

	void		FullRowSelect();
	void		CheckBoxes();
	const BOOL	AnyChecked()  const;
	const BOOL	AnyEntries()  const { return (GetItemCount()  >  0)  ? TRUE : FALSE; }
	const BOOL	AnySelected() const { return (FirstSelected() >= 0) ? TRUE : FALSE; }
	const BOOL	OneSelected() const;
	const BOOL	IsSelected(const int nItem) const;
	const int	FirstSelected() const;
	void		SelectItem(const int  nItem, 
						   const BOOL bState);
	void		DeselectAll();
	const BOOL	Swap(const int a, const int b);
	const BOOL	IsChecked(const int nItem) const;
	const int	GetImage(const int nItem) const;
	const UINT	NumColumns() const;
	void		SetImage(const int  nItem,
						 const int  nImage,
						 const BOOL bUpdate);
	const BOOL	OneSelectedNotLast()  const;
	const BOOL	OneSelectedNotFirst() const;
	void		SetChecked(const int nItem, const BOOL bCheck);
	void		RemoveSelected();
	void		SaveAsFile(LPCTSTR lpszFilename);
	void		ViewAsFile(LPCTSTR lpszFilename);
	void		BuildArray(CStringArray& astrText);

	const int	SortedColumn()     const { return m_iSortedColumn;     }
	const BOOL	Ascending()        const { return m_bAscending;        }
	const BOOL	MarkSortedColumn() const { return m_bMarkSortedColumn; }

protected:
	DECLARE_MESSAGE_MAP()
};

//
//	CMyMFCListCtrl
//
class CFlickerFreeListCtrl : public CMyMFCListCtrl
{
	DECLARE_DYNAMIC(CFlickerFreeListCtrl)

public:
	CFlickerFreeListCtrl();
	virtual ~CFlickerFreeListCtrl();

	COLORREF	OnGetCellTextColor(int nRow, int nColumn);
	COLORREF	OnGetCellBkColor(int nRow, int nColumn);
	void		SetColShadeRate(const int nRate) { m_nColShadeRate = nRate; }
	void		SetRowShadeRate(const int nRate) { m_nRowShadeRate = nRate; }
	void		SetShadeColumn(const int nCol)   { m_anShadeColumns.SetAtGrow(nCol, TRUE); }
	void		SetShadeColumnEx(const int nCol) { m_anShadeColumns.RemoveAll(); m_anShadeColumns.SetAtGrow(nCol, TRUE); }
	void		HighlightRow(const int nRow, const COLORREF colBack = RGB(0,0,0),  const COLORREF colText = RGB(0,0,0))
	{
		m_nHighlightRow    = nRow;
		m_colHighlightBack = colBack;
		m_colHighlightText = colText;
	}
	int			m_nColShadeRate;
	int			m_nRowShadeRate;
	CUIntArray	m_anShadeColumns;

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	int			m_nHighlightRow;
	COLORREF	m_colHighlightBack;
	COLORREF	m_colHighlightText;
};
