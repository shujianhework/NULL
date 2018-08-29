// OtherTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void Defines(){
#define Define1 0
#define Define2 1
#define Define3 2
#define Define4 3
#define ShowDefine(x) printf("arr[%d] = %d\n",(x),arr[(x)])
}
int _tmain(int argc, _TCHAR* argv[])
{
	int arr[4] = { 23, 231, 2435, 532 };
	ShowDefine(Define1);
	ShowDefine(Define2);
	ShowDefine(Define3);
	ShowDefine(Define4);
	return 0;
}

