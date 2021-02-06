// MyMFCListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyMFCListCtrl.h"


// CMyMFCListCtrl

IMPLEMENT_DYNAMIC(CMyMFCListCtrl, CMFCListCtrl)

CMyMFCListCtrl::CMyMFCListCtrl()
{
}

CMyMFCListCtrl::~CMyMFCListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyMFCListCtrl, CMFCListCtrl)
END_MESSAGE_MAP()

// CMyMFCListCtrl message handlers

void CMyMFCListCtrl::Autosize(const int nPadding)
{
	if (m_hWnd)
	{
		SetRedraw(FALSE);

		int	nHdrCount = GetHeaderCtrl().GetItemCount();

		for (int nItem = 0; nItem < GetHeaderCtrl().GetItemCount(); nItem++)
		{
			SetColumnWidth(nItem, LVSCW_AUTOSIZE);

			if ((nPadding > 0) && (nItem < (nHdrCount - 1)))
			{
				int	nWidth = GetColumnWidth(nItem) + nPadding;

				SetColumnWidth(nItem, nWidth);
			}
		}

		SetRedraw(TRUE);
	}
}

void CMyMFCListCtrl::AutosizeUseHeader(const int nPadding)
{
	if (m_hWnd)
	{
		SetRedraw(FALSE);

		int	nHdrCount = GetHeaderCtrl().GetItemCount();

		for (int nItem = 0; nItem < nHdrCount; nItem++)
		{
			SetColumnWidth(nItem, LVSCW_AUTOSIZE_USEHEADER);

			if ((nPadding > 0) && (nItem < (nHdrCount - 1)))
			{
				int	nWidth = GetColumnWidth(nItem) + nPadding;

				SetColumnWidth(nItem, nWidth);
			}
		}

		SetRedraw(TRUE);
	}
}

void CMyMFCListCtrl::AutosizeColumnsUseHeader(const int nColumn,
											  const int nPadding)
{
	SetRedraw(FALSE);

	int	nHdrCount = GetHeaderCtrl().GetItemCount();
	int nItem     = nColumn;

	SetColumnWidth(nItem, LVSCW_AUTOSIZE_USEHEADER);

	if ((nPadding > 0) && (nItem < (nHdrCount - 1)))
	{
		int	nWidth = GetColumnWidth(nItem) + nPadding;

		SetColumnWidth(nItem, nWidth);
	}

	SetRedraw(TRUE);
}

void CMyMFCListCtrl::AutosizeLastColumn()
{
	if (m_hWnd)
	{
		int	nCount = GetHeaderCtrl().GetItemCount();

		SetColumnWidth(nCount - 1, LVSCW_AUTOSIZE_USEHEADER);
	}
}

void CMyMFCListCtrl::FullRowSelect()
{
	DWORD	dwExtStyle = GetExtendedStyle();

	SetExtendedStyle(dwExtStyle | LVS_EX_FULLROWSELECT);
}

void CMyMFCListCtrl::CheckBoxes()
{
	DWORD	dwExtStyle = GetExtendedStyle();
	SetExtendedStyle(dwExtStyle | LVS_EX_CHECKBOXES);
}

//
//	Is this item selected?
//
const BOOL CMyMFCListCtrl::IsSelected(const int nItem) const
{
	UINT nState = GetItemState(nItem, LVNI_SELECTED);

	return (nState & LVNI_SELECTED) ? TRUE : FALSE;
}

//
//	Return first selected entry.
//
const int CMyMFCListCtrl::FirstSelected() const
{
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		UINT nState = GetItemState(nItem, LVNI_SELECTED);

		if (nState & LVNI_SELECTED)
			return nItem;
	}

	return -1;
}

//
//	Exactly one entry selected?
//
const int CMyMFCListCtrl::OneSelected() const
{
	int	nCount = 0;

	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		UINT nState = GetItemState(nItem, LVNI_SELECTED);

		if (nState & LVNI_SELECTED)
			nCount++;
	}

	return (nCount == 1);
}

//
//	Select an item.
//
void CMyMFCListCtrl::SelectItem(const int  nItem, 
							    const BOOL bState)
{
	if (GetItemCount() > nItem)
		SetItemState(nItem,
					 bState ? LVIS_SELECTED : 0,
					 LVIS_SELECTED);
}

void CMyMFCListCtrl::DeselectAll()
{
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
		SelectItem(nItem, FALSE);
}

const BOOL CMyMFCListCtrl::AnyChecked() const
{
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		if (IsChecked(nItem))
			return TRUE;
	}

	return FALSE;
}

const BOOL CMyMFCListCtrl::IsChecked(const int nItem) const
{
	UINT nState = GetItemState(nItem, INDEXTOSTATEIMAGEMASK(LVIS_SELECTED));

	return (nState == INDEXTOSTATEIMAGEMASK(LVIS_SELECTED)) ? TRUE : FALSE;
}

//
//	Get image.
//
const int CMyMFCListCtrl::GetImage(const int nItem) const
{
	LV_ITEM lv = {0};

	lv.mask  = LVIF_IMAGE;
	lv.iItem = nItem;

	GetItem(&lv);

	return lv.iImage;
}

//
//	Number of columns.
//
const UINT CMyMFCListCtrl::NumColumns() const
{
	UINT nCol  = 0;
	UINT nCols = 0;

	//
	//	Number of columns.
	//
	for (nCol = 0; ; nCol++)
	{
		LV_COLUMN lvColumn;

		lvColumn.mask = LVCF_FMT;
		if (!GetColumn(nCol, &lvColumn))
			break;

		nCols++;
	}

	return nCols;
}

//
//	Set the image.
//
void CMyMFCListCtrl::SetImage(const int  nItem,
							  const int  nImage,
							  const BOOL bUpdate)
{
	LV_ITEM lv = {0};

	lv.mask   = LVIF_IMAGE;
	lv.iItem  = nItem;
	lv.iImage = nImage;

	SetItem(&lv);

	if (bUpdate)
		Update(nItem);
}

//
//	Swap two entries.
//
const BOOL CMyMFCListCtrl::Swap(const int a, const int b)
{
	int		nCount  = GetItemCount();
	UINT	nColumn = 0;

	if (a < 0 || b < 0 || a >= nCount || b >= nCount)
		return FALSE;

	//
	//	Get.
	//
	BOOL  bCheckA  = IsChecked(a);
	BOOL  bCheckB  = IsChecked(b);
	BOOL  bSelA    = IsSelected(a);
	BOOL  bSelB    = IsSelected(b);
	DWORD dwDataA  = (DWORD)GetItemData(a);
	DWORD dwDataB  = (DWORD)GetItemData(b);
	UINT  nColumns = NumColumns();
	int   nImageA  = GetImage(a);
	int   nImageB  = GetImage(b);

	CStringArray astrArrayA;
	CStringArray astrArrayB;

	for (nColumn = 0; nColumn < nColumns; nColumn++)
	{
		astrArrayA.Add(GetItemText(a,nColumn));
		astrArrayB.Add(GetItemText(b,nColumn));
	}

	//
	//	Set.
	//
	SetCheck(a, bCheckB);
	SetCheck(b, bCheckA);
	SelectItem(a, bSelB);
	SelectItem(b, bSelA);
	SetImage(a, nImageB, TRUE);
	SetImage(b, nImageA, TRUE);
	SetItemData(a, dwDataB);
	SetItemData(b, dwDataA);

	for (nColumn = 0; nColumn < nColumns; nColumn++)
	{
		SetItemText(a, nColumn, astrArrayB.GetAt(nColumn));
		SetItemText(b, nColumn, astrArrayA.GetAt(nColumn));
	}

	//
	//	Visibility.
	//
	if (bSelA)
		EnsureVisible(b, FALSE);
	else if (bSelB)
		EnsureVisible(a, FALSE);

	return TRUE;
}

const BOOL CMyMFCListCtrl::OneSelectedNotLast() const
{
	int	nSize = GetItemCount();
	int	nSeln = FirstSelected();

	return (nSeln >= 0 && nSeln < (nSize - 1));
}

const BOOL CMyMFCListCtrl::OneSelectedNotFirst() const
{
	int	nSeln = FirstSelected();

	return (nSeln > 0);
}

void CMyMFCListCtrl::SetChecked(const int  nItem,
								const BOOL bCheck)
{
	UINT nState = bCheck ? INDEXTOSTATEIMAGEMASK(LVIS_SELECTED) : INDEXTOSTATEIMAGEMASK(LVIS_FOCUSED);

	SetItemState(nItem, nState, LVIS_STATEIMAGEMASK);
}

void CMyMFCListCtrl::RemoveSelected()
{
	int	nCount = GetItemCount();
	int	nItem  = 0;

	for (nItem = (nCount - 1); nItem >= 0; nItem--)
	{
		if (IsSelected(nItem))
			DeleteItem(nItem);
	}
}

void CMyMFCListCtrl::SaveAsFile(LPCTSTR lpszFilename)
{
	CStringArray	astrText;

	BuildArray(astrText);

	//
	//	Save to file.
	//
	try
	{
		CStdioFile file;

		if (file.Open(lpszFilename, CFile::modeCreate | CFile::modeWrite))
		{
			//
			//	UTF-8
			//
		//	unsigned char ucBOM[3] = { 0xEF, 0xBB, 0xBF };
		//	file.Write(ucBOM, 3);

			for (int nItem = 0; nItem < astrText.GetSize(); nItem++)
				file.WriteString(astrText[nItem] + _T("\n"));

			file.Close();
		}
	}
	catch(...)
	{
	}
}

void CMyMFCListCtrl::ViewAsFile(LPCTSTR lpszFilename)
{
	CStringArray	astrText;

	BuildArray(astrText);

	//
	//	Save to file.
	//
	try
	{
		CStdioFile file;

		if (file.Open(lpszFilename, CFile::modeCreate | CFile::modeWrite))
		{
			//
			//	UTF-8
			//
		//	unsigned char ucBOM[3] = { 0xEF, 0xBB, 0xBF };
		//	file.Write(ucBOM, 3);

			for (int nItem = 0; nItem < astrText.GetSize(); nItem++)
				file.WriteString(astrText[nItem] + _T("\n"));

			file.Close();
		}
	}
	catch(...)
	{
	}

	HWND	hWnd = AfxGetMainWnd()->m_hWnd;

	ShellExecute(hWnd, _T("open"), lpszFilename, _T(""), _T(""), SW_SHOW);
}

void CMyMFCListCtrl::BuildArray(CStringArray& astrText)
{
	CWaitCursor		cur;
	BOOL			bChecks    = FALSE;
	int				nColumn    = 0;
	int				nColumns   = GetHeaderCtrl().GetItemCount();
	int				nItem      = 0;
	int				nItems     = GetItemCount();
	DWORD			dwExtStyle = GetExtendedStyle();
	CUIntArray		anAlign;
	CUIntArray		anWidth;
	CStringArray*	pastrText = new CStringArray[nColumns];

	bChecks = (dwExtStyle & LVS_EX_CHECKBOXES) ? TRUE : FALSE;

	//
	//	Get column array.
	//
	int				nOrderSize = nColumns * sizeof(int);
	LPINT			pnOrder    = (LPINT)new INT[nOrderSize];

	GetColumnOrderArray(pnOrder, nColumns);

	//
	//	Prestretch storage.
	//
	for (nColumn = 0; nColumn < nColumns; nColumn++)
		pastrText[nColumn].SetSize(0, nItems + 2);

	astrText.SetSize(0, nItems);

	for (nColumn = 0; nColumn < nColumns; nColumn++)
	{
		HDITEM	hd          = {0};
		TCHAR	szText[256] = _T("");

		hd.mask       = HDI_TEXT | HDI_FORMAT;
		hd.pszText    = szText;
		hd.cchTextMax = ARRAYSIZE(szText) - 1;

		GetHeaderCtrl().GetItem(pnOrder[nColumn], &hd);
		pastrText[nColumn].Add(szText);

		anAlign.Add(hd.fmt & HDF_JUSTIFYMASK);
	}

	for (nItem = 0; nItem < nItems; nItem++)
	{
		for (nColumn = 0; nColumn < nColumns; nColumn++)
		{
			if (nColumn == 0 && bChecks)
			{
				BOOL	bChecked  = IsChecked(nItem);
				LPCTSTR	lpszCheck = bChecked ? _T("[X] ") : _T("[_] ");

				pastrText[nColumn].Add(lpszCheck + GetItemText(nItem, pnOrder[nColumn]));
			}
			else
				pastrText[nColumn].Add(GetItemText(nItem, pnOrder[nColumn]));
		}
	}

	for (nColumn = 0; nColumn < nColumns; nColumn++)
	{
		int	nWidth = 0;

		for (nItem = 0; nItem < pastrText[nColumn].GetSize(); nItem++)
			nWidth = max(nWidth, pastrText[nColumn].GetAt(nItem).GetLength());

		anWidth.Add(nWidth);
	}

	for (nItem = 0; nItem < pastrText[0].GetSize(); nItem++)
	{
		CString	strLine;
		CString	strTmp;

		for (nColumn = 0; nColumn < nColumns; nColumn++)
		{
			CString	strText = pastrText[nColumn].GetAt(nItem);
			int		nWidth  = anWidth[nColumn];

			switch(anAlign[nColumn])
			{
			default:
			case HDF_LEFT:
				//
				//	Not needed if last column.
				//
				if (nColumn == (nColumns - 1))
					break;

				if (strText.GetLength() < nWidth)
				{
					strText.Format(_T("%s%*s"), strText, (nWidth - strText.GetLength()), _T(""));
				//	strText = strTmp;
				}

			//	while(strText.GetLength() < nWidth)
			//		strText += _T(" ");
				break;

			case HDF_RIGHT:
				while(strText.GetLength() < nWidth)
					strText = _T(" ") + strText;
				break;

			case HDF_CENTER:
				while(strText.GetLength() < nWidth)
					strText = _T(" ") + strText + _T(" ");

				strText = strText.Left(nWidth);
				break;
			}

			if (nColumn > 0)
				strLine = strLine + _T("  ");

			strLine = strLine + strText;
		}

		//
		//	Much faster than TrimRight() with massively wide columns.
		//
		//	If there's a space at the end then we'll be truncating.
		//
		if (strLine.Right(1) == _T(" "))
		{
			int		nLength    = strLine.GetLength();
			LPCTSTR lpszBuffer = strLine.GetBuffer(nLength);
			BOOL	bReleased  = FALSE;

			for (int nItem = (nLength - 1); nItem >= 0; nItem--)
			{
				if (lpszBuffer[nItem] != _T(' '))
				{
					bReleased = TRUE;
					strLine.ReleaseBuffer();
					strLine = strLine.Left(nItem + 1);
					break;
				}
			}

			//
			//	Must be released.
			//
			if (!bReleased)
				strLine.ReleaseBuffer();
		}
	//	strLine.TrimRight();
		astrText.Add(strLine);
	}

	CString	strText = astrText[0];

	for (nItem = 0; nItem < strText.GetLength(); nItem++)
		strText.SetAt(nItem, _T('-'));

	astrText.InsertAt(1, strText);

	delete	pnOrder;
}

IMPLEMENT_DYNAMIC(CFlickerFreeListCtrl, CMyMFCListCtrl)

CFlickerFreeListCtrl::CFlickerFreeListCtrl()
{
	m_nColShadeRate    = 0;
	m_nRowShadeRate    = 0;
	m_nHighlightRow    = -1;
	m_colHighlightBack = 0;
	m_colHighlightText = 0;
}

CFlickerFreeListCtrl::~CFlickerFreeListCtrl()
{
}


BEGIN_MESSAGE_MAP(CFlickerFreeListCtrl, CMyMFCListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//
//	Flickering apears, because the complete control is erased and it needs some time to fill
//	the control-area with some text/graphics or whatever. There is a really short moment, when
//	nothing can bee seen, because the background was erased but nothing is written yet. That's
//	why we don't erase the background of the control anymore. to do this, we have to override
//	the OnEraseBkgnd()-routine of the control like this:
//
BOOL CFlickerFreeListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

//
//	The trick is, to paint the complete control into a memory device-context and copy it in the
//	original DC via BitBlt(). The CMemDC-class does this work for you. Because we don't erase
//	the background anymore (see above), we need to erase the memory-device-context with the
//	background-color of the control.
//
void CFlickerFreeListCtrl::OnPaint()
{
	CPaintDC	dc(this);
	CRect		headerRect;

	//
	//	Exclude header.
	//
	GetDlgItem(0)->GetWindowRect(&headerRect);
	ScreenToClient(&headerRect);
	dc.ExcludeClipRect(&headerRect);

	CMemDC memDC(dc, this);

	CRect clip;
	memDC.GetDC().GetClipBox(&clip);
	memDC.GetDC().FillSolidRect(clip, GetSysColor(COLOR_WINDOW));
 
	DefWindowProc(WM_PAINT, (WPARAM)memDC.GetDC().m_hDC, (LPARAM)0);
}

COLORREF CFlickerFreeListCtrl::OnGetCellTextColor(int nRow, int nColumn)
{
	if (nRow == m_nHighlightRow)
		return m_colHighlightText;

	return CMyMFCListCtrl::OnGetCellTextColor(nRow, nColumn);
}

COLORREF CFlickerFreeListCtrl::OnGetCellBkColor(int nRow, int nColumn)
{
	if (nRow == m_nHighlightRow)
		return m_colHighlightBack;

	if (m_nRowShadeRate > 0)
	{
	//	if (((nRow / m_nRowShadeRate) % m_nRowShadeRate) == 1)
		if (((nRow / m_nRowShadeRate) % 2) == 1)
			return RGB(247,247,244);
	}

	if (m_nColShadeRate > 0)
	{
	//	if (((nColumn / m_nColShadeRate) % m_nColShadeRate) == 1)
		if (((nColumn / m_nColShadeRate) % 2) == 1)
			return RGB(247,247,244);
	}

	if ((nColumn < m_anShadeColumns.GetSize()) && m_anShadeColumns[nColumn])
		return RGB(247,247,244);

	return CMyMFCListCtrl::OnGetCellBkColor(nRow, nColumn);
}