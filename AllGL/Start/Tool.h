#pragma once
#include <iostream>
#include <string>
class Tool
{
private:
	Tool();
public:
	static std::wstring __C2W(std::string s){
		return __C2W((char*)s.c_str());
	}
	static std::wstring __C2W(char* s);
	static std::string __W2C(std::string s){
		return __W2C((wchar_t*)s.c_str());
	}
	static std::string __W2C(wchar_t* s);
};
#define TC2W(v) Tool::__C2W(v)
#define TW2C(v) Tool::__W2C(v)
#if _UNICODE
#define printf(str,...) wprintf(TC2W(str).c_str(),__VA_ARGS__)
#endif