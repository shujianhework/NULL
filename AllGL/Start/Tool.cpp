#include "stdafx.h"
#include "Tool.h"
#include <Windows.h>
Tool::Tool()
{
}
std::string Tool::__W2C(wchar_t* s){
	if (s == NULL)
		return "";
	int len = WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
	char *p = new char[len + 1];
	memset(p, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, s, -1, p, len, NULL, NULL);
	std::string cs = p;
	delete[] p;
	return cs;
}
std::wstring Tool::__C2W(char* s){
	if (s == NULL)
		return L"";

	int len = MultiByteToWideChar(CP_ACP, 0, s, -1, NULL, 0);
	wchar_t *p = new wchar_t[len];
	memset(p, 0, len);
	MultiByteToWideChar(CP_ACP, 0, s, -1, p, len);
	std::wstring cs = p;
	delete[] p;
	return cs;
}