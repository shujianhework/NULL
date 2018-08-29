#pragma once
#include <iostream>
#include <string>
class Tool
{
private:
	Tool();
public:
	static std::wstring __C2W(char* s);
	static std::string __W2C(wchar_t* s);
};
#define TC2W(v) Tool::__C2W(v)
#define TW2C(v) Tool::__W2C(v)

