
// SDRRepeaterTesterDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSDRRepeaterTesterDlg dialog
class CSDRRepeaterTesterDlg : public CDialogEx
{
// Construction
public:
	CSDRRepeaterTesterDlg(CWnd* pParent = NULL);	// standard constructor
	CSDRRepeaterLoader&	Loader() { return m_loader; }

// Dialog Data
	enum { IDD = IDD_SDRREPEATERTESTER_DIALOG };

	void	DlgIQDataCallback(const void* pData,
							  const int   nDataSize,
							  const int   nSamples);
	void	DlgRadioCallback(LPCTSTR       lpszRadio,
							 const __int64 n64FrequencyHz,
							 const UINT    nBandwidth);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL	OnInitDialog();
	afx_msg void	OnBnClickedConnect();
	afx_msg void	OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void	OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT	OnMsgAsync(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMsgLogfile(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnBnClickedDisconnect();
	afx_msg void	OnClose();
	afx_msg LRESULT	OnKickIdle(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnNMClickViewLogfile(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:
	void	UpdateStatus();

private:
	CCriticalSection		m_CritSect;
	CFont					m_fontConsolas;
	CStatic					m_staticRadio;
	CStatic					m_staticData;
	CString					m_strRadio;
	CString					m_strData;
	CString					m_strName;
	CFlickerFreeListCtrl	m_list;
	CSDRRepeaterLoader		m_loader;
	int						m_nSamples;
	__int64					m_n64Bytes;
	__int64					m_n64Frequency;
	UINT					m_nBandwidth;
};
