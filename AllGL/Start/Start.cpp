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
#include "Tool.h"
//#pragma comment(lib,"legacy_stdio_definitions_lib")
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HGLRC hrc = NULL;//������ɫ��������
HDC hdc = NULL;//OpenGL��Ⱦ��������
HWND hwnd = NULL;
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING] = TEXT("���ҿ�");                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("zuokanyoukan");            // ����������

bool keys[256];//������̰���������
bool active = TRUE;
bool fullscreen = false;

// �˴���ģ���а����ĺ�����ǰ������: 
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
int InitGL(GLvoid);
int DrawGLScene(GLvoid);
GLvoid KillGLWindow(GLvoid);
BOOL CreateGLWindow(int width, int height, int bits, bool fullscreenflag);
void DrawTexture();
void DrawMitsubishiCone();
AUX_RGBImageRec* LoadBMP(WCHAR* Filename);
int LoadGLTextures(int index, int &texture);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow);
int main(int argc,WCHAR* argv[]){
	int ret = wWinMain(GetModuleHandle(NULL), NULL, TEXT(""), TRUE);
	exit(ret);
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
	/*if (MessageBox(NULL, L"������ȫ��ģʽ������ô��", L"����ȫ��ģʽ", MB_YESNO | MB_ICONQUESTION) == IDNO){
		fullscreen = false;
	}*/

	HDC dispalyhdc = CreateDC(_T("display"), NULL, NULL, NULL);//dispalyhdc
	int nBitsPerPixel = GetDeviceCaps(dispalyhdc, BITSPIXEL);
	DeleteDC(dispalyhdc);
	dispalyhdc = NULL;
	if (!CreateGLWindow(640, 480, nBitsPerPixel, fullscreen)) {
		return 0;
	}
	// ִ��Ӧ�ó����ʼ��: 
	ShowWindow(hwnd, true);

	MSG msg;
	BOOL done = false;
	// ����Ϣѭ��: 
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
					DrawGLScene();
					SwapBuffers(::hdc);
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

// �����ڡ������Ϣ�������
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
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);//������Ӱƽ��
	glClearColor(1.0, 1.0, 1.0, 0.0);//���ñ���ɫ
	glClearDepth(1.0f);//������Ȼ���
	glEnable(GL_DEPTH_TEST);//������Ȳ���
	glDepthFunc(GL_LEQUAL);//��Ȳ�������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//��͸�ӽ�������
	return TRUE;
}
//���������κ�һ��ֱ����Բ���˶�
void DrawTRIANGLESAndLine()
{
#define UnitAngle (3.141592627/180)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����Ļ����Ȼ���
	glLoadIdentity();//���õ�ǰģ�͹۲����
	glTranslatef(-1.5f, 0.0f, -6.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 1, 0);
	glColor3f(1, 0, 0);
	glVertex3f(-1, -1, 0);
	glColor3f(0, 0, 1);
	glVertex3f(1, -1, 0);
	glColor3f(0, 0.85, 0);
	glEnd();
	static double i = 0;
	glBegin(GL_LINES);
	double x = 0.0;
	double y = 0.0;
	glVertex3f(2, 0, 0);
	x = sin(i*UnitAngle);
	y = cos(i*UnitAngle);
	glVertex3f(x + 2, y, 0);
	glColor3f(x, y, 1);
	glEnd();
	i += 0.15;
	if (i > 360)
		i = 0.0f;
#undef UnitAngle
}
//��������׶
void DrawMitsubishiCone()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����Ļ����Ȼ���
	glLoadIdentity();//���õ�ǰģ�͹۲����
	glTranslatef(-1.5f, 0.0f, -6.0f);
	static double RotateAng = 0.0f;
	glRotated(RotateAng, 1, 1, 0);
	RotateAng += 0.15;
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
#ifdef _UNICODE
static FILE *fopen(wchar_t *filename, wchar_t *mode){
	return ::fopen(TW2C(filename).c_str(),TW2C(mode).c_str());
}
#endif
AUX_RGBImageRec* LoadBMP(WCHAR* Filename){
	FILE *fp = fopen(Filename, TEXT("r"));
	if (fp == NULL){
		printf("�ļ���ʧ��");
		return NULL;
	}
	fclose(fp);
	return auxDIBImageLoad(Filename);
}
int LoadGLTextures(int index,int &texture){
	static const WCHAR* filenames[] = { TEXT("test2.bmp"), TEXT("test1.bmp"),
										TEXT("test.bmp"), TEXT("test3.bmp"),
										TEXT("test3.bmp"), TEXT("test4.bmp"), TEXT("test5.bmp") };
	if (index > 5 && index < 0)
		return FALSE;
	int Status = FALSE;
	static GLuint textures[6] = {0};
	if (textures[index] > 0){
		texture = textures[index];
		return TRUE;
	}
	AUX_RGBImageRec* TextureImages[1] = {NULL};
	
	TextureImages[0] = LoadBMP((WCHAR*)filenames[index]);
	if (TextureImages[0]){
		Status = TRUE;
		GLuint arrtextres[] = { 0 };
		glGenTextures(1, &arrtextres[0]);
		glBindTexture(GL_TEXTURE_2D, arrtextres[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImages[0]->sizeX, TextureImages[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImages[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		textures[index] = arrtextres[0];
		texture = textures[index];
	}
	if (TextureImages[0]){
		if (TextureImages[0]->data){
			free(TextureImages[0]->data);
		}
		free(TextureImages[0]);
		TextureImages[0] = NULL;
	}
	return Status;
}
void DrawTexture(){
	static GLfloat xrot = 0;
	static GLfloat yrot = 0;
	static GLfloat zrot = 0;
	static int texture = 0;
	if (texture == 0){
		if (LoadGLTextures(1, texture) == FALSE){
			texture = -1;
			return;
		}
	}
	else if (texture == -1)
		return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -5);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);
	glRotatef(zrot, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	LoadGLTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 0.0f, 0.0f);						// Red
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
	glColor3f(0.0f, 1.0f, 0.0f);						// Green
	glTexCoord2f(1, 1);
	glVertex3f(-1.0f, -1.0f, 1.0f);					// Left Of Triangle (Front)

	LoadGLTextures(2, texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glColor3f(0.0f, 0.0f, 1.0f);						// Blue
	glTexCoord2f(0.23, 0.75);
	glVertex3f(1.0f, -1.0f, 1.0f);					// Right Of Triangle (Front)
	glColor3f(1.0f, 0.0f, 0.0f);						// Red
	glTexCoord2f(0.1, 1);
	glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
	glColor3f(0.0f, 0.0f, 1.0f);						// Blue

	LoadGLTextures(3, texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexCoord2f(1, 0);
	glVertex3f(1.0f, -1.0f, 1.0f);					// Left Of Triangle (Right)
	glColor3f(0.0f, 1.0f, 0.0f);						// Green
	glTexCoord2f(0.33, 1);
	glVertex3f(1.0f, -1.0f, -1.0f);					// Right Of Triangle (Right)
	glColor3f(1.0f, 0.0f, 0.0f);						// Red
	glTexCoord2f(0, 0.35);
	glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
	glColor3f(0.0f, 1.0f, 0.0f);						// Green
	glTexCoord2f(0, 0.42);
	glVertex3f(1.0f, -1.0f, -1.0f);					// Left Of Triangle (Back)
	glColor3f(0.0f, 0.0f, 1.0f);						// Blue
	glTexCoord2f(0, 0.88);
	glVertex3f(-1.0f, -1.0f, -1.0f);					// Right Of Triangle (Back)
	glColor3f(1.0f, 0.0f, 0.0f);						// Red
	glTexCoord2f(0, 0.67);
	glVertex3f(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
	glColor3f(0.0f, 0.0f, 1.0f);						// Blue
	glTexCoord2f(0.66, 1);
	glVertex3f(-1.0f, -1.0f, -1.0f);					// Left Of Triangle (Left)
	glColor3f(0.0f, 1.0f, 0.0f);						// Green
	glTexCoord2f(0.21, 0.54);
	glVertex3f(-1.0f, -1.0f, 1.0f);					// Right Of Triangle (Left)
	glEnd();											// Done Drawing The Quad
	xrot += 0.2f;
	yrot += 0.3f;
	zrot += 0.1f;
}
int DrawGLScene(GLvoid) {
	//DrawMitsubishiCone();
	DrawTexture();
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
		dmScreenSettings.dmPelsWidth = width;//��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight = height;//��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel = bits;//ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ErrorCode = GetLastError();
		
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
	if (!(::hdc = GetDC(hwnd))) {
		KillGLWindow();
		MessageBox(NULL, L"���ܴ���һ����ƥ������ظ�ʽ", L"����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	//windows����Ӧ�����ظ�ʽ
	if (!(PixelFormat = ChoosePixelFormat(::hdc, &pfd))) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"�����ҵ����ظ�ʽ", L"����", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	if (!SetPixelFormat(::hdc, PixelFormat, &pfd)) {//�������ظ�ʽ
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"�����������ظ�ʽ", L"����", MB_OK | MB_ICONINFORMATION);
		return false;
	}


	if (!(hrc = wglCreateContext(::hdc))) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"���ܴ���OpenGL��Ⱦ������", L"����", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	if (!wglMakeCurrent(::hdc, hrc)) {
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

