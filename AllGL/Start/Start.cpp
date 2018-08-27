// Start.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <Windows.h>
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HGLRC hrc = NULL;//������ɫ���������
HDC hdc = NULL;//OpenGL��Ⱦ���������
HWND hwnd = NULL;
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING] = TEXT("���ҿ�");                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("zuokanyoukan");            // ����������

bool keys[256];//������̰���������
bool active = TRUE;
bool fullscreen = TRUE;

// �˴���ģ���а����ĺ�����ǰ������: 
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
int InitGL(GLvoid);
int DrawGLScene(GLvoid);
GLvoid KillGLWindow(GLvoid);
BOOL CreateGLWindow(int width, int height, int bits, bool fullscreenflag);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow);
int main(int argc,WCHAR* argv[]){
	return wWinMain(GetModuleHandle(NULL), NULL, TEXT(""), TRUE);
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: �ڴ˷��ô��롣
	hInst = hInstance;
	// ��ʼ��ȫ���ַ���
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadStringW(hInstance, IDC_GLFRAME, szWindowClass, MAX_LOADSTRING);
	if (MessageBox(NULL, L"������ȫ��ģʽ������ô��", L"����ȫ��ģʽ", MB_YESNO | MB_ICONQUESTION) == IDNO){
		fullscreen = false;
	}
	if (!CreateGLWindow(640, 480, 16, fullscreen)) {
		return 0;
	}
	// ִ��Ӧ�ó����ʼ��: 
	/*if (!InitInstance (hInstance, nCmdShow))
	{
	return FALSE;
	}
	*/
	ShowWindow(hwnd, true);
	//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GLFRAME));

	MSG msg;
	BOOL done = false;
	// ����Ϣѭ��: 
	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = true;
			}
			else {
				//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				//{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				//}
			}
		}
		else {
			if (active) {
				if (keys[VK_ESCAPE]) {
					done = true;
				}
				else {
					DrawGLScene();
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
	return (int)msg.wParam;
}



//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
		keys[wParam] = true;
		return 0;
	}
	case WM_KEYUP: {
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

// �����ڡ������Ϣ��������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {
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
int InitGL(GLvoid) {
	glShadeModel(GL_SMOOTH);//������Ӱƽ��
	glClearColor(0.0, 0.0, 0.0, 0.0);//���ñ���ɫ
	glClearDepth(1.0f);//������Ȼ���
	glEnable(GL_DEPTH_TEST);//������Ȳ���
	glDepthFunc(GL_LEQUAL);//��Ȳ�������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//��͸�ӽ�������
	return TRUE;
}
int DrawGLScene(GLvoid) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����Ļ����Ȼ���
	glLoadIdentity();
	return TRUE;
}
GLvoid KillGLWindow(GLvoid) {
	if (fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}
	if (hrc) {
		if (!wglMakeCurrent(NULL, NULL)) {
			MessageBox(NULL, L"�ͷ�DC��RCʧ��", L"�رմ���", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hrc)) {
			MessageBox(NULL, L"�ͷ�Rcʧ��", L"�رմ���", MB_OK | MB_ICONINFORMATION);
		}
		hrc = NULL;
	}
	if (hdc && !ReleaseDC(hwnd, hdc)) {
		MessageBox(NULL, L"�ͷ�hdcʧ��", L"�رմ���", MB_OK | MB_ICONINFORMATION);
		hdc = NULL;
	}
	if (hwnd && !DestroyWindow(hwnd)) {
		MessageBox(NULL, L"�ͷŴ��ھ��ʧ��", L"�رմ���", MB_OK | MB_ICONINFORMATION);
		hwnd = NULL;
	}
	if (!UnregisterClass(szWindowClass, hInst)) {
		MessageBox(NULL, L"����ע��������", L"�رմ���", MB_OK | MB_ICONINFORMATION);
		hInst = NULL;
	}
}
BOOL CreateGLWindow(int width, int height, int bits, bool fullscreenflag) {
	GLuint PixelFormat = 0;//����ƥ����ҽ��
	int ErrorCode = 0;
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;//DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;// LoadIcon(hInst, MAKEINTRESOURCE(IDI_GLFRAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_GLFRAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, L"ע�ᴰ��ʧ��", L"����", MB_OK | MB_ICONEXCLAMATION);
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
		dmScreenSettings.dmPelsWidth = width;//��ѡ��Ļ����
		dmScreenSettings.dmPelsHeight = height;//��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel = bits;//ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ErrorCode = GetLastError();
		//����������ʾģʽ�����ؽ�� CDS_FULLSCREEN ��ȥ״̬��
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if (MessageBox(NULL, L"ȫ��ģʽ�ڵ�ǰ�Կ�������ʧ��!\nʹ�ô���ģʽ?", L"NeHe GL", MB_YESNO | MB_ICONINFORMATION)) {
				fullscreen = false;
			}
			else {
				MessageBox(NULL, L"���򽫱��ر�", L"����", MB_OK | MB_ICONINFORMATION);
				return false;
			}
		}
	}
	ErrorCode = GetLastError();
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
	ErrorCode = GetLastError();
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);//�������ڴﵽ����Ҫ���С
	ErrorCode = GetLastError();
	// 
	//WS_CHILDWINDOW
	//
	hwnd = CreateWindowEx(dwExStyle, szWindowClass, szTitle, dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInst, NULL);
	if (!hwnd){
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"���ܴ���һ������", L"����", MB_OK | MB_ICONINFORMATION);
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
		MessageBox(NULL, L"���ܴ���һ����ƥ������ظ�ʽ", L"����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	//windows����Ӧ�����ظ�ʽ
	if (!(PixelFormat = ChoosePixelFormat(hdc, &pfd))) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"�����ҵ����ظ�ʽ", L"����", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	if (!SetPixelFormat(hdc, PixelFormat, &pfd)) {//�������ظ�ʽ
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"�����������ظ�ʽ", L"����", MB_OK | MB_ICONINFORMATION);
		return false;
	}


	if (!(hrc = wglCreateContext(hdc))) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"���ܴ���OpenGL��Ⱦ������", L"����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	if (!wglMakeCurrent(hdc, hrc)) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"���ܼ���OpenGL��Ⱦ������", L"����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ReSizeGLScene(width, height);
	if (!InitGL()) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"���ܴ���һ�������豸������", L"����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	return true;
}
