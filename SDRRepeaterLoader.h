#pragma once

typedef BOOL	(__cdecl pfnSDRRepeaterConnect      )(LPCTSTR lpszAddress, const WORD wPort);
typedef BOOL	(__cdecl pfnSDRRepeaterConnectDlg   )(CWnd* pWndParent);
typedef LPCTSTR	(__cdecl pfnSDRRepeaterConnectStatus)();
typedef void	(__cdecl pfnSDRRepeaterDisconnect   )();
typedef void	(__cdecl pfnSDRRepeaterEnable       )(const BOOL bEnable, const WORD wUDPPort);
typedef void	(__cdecl pfnSDRRepeaterDataMsg      )(const HWND hWnd, const UINT nMsg);
typedef	void	(__cdecl pfnSDRRepeaterLogfileMsg   )(CWnd* pParent, UINT nMessage);
typedef void	(__cdecl pfnSDRRepeaterReturnBuffer )(Ipp32fc* p32fcBuffer, const int nSize);
typedef void	(__cdecl pfnSDRRepeaterReturnText   )(TCHAR* pszText);
typedef void	(__cdecl pfnSDRRepeaterSetProgram   )(LPCTSTR lpszProgramName);

//
//	Supply IQ data.
//
typedef	void	(CALLBACK *pfnIQDataCallback)(const void* pData, const int nDataSize, const int nSamples);
typedef	void	(__cdecl pfnSDRRepeaterIQDataSetCallback)(pfnIQDataCallback pfnCallback);

//
//	Console frequency / bandwidth changed - or just a periodic update.
//
//	0 Hz => Radio stopped.
//
typedef	void	(CALLBACK *pfnRadioCallback)(LPCTSTR lpszRadio, const __int64 n64FrequencyHz, const UINT nBandwidth);
typedef	void	(__cdecl pfnSDRRepeaterRadioSetCallback)(pfnRadioCallback pfnCallback);

class CSDRRepeaterLoader
{
public:
	CSDRRepeaterLoader();
	~CSDRRepeaterLoader();

	const BOOL	Load();
	const BOOL	Ready() const { return m_bReady; }
	void		Unload()
	{
		if (m_hLibrary != NULL)
		{
			FreeLibrary(m_hLibrary);
			m_hLibrary = NULL;
		}
	}

	BOOL	m_bReady;
	HMODULE	m_hLibrary;

	pfnSDRRepeaterConnect*			m_pfnConnect;
	pfnSDRRepeaterConnectDlg*		m_pfnConnectDlg;
	pfnSDRRepeaterConnectStatus*	m_pfnConnectStatus;
	pfnSDRRepeaterDataMsg*			m_pfnDataMsg;
	pfnSDRRepeaterDisconnect*		m_pfnDisconnect;
	pfnSDRRepeaterEnable*			m_pfnEnable;
	pfnSDRRepeaterLogfileMsg*		m_pfnLogfileMsg;
	pfnSDRRepeaterReturnBuffer*		m_pfnReturnBuffer;
	pfnSDRRepeaterReturnText*		m_pfnReturnText;
	pfnSDRRepeaterSetProgram*		m_pfnSetProgram;

	pfnSDRRepeaterIQDataSetCallback*	m_pfnIQDataSetCallback;
	pfnSDRRepeaterRadioSetCallback*		m_pfnRadioSetCallback;
};

