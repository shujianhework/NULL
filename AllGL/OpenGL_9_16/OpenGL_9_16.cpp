// OpenGL_9_16.cpp : �������̨Ӧ�ó������ڵ㡣
//

// Start.cpp : �������̨Ӧ�ó������ڵ㡣
/*
//���������κ�һ��ֱ����Բ���˶�
//��������׶
//����ͼƬ��ͼ
*/
#include "stdafx.h"
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLAux.h>
#include <Windows.h>
#include "../Start/Tool.h"
#include "AppFrame.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow);
void OnDraw();
int main(int argc, WCHAR* argv[]){
	int ret = wWinMain(GetModuleHandle(NULL), NULL, TEXT(""), TRUE);
	exit(ret);
}
#if _UNICODE
#define printf(str,...) wprintf(L#str,__VA_ARGS__)
#endif
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	AppFrame *app = AppFrame::getInstance();
	app->setNotify([&](AppFrameBackData data){
		switch (data.Type)
		{
		case AppFrameBackData::APBDType::ErrorAssert:
			break;
		case AppFrameBackData::APBDType::ErrorLog:
			printf("ErrorLog:%s", data.Data.c);
			break;
		case AppFrameBackData::APBDType::GLInit:
			break;
		case AppFrameBackData::APBDType::GLKeyBoard:
			break;
		case AppFrameBackData::APBDType::GLDraw:
			OnDraw();
			break;
		case AppFrameBackData::APBDType::MessageNotify:
			return MessageBox(NULL, data.Data.c, TEXT("Notify"), MB_OKCANCEL) == MB_OK;
		}
		return true;
	});
	app->Start();
}
void OnDraw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����Ļ����Ȼ���
	glLoadIdentity();//���õ�ǰģ�͹۲����
	glTranslatef(-1.5f, 0.0f, -6.0f);
	static double RotateAng = 0.0f;
	glRotated(RotateAng, 1, 1, 0);
	RotateAng += 0.25;
	glBegin(GL_TRIANGLES);
	glVertex3f(2.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 2.0f, 0.5f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glColor3f(1, 0, 0);
	//���
	glVertex3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 2.0f, 0.5f);
	glVertex3f(1.0f, 0.5f, 0.0f);
	glColor3f(1, 0, 1);
	//�Ҳ�
	glVertex3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.0f, 2.0f, 0.5f);
	glVertex3f(2.0f, 1.0f, 1.0f);
	glColor3f(1, 1, 0);
	//�ײ�
	glVertex3f(1.0f, 0.5f, 0.0f);
	glVertex3f(2.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glColor3f(1, 1, 1);
	glEnd();
}