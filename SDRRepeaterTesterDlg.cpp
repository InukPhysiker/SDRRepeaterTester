
// SDRRepeaterTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SDRRepeaterTester.h"
#include "SDRRepeaterTesterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	KB_BYTES			1024
#define	UPDATE_ID			100
#define	UPDATE_INTERVAL		100
#define	TEXT_POINT_SIZE		10
#define TMP_TXT_FILENAME	_T("SDRRepeaterLogfile.txt")
#define	MAX_ITEMS			2000

extern UINT g_nMsgAsync   = RegisterWindowMessage(_T("CSDRRepeaterTesterDlg::g_nMsgAsync"));
extern UINT g_nMsgLogfile = RegisterWindowMessage(_T("CSDRRepeaterTesterDlg::g_nMsgLogfile"));

extern CSDRRepeaterTesterApp	theApp;

CSDRRepeaterTesterDlg*	m_pDbg = NULL;

static enum eColumns
{
	COL_TIME,
	COL_TEXT,
	COL_NONE,
};

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

//
//	CSDRRepeaterTesterDlg dialog
//
void CALLBACK IQDataCallback(const void* pData,
							 const int   nDataType,
							 const int   nSamples)
{
	m_pDbg->DlgIQDataCallback(pData, nDataType, nSamples);
}

void CALLBACK RadioCallback(LPCTSTR       lpszRadio,
							const __int64 n64FrequencyHz,
							const UINT    nBandwidth)
{
	m_pDbg->DlgRadioCallback(lpszRadio, n64FrequencyHz, nBandwidth);
}

CSDRRepeaterTesterDlg::CSDRRepeaterTesterDlg(CWnd* pParent /*=NULL*/) : CDialogEx(CSDRRepeaterTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSamples     = 0;
	m_n64Bytes     = 0;
	m_n64Frequency = 0;
	m_nBandwidth   = 0;
	m_strRadio     = _T("");
	m_strData      = _T("");
	m_strName      = _T("");
}

void CSDRRepeaterTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_RADIO, m_staticRadio);
	DDX_Control(pDX, IDC_DATA, m_staticData);
	DDX_Text(pDX, IDC_RADIO, m_strRadio);
	DDX_Text(pDX, IDC_DATA, m_strData);
}

BEGIN_MESSAGE_MAP(CSDRRepeaterTesterDlg, CDialogEx)
	ON_MESSAGE(WM_KICKIDLE, CSDRRepeaterTesterDlg::OnKickIdle)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CSDRRepeaterTesterDlg::OnNMClickViewLogfile)

	ON_BN_CLICKED(IDC_BUTTON1, &CSDRRepeaterTesterDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_BUTTON2, &CSDRRepeaterTesterDlg::OnBnClickedDisconnect)

	ON_REGISTERED_MESSAGE(g_nMsgAsync,   &CSDRRepeaterTesterDlg::OnMsgAsync)
	ON_REGISTERED_MESSAGE(g_nMsgLogfile, &CSDRRepeaterTesterDlg::OnMsgLogfile)

	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//
//	CSDRRepeaterTesterDlg message handlers
//
BOOL CSDRRepeaterTesterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//
	//	Add "About..." menu item to system menu.
	//
	//	IDM_ABOUTBOX must be in the system command range.
	//
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL	bNameValid;
		CString	strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);

		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//
	//	Set the icon for this dialog.  The framework does this automatically
	//	when the application's main window is not a dialog.
	//
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//
	//	Create a non-proportional font with custom height.
	//
	LOGFONT lfRegular       = { 0 };
	LPCTSTR	lpszFaceNames[] = { _T("Consolas"), _T("Lucida"), _T("console"), _T("Courier New") };

	GetGlobalData()->fontRegular.GetLogFont(&lfRegular);

	for (int nFont = 0; nFont < ARRAYSIZE(lpszFaceNames); nFont++)
	{
		LOGFONT lfSet = { 0 };
		LOGFONT lfGet = { 0 };

		memset(&lfSet, 0, sizeof(LOGFONT));

		lfSet.lfCharSet = ANSI_CHARSET;
//		lfSet.lfHeight  = lfRegular.lfHeight;
		lfSet.lfHeight = -MulDiv(TEXT_POINT_SIZE, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY), 72);
		lfSet.lfWeight  = lfRegular.lfWeight;
		lfSet.lfItalic  = lfRegular.lfItalic;

		lstrcpy(lfSet.lfFaceName, lpszFaceNames[nFont]);
	
		VERIFY(m_fontConsolas.CreateFontIndirect(&lfSet));

		//
		//	Confirm created with required facename.
		//
		m_fontConsolas.GetLogFont(&lfGet);

		CString	strSet = lfSet.lfFaceName;
		CString	strGet = lfGet.lfFaceName;

		if (strSet.CompareNoCase(strGet) == 0)
			break;
	}

	//
	//	Apply font.
	//
	m_list.       SetFont(&m_fontConsolas);
	m_staticData. SetFont(&m_fontConsolas);
	m_staticRadio.SetFont(&m_fontConsolas);

	//
	//	List - prepare for a lot of entries and insert columns.
	//
	m_list.SetItemCount(MAX_ITEMS);
	m_list.InsertColumn(COL_TIME, _T("Time"), LVCFMT_LEFT);
	m_list.InsertColumn(COL_TEXT, _T("Text"), LVCFMT_LEFT);
	m_list.InsertColumn(COL_NONE, _T(""),     LVCFMT_LEFT);
	m_list.AutosizeUseHeader(10);

	//
	//	Load the DLL.
	//
	m_pDbg = this;

	if (!Loader().Load())
	{
		MessageBox(_T("SDRRepeater.dll was not found!"));
		return FALSE;
	}

	Loader().m_pfnLogfileMsg(this, g_nMsgLogfile);
	Loader().m_pfnIQDataSetCallback(IQDataCallback);
	Loader().m_pfnRadioSetCallback(RadioCallback);

	//
	//	Get program name from the dialog title, this in turn comes
	//	from the resource file.
	//
	CString	strTitle;
	GetWindowText(strTitle);
	Loader().m_pfnSetProgram(strTitle);

	//
	//	Update timer.
	//
	SetTimer(UPDATE_ID, UPDATE_INTERVAL, NULL);

	return TRUE;
}

//
//	Data cast to Ipp32fc (IQ stream).
//
void CSDRRepeaterTesterDlg::DlgIQDataCallback(const void* pData,
											  const int   nDataSize,
											  const int   nSamples)
{
	m_CritSect.Lock();

	m_nSamples += nSamples;
	m_n64Bytes += (nSamples * 2 * nDataSize / 8);

	m_CritSect.Unlock();

//	PostMessage(WM_KICKIDLE);
}

void CSDRRepeaterTesterDlg::DlgRadioCallback(LPCTSTR       lpszRadio,
											 const __int64 n64FrequencyHz,
											 const UINT    nBandwidth)
{
	m_CritSect.Lock();

	m_strName      = lpszRadio;
	m_n64Frequency = n64FrequencyHz;
	m_nBandwidth   = nBandwidth;

	m_CritSect.Unlock();
}

//
//	Idle processing.
//
LRESULT CSDRRepeaterTesterDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CSDRRepeaterTesterDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case UPDATE_ID:
		UpdateStatus();
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CSDRRepeaterTesterDlg::UpdateStatus()
{
	CString		strData;
	CString		strRadio;
	CMyString	strStg1;
	CMyString	strBandwidth;
	CMyString	strFrequency;
	TCHAR		szTextMEM[256] = _T("");
	double		dBytes         = 0;

	m_CritSect.Lock();

	//
	//	Data.
	//
	strStg1.FormatNumber  (m_nSamples, _T(','));

	dBytes  = (double)m_n64Bytes;

	if (dBytes < (KB_BYTES * KB_BYTES * KB_BYTES))
		_stprintf(szTextMEM, _T("Size: %.1f MB"), dBytes / (KB_BYTES * KB_BYTES));
	else
		_stprintf(szTextMEM, _T("Size: %.2f GB"), dBytes / (KB_BYTES * KB_BYTES * KB_BYTES));

	strData.Format(_T("Samples: %s, %s"), strStg1, szTextMEM);

	//
	//	Radio, frequency, bandwidth.
	//
	strBandwidth.FormatFrequencyEx  (m_nBandwidth);
	strFrequency.FormatFrequency64Ex(m_n64Frequency);

	if (!m_strName.IsEmpty())
		strRadio.Format(_T("%s: %s, %s"), m_strName, strFrequency, strBandwidth);

	m_CritSect.Unlock();

	if (strData != m_strData || m_strRadio != strRadio)
	{
		m_strData  = strData;
		m_strRadio = strRadio;
		UpdateData(FALSE);
	}
}

void CSDRRepeaterTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSDRRepeaterTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSDRRepeaterTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSDRRepeaterTesterDlg::OnBnClickedConnect()
{
	m_list.DeleteAllItems();

	m_CritSect.Lock();
	m_nSamples = 0;
	m_n64Bytes = 0;
	m_CritSect.Unlock();

	Loader().m_pfnConnectDlg(this);
}

void CSDRRepeaterTesterDlg::OnBnClickedDisconnect()
{
	Loader().m_pfnDisconnect();
}

afx_msg LRESULT CSDRRepeaterTesterDlg::OnMsgAsync(WPARAM wParam, LPARAM lParam)
{
	_ASSERT(0);
	return 0;
}

afx_msg LRESULT CSDRRepeaterTesterDlg::OnMsgLogfile(WPARAM wParam, LPARAM lParam)
{
	TCHAR*	pszTitle = (TCHAR*)wParam;
	TCHAR*	pszText  = (TCHAR*)lParam;
	CString	strText;

	if (pszTitle != NULL)
		strText.Format(_T("%s> %s"), pszTitle, pszText);
	else
		strText.Format(_T("%s"), pszText);

	//delete pszText;
	//delete pszTitle;
	Loader().m_pfnReturnText(pszText);
	Loader().m_pfnReturnText(pszTitle);

	int	nCount = m_list.GetItemCount();
	int	nIndex = m_list.InsertItem(nCount, _T(""));

	m_list.SetItemText(nIndex, COL_TIME, CTime::GetCurrentTime().Format(_T("%H:%M:%S")));
	m_list.SetItemText(nIndex, COL_TEXT, strText);
	m_list.EnsureVisible(m_list.GetItemCount() - 1, FALSE);
	m_list.AutosizeUseHeader(10);

	//
	//	Don't allow list to get too big.
	//
	if (m_list.GetItemCount() >= MAX_ITEMS)
	{
		for (int nItem = 0; nItem < (MAX_ITEMS/4); nItem++)
			m_list.DeleteItem(0);
	}

	return 0;
}

void CSDRRepeaterTesterDlg::OnClose()
{
	Loader().Unload();

	CDialogEx::OnClose();
}


void CSDRRepeaterTesterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_list.m_hWnd == NULL)
		return;

	CRect	rectList;
	m_list.GetWindowRect(&rectList);
	ScreenToClient(&rectList);

	rectList.right  = cx;
	rectList.bottom = cy;

	m_list.MoveWindow(&rectList);
}

void CSDRRepeaterTesterDlg::OnNMClickViewLogfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString	strTmpFolder = _tgetenv(_T("TEMP"));
	CString	strTmpFile;

	strTmpFile.Format(_T("%s\\%s"), strTmpFolder, TMP_TXT_FILENAME);

	m_list.ViewAsFile(strTmpFile);

	*pResult = 0;
}
