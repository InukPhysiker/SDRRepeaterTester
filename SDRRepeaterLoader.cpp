#include "stdafx.h"
#include "SDRRepeaterLoader.h"

#define	CHECK_LOADED(MEMBER, NAME)		\
if (MEMBER == NULL)						\
{										\
	TRACE(_T("%s = FAILED\n"), NAME);	\
	_ASSERT(0);							\
	return false;						\
}

CSDRRepeaterLoader::CSDRRepeaterLoader()
{
	m_bReady        = FALSE;
	m_hLibrary      = NULL;

	m_pfnConnect           = NULL;
	m_pfnConnectDlg        = NULL;
	m_pfnConnectStatus     = NULL;
	m_pfnDataMsg           = NULL;
	m_pfnDisconnect        = NULL;
	m_pfnEnable            = NULL;
	m_pfnLogfileMsg        = NULL;
	m_pfnReturnBuffer      = NULL;
	m_pfnReturnText        = NULL;
	m_pfnSetProgram        = NULL;
	m_pfnIQDataSetCallback = NULL;
	m_pfnRadioSetCallback  = NULL;
}

CSDRRepeaterLoader::~CSDRRepeaterLoader()
{
}

const BOOL CSDRRepeaterLoader::Load()
{
	TCHAR	szFile[_MAX_PATH + 1] = _T("");
	CString	strFolder;
	CString	strFile;

	if (GetModuleFileName(NULL, szFile, _MAX_PATH) == 0)
	{
		_ASSERT(0);
		_tcscpy(szFile,AfxGetApp()->m_pszExeName);
	}

	//
	//	Remove everything after the last \ and we have the directory.
	//
	strFolder = szFile;

	int nPos = strFolder.ReverseFind(_T('\\'));
	if (nPos != -1)
		strFolder = strFolder.Left(nPos + 1);

	strFile = strFolder + _T("SDRRepeater.dll");

	m_hLibrary = LoadLibrary(strFile);

	if (m_hLibrary == NULL)
		return FALSE;

	m_pfnConnect           = reinterpret_cast<pfnSDRRepeaterConnect*>           (GetProcAddress(m_hLibrary, "SDRRepeaterConnect"));
	m_pfnConnectDlg        = reinterpret_cast<pfnSDRRepeaterConnectDlg*>        (GetProcAddress(m_hLibrary, "SDRRepeaterConnectDlg"));
	m_pfnConnectStatus     = reinterpret_cast<pfnSDRRepeaterConnectStatus*>     (GetProcAddress(m_hLibrary, "SDRRepeaterConnectStatus"));
	m_pfnDisconnect        = reinterpret_cast<pfnSDRRepeaterDisconnect*>        (GetProcAddress(m_hLibrary, "SDRRepeaterDisconnect"));
	m_pfnEnable            = reinterpret_cast<pfnSDRRepeaterEnable*>            (GetProcAddress(m_hLibrary, "SDRRepeaterEnable"));
	m_pfnDataMsg           = reinterpret_cast<pfnSDRRepeaterDataMsg*>           (GetProcAddress(m_hLibrary, "SDRRepeaterDataMsg"));
	m_pfnLogfileMsg        = reinterpret_cast<pfnSDRRepeaterLogfileMsg*>        (GetProcAddress(m_hLibrary, "SDRRepeaterLogfileMsg"));
	m_pfnReturnBuffer      = reinterpret_cast<pfnSDRRepeaterReturnBuffer*>      (GetProcAddress(m_hLibrary, "SDRRepeaterReturnBuffer"));
	m_pfnReturnText        = reinterpret_cast<pfnSDRRepeaterReturnText*>        (GetProcAddress(m_hLibrary, "SDRRepeaterReturnText"));
	m_pfnSetProgram        = reinterpret_cast<pfnSDRRepeaterSetProgram*>        (GetProcAddress(m_hLibrary, "SDRRepeaterSetProgram"));
	m_pfnIQDataSetCallback = reinterpret_cast<pfnSDRRepeaterIQDataSetCallback*> (GetProcAddress(m_hLibrary, "SDRRepeaterIQDataSetCallback"));
	m_pfnRadioSetCallback  = reinterpret_cast<pfnSDRRepeaterRadioSetCallback*>  (GetProcAddress(m_hLibrary, "SDRRepeaterRadioSetCallback"));

	CHECK_LOADED(m_pfnConnect,           _T("SDRRepeaterConnect"));
	CHECK_LOADED(m_pfnConnectDlg,        _T("SDRRepeaterConnectDlg"));
	CHECK_LOADED(m_pfnConnectStatus,     _T("SDRRepeaterConnectStatus"));
	CHECK_LOADED(m_pfnDisconnect,        _T("SDRRepeaterDisconnect"));
	CHECK_LOADED(m_pfnEnable,            _T("SDRRepeaterEnable"));
	CHECK_LOADED(m_pfnDataMsg,           _T("SDRRepeaterDataMsg"));
	CHECK_LOADED(m_pfnLogfileMsg,        _T("SDRRepeaterLogfileMsg"));
	CHECK_LOADED(m_pfnReturnBuffer,      _T("SDRRepeaterReturnBuffer"));
	CHECK_LOADED(m_pfnReturnText,        _T("SDRRepeaterReturnText"));
	CHECK_LOADED(m_pfnSetProgram,        _T("SDRRepeaterSetProgram"));
	CHECK_LOADED(m_pfnIQDataSetCallback, _T("SDRRepeaterIQDataSetCallback"));
	CHECK_LOADED(m_pfnRadioSetCallback,  _T("SDRRepeaterRadioSetCallback"));

	m_bReady = TRUE;

	return true;
}

