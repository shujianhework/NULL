#include "stdafx.h"
#include "AppFrame.h"
#include <Windows.h>
AppFrame *AppFrame::instan = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
AppFrame::AppFrame() :Notify(nullptr), fullscreen(false), szWindowClass(TEXT("zuokanyoukan")), szTitle(TEXT("���Դ���"))
{
	HDC dispalyhdc = CreateDC(_T("display"), NULL, NULL, NULL);
	int nBitsPerPixel = GetDeviceCaps(dispalyhdc, BITSPIXEL);
	DeleteDC(dispalyhdc);
	dispalyhdc = NULL;
	hInst = GetModuleHandle(NULL);
}


AppFrame::~AppFrame()
{
	KillGLWindow();
}
bool AppFrame::FullDynamicParamSendData(){
	DynamicParamData.Type = AppFrameBackData::GLDraw;
	DynamicParamData.Data.Null = 0;
	return this->Notify(DynamicParamData);
}
bool AppFrame::FullDynamicParamSendData(AppFrameBackData::APBDType type, WCHAR *p){
	DynamicParamData.Type = type;
	DynamicParamData.Data.c = p;
	return this->Notify(DynamicParamData);
}
bool AppFrame::FullDynamicParamSendData(int data){
	/*DynamicParamData.Type = AppFrameBackData::GLInit;
	DynamicParamData.Data.Null = 0;
	return this->Notify(DynamicParamData);*/
	return false;
}
bool AppFrame::FullDynamicParamSendData(WCHAR *p){
	DynamicParamData.Type = AppFrameBackData::ErrorLog;
	DynamicParamData.Data.c = p;
	return this->Notify(DynamicParamData);
}
bool AppFrame::FullDynamicParamSendData(unsigned char index, bool down){
	DynamicParamData.Type = AppFrameBackData::GLKeyBoard;
	DynamicParamData.Data.KeyBoard.index = index;
	DynamicParamData.Data.KeyBoard.down = down;
	return this->Notify(DynamicParamData);
}
bool AppFrame::Start(){
	if (Notify == nullptr)
		return false;
	if (MessageBox(NULL, L"������ȫ��ģʽ������ô��", L"����ȫ��ģʽ", MB_YESNO | MB_ICONQUESTION) == IDNO){
		fullscreen = false;
	}
	
	if (!CreateGLWindow(640, 480, nBitsPerPixel, fullscreen)) {
		return false;
	}
	FullDynamicParamSendData(AppFrameBackData::APBDType::GLInit, TEXT(""));
	ShowWindow(hwnd, true);
	MSG msg;
	BOOL done = false;
	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = true;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			if (active) {
				if (keys[VK_ESCAPE]) {
					done = true;
				}
				else {
					//DrawGLScene();
					FullDynamicParamSendData();
					SwapBuffers(hdc);
				}
			}
			if (keys[VK_F1]) {
				keys[VK_F1] = false;
				KillGLWindow();
				fullscreen = !fullscreen;
				if (!CreateGLWindow(640, 480, 16, fullscreen)) {
					return 0;
				}
			}
		}
	}
	KillGLWindow();
	return true;
}
BOOL AppFrame::CreateGLWindow(int width, int height, int bits, bool fullscreenflag) {
	GLuint PixelFormat = 0;//����ƥ����ҽ��
	int ErrorCode = 0;
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;
	if (!RegisterClassEx(&wcex)) {
		FullDynamicParamSendData(AppFrameBackData::APBDType::ErrorAssert,TEXT("ע�ᴰ��ʧ��"));
		return false;
	}
	ErrorCode = GetLastError();
	RECT WindowRect;//ȡ�þ������Ͻ� ���½� ����ֵ
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.bottom = (long)height;
	WindowRect.top = (long)0;
	fullscreen = fullscreenflag;
	ErrorCode = GetLastError();
	if (fullscreen) {
		DEVMODE dmScreenSettings = { 0 };//�豸ģʽ
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);//DEvmode�ṹ��С
		dmScreenSettings.dmPelsWidth = width;//��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight = height;//��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel = bits;//ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ErrorCode = GetLastError();

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if (FullDynamicParamSendData(AppFrameBackData::APBDType::MessageNotify, TEXT("ȫ��ģʽ�ڵ�ǰ�Կ�������ʧ��!\nʹ�ô���ģʽ?"))) {
				fullscreen = false;
			}
			else {
				return false;
			}
		}
	}
	DWORD dwExStyle;
	DWORD dwStyle;
	if (fullscreen) {
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);//�������ڴﵽ����Ҫ���С
	// 
	//WS_CHILDWINDOW
	//
	hwnd = CreateWindowEx(dwExStyle, szWindowClass, szTitle, dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInst, NULL);
	if (!hwnd){
		ErrorCode = GetLastError();
		KillGLWindow();
		FullDynamicParamSendData(TEXT("���ܴ���һ������"));
		return false;
	}
	if (fullscreen)
		ShowCursor(FALSE);

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),//��С
		1,//�汾��
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,//��ʽ֧�� ���� opengl ˫����
		PFD_TYPE_RGBA,//����rgba ��ʽ
		bits,//ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,//����ɫ��λ
		0,//alpha
		0,//����shift bit
		0,//���ۼӻ���
		0, 0, 0, 0,//���Ծۼ�λ
		16,//16λ z ���� ��Ȼ���
		0,//���ɰ滺��
		0,//�޸�������
		PFD_MAIN_PLANE,//����ͼ��
		0,//Reserved
		0, 0, 0//���Բ�����
	};
	if (!(hdc = GetDC(hwnd))) {
		KillGLWindow();
		FullDynamicParamSendData(TEXT("���ܴ���һ����ƥ������ظ�ʽ"));
		return false;
	}
	//windows����Ӧ�����ظ�ʽ
	if (!(PixelFormat = ChoosePixelFormat(hdc, &pfd))) {
		KillGLWindow();
		FullDynamicParamSendData(TEXT("�����ҵ����ظ�ʽ"));
		return FALSE;
	}
	if (!SetPixelFormat(hdc, PixelFormat, &pfd)) {//�������ظ�ʽ
		KillGLWindow();
		FullDynamicParamSendData(TEXT("�����������ظ�ʽ"));
		return false;
	}


	if (!(hrc = wglCreateContext(hdc))) {
		ErrorCode = GetLastError();
		KillGLWindow();
		FullDynamicParamSendData(TEXT("���ܴ���OpenGL��Ⱦ������"));
		return false;
	}
	if (!wglMakeCurrent(hdc, hrc)) {
		KillGLWindow();
		FullDynamicParamSendData(TEXT("���ܼ���OpenGL��Ⱦ������"));
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ReSizeGLScene(width, height);
	if (!InitGL()) {
		ErrorCode = GetLastError();
		KillGLWindow();
		FullDynamicParamSendData(TEXT("���ܴ���һ�������豸������"));
		return false;
	}
	return true;
}
GLvoid AppFrame::KillGLWindow(GLvoid) {
	if (fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}
	if (hrc) {
		if (!wglMakeCurrent(NULL, NULL)) {
			FullDynamicParamSendData(TEXT("�ͷ�DC��RCʧ��"));
		}
		if (!wglDeleteContext(hrc)) {
			FullDynamicParamSendData(TEXT("�ͷ�Rcʧ��"));
		}
		hrc = NULL;
	}
	if (hdc && !ReleaseDC(hwnd, hdc)) {
		FullDynamicParamSendData(TEXT("�ͷ�hdcʧ��"));
		hdc = NULL;
	}
	if (hwnd && !DestroyWindow(hwnd)) {
		FullDynamicParamSendData(TEXT("�ͷŴ��ھ��ʧ��"));
		hwnd = NULL;
	}
	if (!UnregisterClass(szWindowClass, hInst)) {
		FullDynamicParamSendData(TEXT("����ע��������"));
		hInst = NULL;
	}
}
GLvoid AppFrame::ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0) {
		height = 1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);//ͶӰ����
	glLoadIdentity();//����ͶӰ����
	//�����ӿڴ�С
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);//ѡ��ģ�͹۲����
	glLoadIdentity();//����ģ�͹۲����
}
int AppFrame::InitGL(GLvoid) {
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);//������Ӱƽ��
	glClearColor(0.0, 0.0, 0.0, 0.0);//���ñ���ɫ
	glClearDepth(1.0f);//������Ȼ���
	glEnable(GL_DEPTH_TEST);//������Ȳ���
	glDepthFunc(GL_LEQUAL);//��Ȳ�������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//��͸�ӽ�������
	return TRUE;
}
LRESULT AppFrame::LoopWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message)
	{
	case WM_ACTIVATE://���Ӵ��ڼ���
	{
		if (!HIWORD(wParam)) {//�����С��״̬
			active = true;//����
		}
		else {
			active = false;//������
		}
		return 0;
	}
	case WM_SYSCOMMAND: {
		switch (wParam)
		{
		case SC_SCREENSAVE://����Ҫ����
		case SC_MONITORPOWER://��ʾ��Ҫ����ڵ�ģʽ
			return 0;//��ֹ����
		}
		break;
	}
	case WM_CLOSE: {
		PostQuitMessage(0);
		return 0;
	}
	case WM_DESTROY:
		keys[VK_ESCAPE] = true;
		break;
	case WM_KEYDOWN: {
		if(true == this->FullDynamicParamSendData((unsigned char)wParam, true))
			keys[wParam] = true;
		return 0;
	}
	case WM_KEYUP: {
		if(this->FullDynamicParamSendData((unsigned char)wParam, false))
			keys[wParam] = false;
		return 0;
	}
	case WM_SIZE:
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	return AppFrame::getInstance()->LoopWndProc(hWnd, message, wParam, lParam);
}