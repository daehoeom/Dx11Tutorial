#pragma once

#include <Windows.h>
// singleton include list
#include "CameraClass.h"

// template delete
template <typename _Data>
inline void safe_delete(_Data& d)
{
	if (d)
	{
		delete (d);
		(d) = nullptr;
	}
}

template <typename _Data>
inline void safe_release(_Data& d)
{
	if (d)
	{
		(d)->Release();
		(d) = nullptr;
	}
}

template <typename _Data>
inline void safe_delete_array(_Data& d)
{
	if (d)
	{
		delete[](d);
		(d) = nullptr;
	}
}

template <typename _Data>
inline void safe_delete_destroy(_Data& d)
{
	if (d)
	{
		(d)->Destroy();
		safe_delete(d);
	}
}

// dxtrace
#if defined(DEBUG) || defined(_DEBUG)
#ifndef HR
#define HR(x)			\
{						\
	HRESULT hr = (x);	\
	if (FAILED(hr))		\
	{					\
		LPWSTR output;	\
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |	\
			FORMAT_MESSAGE_IGNORE_INSERTS |			\
			FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
			NULL,									\
			hr,										\
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
			(LPTSTR)&output,	\
			0,	\
			NULL);	\
		MessageBox(NULL, output, L"Error", MB_OK);	\
	}	\
}	
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

//Func
char* ConvertWCtoC(wchar_t* _str);
wchar_t* ConvertCtoWC(char* _str);