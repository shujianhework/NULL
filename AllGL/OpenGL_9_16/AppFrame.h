#pragma once
#include <cmath>
#include <iostream>
#include <string>
#include <functional>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLAux.h>

struct AppFrameBackData
{
	enum APBDType{
		ErrorAssert,
		ErrorLog,
		MessageNotify,
		GLInit,
		GLKeyBoard,
		GLDraw,
		Idle,
	}Type;
	union MyUnion
	{
		TCHAR *c;
		int Null;
		struct 
		{
			unsigned char index;
			bool down;
		}KeyBoard;
	}Data;
};
class AppFrame
{
private:
	std::function<bool(AppFrameBackData)> Notify;
	static AppFrame* instan;
	int nBitsPerPixel;
	bool fullscreen;
	HINSTANCE hInst;
	HGLRC hrc;// = NULL;//������ɫ��������
	HDC hdc;// = NULL;//OpenGL��Ⱦ��������
	HWND hwnd;// = NULL;                          // ��ǰʵ��
	TCHAR *szTitle;// = TEXT("���ҿ�");                  // �������ı�
	TCHAR *szWindowClass;// = TEXT("zuokanyoukan");            // ����������
	bool keys[256];//������̰���������
	bool active;
	AppFrameBackData DynamicParamData;
	bool StopFlg;
private:
	AppFrame();
	~AppFrame();
	bool FullDynamicParamSendData(AppFrameBackData::APBDType type, TCHAR *p);
	bool FullDynamicParamSendData();
	bool FullDynamicParamSendData(int data);
	bool FullDynamicParamSendData(TCHAR *p);
	bool FullDynamicParamSendData(unsigned char index, bool down);
	BOOL CreateGLWindow(int width, int height, int bits, bool fullscreenflag);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	void GLInit();
	GLvoid KillGLWindow();
	int InitGL(GLvoid);
public:
	static AppFrame*getInstance(){
		instan = instan ? instan : new AppFrame();
		return instan;
	}
	inline void setNotify(std::function<bool(AppFrameBackData)> Back){
		Notify = Back;
	}
	bool Start();
	LRESULT LoopWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	inline void Stop(){
		StopFlg = true;
	}
	inline bool FindKey(unsigned char index){
		return keys[index];
	}
	inline HDC getDC(){
		return this->hdc;
	}
};