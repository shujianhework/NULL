// OpenGL_9_16.cpp : 定义控制台应用程序的入口点。
//

// Start.cpp : 定义控制台应用程序的入口点。
/*
//绘制三角形和一条直线做圆周运动
//绘制三棱锥
//加载图片贴图
*/
#include "stdafx.h"
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLAux.h>
#include <Windows.h>
#include "../Start/Tool.h"
#include "AppFrame.h"
#include "Textures.h"
#include "LearningUnit.h"
#include "SystemFile.h"
unsigned int iaTextrus[100] = { 0 };
int main(int argc, WCHAR* argv[]);
int flg = (int)setlocale(LC_ALL, "");
#ifdef _UNICODE
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
#else
#define AL 1
#define FL HI_##AL
struct HI_1{
	int a;
};
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
#endif
	_In_ int       nCmdShow)
{

	AppFrame *app = AppFrame::getInstance();
	LearningUnit *logic = GetLearningUnit();
	app->setNotify([&](AppFrameBackData data){
		switch (data.Type)
		{
		case AppFrameBackData::APBDType::ErrorAssert:
			break;
		case AppFrameBackData::APBDType::ErrorLog:
			printf("ErrorLog:%s", data.Data.c);
			break;
		case AppFrameBackData::APBDType::GLInit:
			logic->OnInit();
			break;
		case AppFrameBackData::APBDType::GLKeyBoard:
			logic->OnKeyBoard(data.Data.KeyBoard.index, data.Data.KeyBoard.down);
			break;
		case AppFrameBackData::APBDType::GLDraw:
			logic->OnDraw();
			break;
		case AppFrameBackData::APBDType::MessageNotify:
			return MessageBox(NULL, data.Data.c, TEXT("Notify"), MB_OKCANCEL) == MB_OK;
		case AppFrameBackData::APBDType::Idle:
			logic->OnIdle();
			break;
		}
		return true;
	});
	app->Start();
	FreeLearningUnit();
	return true;
}
int main(int argc, WCHAR* argv[]){
#ifdef _UNICODE
	int ret = wWinMain(GetModuleHandle(NULL), NULL, TEXT(""), TRUE);
#else
	int ret = WinMain(GetModuleHandle(NULL), NULL, TEXT(""), TRUE);
#endif
	exit(ret);
}
