#pragma once


class CMyString : public CString
{
public:
	//	Constructs empty CString
	CMyString();

	//	Copy constructor
	CMyString(const CString& stringSrc);

	//	From a single character
	CMyString(TCHAR ch, int nRepeat = 1);

	//	From an ANSI string (converts to TCHAR)
	CMyString(LPCSTR lpsz);

	//	From a UNICODE string (converts to TCHAR)
	CMyString(LPCWSTR lpsz);

	//	Subset of characters from an ANSI string (converts to TCHAR)
	CMyString(LPCSTR lpch, int nLength);

	//	Subset of characters from a UNICODE string (converts to TCHAR)
	CMyString(LPCWSTR lpch, int nLength);

	//	From unsigned characters
	CMyString(const unsigned char* psz);

	void	FormatFrequency(const UINT nNumber) { FormatNumber(nNumber, _T('.')); }
	void	FormatFrequency64(const __int64 nNumber) { FormatNumber64(nNumber, _T('.')); }
	void	FormatFrequencyEx(const UINT nHertz);
	void	FormatFrequency64Ex(const __int64 n64Hertz);
	void	FormatFrequency64Min(const __int64 n64Hertz);
	void	FormatNumber(const UINT  nNumber, const TCHAR tDelim = _T(','));
	void	FormatNumber64(const __int64 nNumber, const TCHAR tDelim);
	void	FormatDeltaTime(const UINT nSeconds);
	LPSTR	MultiByte();
	LPSTR	UTF8();
	void	CreateArray(CStringArray& astrArray, const TCHAR t);
	void	CreateArrayEx(CStringArray& astrArray, LPCTSTR lpszDelim);
	void	LoadUTF8(const char* pszMultiByte);
};

