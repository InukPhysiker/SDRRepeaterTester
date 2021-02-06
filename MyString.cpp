// MyComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MyString.h"

CMyString::CMyString()
{
}

CMyString::CMyString(const CString& stringSrc) : CString(stringSrc)
{
}

CMyString::CMyString(LPCWSTR lpsz) : CString(lpsz)
{
}

CMyString::CMyString(LPCSTR lpsz) : CString(lpsz)
{
}

void CMyString::FormatNumber(const UINT  nNumber,
							 const TCHAR tDelim)
{
	//
	//	As number.
	//
	Format(_T("%u"), nNumber);

	//
	//	Get length.
	//
	int nLength = GetLength();

	//
	//	Require at least 4 characters.
	//
	if (nLength < 4)
		return;

	//
	//	Prefix, guts and suffix.
	//
	CString strPrefix;
	CString strSuffix;

	int nPrefixLength = 0;
	int nSuffixLength = 0;

	TCHAR cZero = GetAt(0);
	
	//
	//	Valid prefix is - / +.
	//
	if (cZero == _T('-') || cZero == _T('+'))
	{
		strPrefix     = Left(1);
		nPrefixLength = 1;
	}

	//
	//	Look for a suffix - starts with a period.
	//
	int nPos = Find(tDelim);

	if (nPos != -1)
	{
		strSuffix     = Right(nLength - nPos);
		nSuffixLength = strSuffix.GetLength();
	}

	TCHAR cBuff1[512] = _T("");
	TCHAR cBuff2[512] = _T("");

	//
	//	Get the buffer to be worked on - this is everything except the prefix and suffix.
	//
	_tcscpy(cBuff1,Mid(nPrefixLength,nLength - nPrefixLength - nSuffixLength));

	cBuff2[0] = cBuff1[0];

	int nFrom;
	int nTo;
	int nSize = nLength - nPrefixLength - nSuffixLength;

	for (nFrom = nTo = 1; cBuff1[nFrom]; )
	{
		//
		//	If there is a multiple of 3 characters left then add a comma.
		//
		if ((nSize - nFrom) % 3 == 0)
			cBuff2[nTo++] = tDelim;
		cBuff2[nTo++] = cBuff1[nFrom++];
	}

	cBuff2[nTo] = 0;

	//
	//	Rebuild the supplied buffer.
	//
	Format(_T("%s%s%s"), strPrefix, cBuff2, strSuffix);
}

void CMyString::FormatNumber64(const __int64 nNumber,
							   const TCHAR   tDelim)
{
	//
	//	As number.
	//
	Format(_T("%I64d"), nNumber);

	//
	//	Get length.
	//
	int nLength = GetLength();

	//
	//	Require at least 4 characters.
	//
	if (nLength < 4)
		return;

	//
	//	Prefix, guts and suffix.
	//
	CString strPrefix;
	CString strSuffix;

	int nPrefixLength = 0;
	int nSuffixLength = 0;

	TCHAR cZero = GetAt(0);
	
	//
	//	Valid prefix is - / +.
	//
	if (cZero == _T('-') || cZero == _T('+'))
	{
		strPrefix     = Left(1);
		nPrefixLength = 1;
	}

	//
	//	Look for a suffix - starts with a period.
	//
	int nPos = Find(tDelim);

	if (nPos != -1)
	{
		strSuffix     = Right(nLength - nPos);
		nSuffixLength = strSuffix.GetLength();
	}

	TCHAR cBuff1[512] = _T("");
	TCHAR cBuff2[512] = _T("");

	//
	//	Get the buffer to be worked on - this is everything except the prefix and suffix.
	//
	_tcscpy(cBuff1,Mid(nPrefixLength,nLength - nPrefixLength - nSuffixLength));

	cBuff2[0] = cBuff1[0];

	int nFrom;
	int nTo;
	int nSize = nLength - nPrefixLength - nSuffixLength;

	for (nFrom = nTo = 1; cBuff1[nFrom]; )
	{
		//
		//	If there is a multiple of 3 characters left then add a comma.
		//
		if ((nSize - nFrom) % 3 == 0)
			cBuff2[nTo++] = tDelim;
		cBuff2[nTo++] = cBuff1[nFrom++];
	}

	cBuff2[nTo] = 0;

	//
	//	Rebuild the supplied buffer.
	//
	Format(_T("%s%s%s"), strPrefix, cBuff2, strSuffix);
}

void CMyString::FormatFrequencyEx(const UINT nHertz)
{
	//
	//	< 1MB ==> kHz.
	//
	if (nHertz < 1000000)
	{
		if ((nHertz % 1000) == 0)
			Format(_T("%d kHz"), nHertz / 1000);
		else
			Format(_T("%d.%03d kHz"), nHertz / 1000, nHertz % 1000);
	}

	else
	{
		if ((nHertz % 1000000) == 0)
			Format(_T("%u MHz"), nHertz / 1000000);
		else if ((nHertz % 1000) == 0)
			Format(_T("%u.%03u MHz"), nHertz / 1000000, (nHertz / 1000) % 1000);
		else
			Format(_T("%u.%06u MHz"), nHertz / 1000000, nHertz % 1000000);
	}
}

void CMyString::FormatFrequency64Ex(const __int64 n64Hertz)
{
	__int64	n64Abs = _abs64(n64Hertz);

	//
	//	< 1MB ==> kHz.
	//
	if (n64Abs < 1000)
	{
		Format(_T("%lld Hz"), n64Abs);
	}

	else if (n64Abs < 1000000)
	{
		if ((n64Abs % 1000) == 0)
			Format(_T("%lld kHz"), n64Abs / 1000);
		else
			Format(_T("%lld.%03d kHz"), n64Abs / 1000, n64Abs % 1000);
	}

	else
	{
		if ((n64Abs % 1000) == 0)
			Format(_T("%llu.%03u MHz"), n64Abs / 1000000, (n64Abs / 1000) % 1000);
		else
			Format(_T("%llu.%06u MHz"), n64Abs / 1000000, n64Abs % 1000000);
	}

	if (n64Hertz < 0)
		Insert(0, _T('-'));
}

void CMyString::FormatFrequency64Min(const __int64 n64Hertz)
{
	//
	//	< 1MB ==> kHz.
	//
	if (n64Hertz < 1000)
	{
		Format(_T("%lld Hz"), n64Hertz);
	}

	else if (n64Hertz < 1000000)
	{
		if ((n64Hertz % 1000) == 0)
			Format(_T("%lld kHz"), n64Hertz / 1000);
		else
			Format(_T("%lld.%03d kHz"), n64Hertz / 1000, n64Hertz % 1000);
	}

	else
	{
		if ((n64Hertz % 1000000) == 0)
			Format(_T("%llu MHz"),
				   (n64Hertz / 1000000));
		else if ((n64Hertz % 1000) == 0)
		{
			Format(_T("%llu.%03u"),
				   (n64Hertz / 1000000),
				   (n64Hertz / 1000) % 1000);

			TrimRight(_T('0'));
			TrimRight(_T('.'));

			*this += _T(" MHz");
		}

		/*
		else if ((n64Hertz % 100) == 0)
			Format(_T("%llu.%03u.%01u MHz"),
				   (n64Hertz / 1000000),
				   (n64Hertz / 1000) % 1000,
				   (n64Hertz / 100)  % 100);
		else
			Format(_T("%llu.%03u.%03u MHz"),
				   (n64Hertz / 1000000),
				   (n64Hertz / 1000) % 1000,
				   (n64Hertz % 1000));
				   */
		else
		{
			Format(_T("%llu.%03u.%03u"),
				(n64Hertz / 1000000),
				(n64Hertz / 1000) % 1000,
				(n64Hertz % 1000));

			TrimRight(_T('0'));

			if (Right(1) == _T("."))
				*this += _T("0");

			*this += _T(" MHz");
		}
	}
}

void CMyString::FormatDeltaTime(const UINT nSeconds)
{
	int	nDays  = nSeconds  / (60 * 60 * 24);
	int	nHours = nSeconds / (60 * 60);
	int	nMins  = nSeconds / 60;
	int	nSecs  = nSeconds;

	if (nDays != 0)
		Format(_T("%d %02d:%02d:%02d"), nDays, nHours % 24, nMins % 60, nSecs % 60);
	else
		Format(_T("%02d:%02d:%02d"), nHours % 24, nMins % 60, nSecs % 60);
}

//
//	Wide to multibyte.
//
LPSTR CMyString::MultiByte()
{
	int   nInput     = GetLength();
	int   nOutput    = (nInput * 2) + 1;
	LPSTR lpszOutput = new char [nOutput];

	//
    //  If input is null then just return the same.
	//
    if (nInput == 0)
    {
		lpszOutput[0] = 0;
        return lpszOutput;
    }

	//
    //	Convert to ANSI.
	//
	UINT nBytes = 0;

    nBytes = WideCharToMultiByte(CP_ACP,			//	ANSI code page
								 0,					//	Performance flags
								 (LPCTSTR)*this,	//	Wide input address
								 nInput,			//	Wide input size
								 lpszOutput,		//	Wide output address
								 nOutput,			//	Wide output size
								 NULL,				//	Address of default for unmappable characters  
								 NULL);				//	Address of flag set when default chararacter used
	if (nBytes == 0)
    {
		_ASSERT(0);
		lpszOutput[0] = 0;
        return lpszOutput;
    }

	//
	//	Add NULL.
	//
	lpszOutput[nBytes] = 0;

	//
	//	Done.
	//
    return lpszOutput;
}

//
//	Wide to UTF-8.
//
LPSTR CMyString::UTF8()
{
	int   nInput     = GetLength();
	int   nOutput    = (nInput * 2) + 1;
	LPSTR lpszOutput = new char [nOutput];

	//
    //  If input is null then just return the same.
	//
    if (nInput == 0)
    {
		lpszOutput[0] = 0;
        return lpszOutput;
    }

	//
    //	Convert to ANSI.
	//
	UINT nBytes = 0;

    nBytes = WideCharToMultiByte(CP_UTF8,			//	UTF-8 code page
								 0,					//	Performance flags
								 (LPCTSTR)*this,	//	Wide input address
								 nInput,			//	Wide input size
								 lpszOutput,		//	Wide output address
								 nOutput,			//	Wide output size
								 NULL,				//	Address of default for unmappable characters  
								 NULL);				//	Address of flag set when default chararacter used
	if (nBytes == 0)
    {
		_ASSERT(0);
		lpszOutput[0] = 0;
        return lpszOutput;
    }

	//
	//	Add NULL.
	//
	lpszOutput[nBytes] = 0;

	//
	//	Done.
	//
    return lpszOutput;
}

void CMyString::CreateArrayEx(CStringArray& astrArray,
							  LPCTSTR       lpszDelim)
{
	TCHAR*	pszText = new TCHAR[GetLength() + 1];
	_tcscpy(pszText, (LPCTSTR)*this);

	astrArray.RemoveAll();

	for (int nToken = 0; ; nToken++)
	{
		LPTSTR lpszToken = (nToken == 0) ? wcstok(pszText, lpszDelim) : wcstok(NULL, lpszDelim);

		if (lpszToken == NULL)
			break;

		astrArray.Add(lpszToken);
	}

	delete pszText;
}

void CMyString::CreateArray(CStringArray& astrArray,
							const TCHAR   t)
{
	astrArray.RemoveAll();
	astrArray.SetSize(0, 100);

	int nLength = GetLength();
	int nPos    = 0;
	int nStart  = 0;

	if (nLength)
	{
		for (nPos = 0; nPos <= nLength; nPos++)
		{
			if (nPos == nLength)
			{
				astrArray.Add(Mid(nStart,nPos - nStart));
				break;
			}

			if (GetAt(nPos) == t)
			{
				astrArray.Add(Mid(nStart,nPos - nStart));
				nStart = nPos + 1;
			}
		}
	}
}

void CMyString::LoadUTF8(const char* pszMultiByte)
{
	//
	//	Convert to ANSI - get in a char*.
	//
	int		nInput   = (int)strlen(pszMultiByte);
	int		nOutput  = 0;
	DWORD	dwFlags  = MB_ERR_INVALID_CHARS;
	LPTSTR	lpszWide = NULL;
	int		nWide    = 0;

	//
	//  First find number of bytes required...
	//
	nWide = MultiByteToWideChar(CP_UTF8,		//	Code page
								dwFlags,		//	Character-type options
								pszMultiByte,	//	String to map
								(int)nInput,	//	Number of bytes in string
								NULL,			//	Wide-character buffer
								0);				//	Size of buffer = 0 -> get size

	if (nWide == 0)
	{
		//
		//	No translation - use what we have.
		//
		if (GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
		{
			*this = pszMultiByte;
			return;
		}
	}

	nOutput  = nWide + 1;
	lpszWide = new TCHAR[nOutput];
	ZeroMemory(lpszWide, nOutput * sizeof(TCHAR));

	//
	//	Convert.
	//
	nWide = MultiByteToWideChar(CP_UTF8,					// code page
								dwFlags,					// character-type options
								(const char*)pszMultiByte,	// string to map
								(int)nInput,				// number of bytes in string
								lpszWide,					// wide-character buffer
								nOutput);					// size of buffer

	//
	//	Add NULL.
	//
	lpszWide[nWide] = 0;

	*this = lpszWide;

	delete [] lpszWide;
}
