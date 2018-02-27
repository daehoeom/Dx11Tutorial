#include "Define.h"

// wchar_t to char
char* ConvertWCtoC(wchar_t* _str)
{
	char* pStr;

	int bufferSize = WideCharToMultiByte(CP_ACP, 0, _str, -1, NULL, 0, NULL, NULL);

	pStr = new char[bufferSize];

	WideCharToMultiByte(CP_ACP, 0, _str, -1, pStr, bufferSize, 0, 0);

	return pStr;
}

// char to wchar_t
wchar_t* ConvertCtoWC(char* _str)
{
	wchar_t* pStr;

	int bufferSize = MultiByteToWideChar(CP_ACP, 0, _str, -1, NULL, NULL);

	pStr = new WCHAR[bufferSize];

	MultiByteToWideChar(CP_ACP, 0, _str, strlen(_str) + 1, pStr, bufferSize);

	return pStr;
}