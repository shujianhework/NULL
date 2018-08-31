// LightKeyBoardBlend.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLAux.h>
#include <Windows.h>
#include "../Start/Tool.h"
void DefinesPublis(){
#define KeyIFOperIndex(Condition,Index,NewAddValue) \
if(Condition){\
	GLfloat value = 0;\
	Quete(value,Index,false);\
	value+=NewAddValue;\
	Quete(value,Index,true);\
}
}
void DefinesInLight(){//�����������õ�
	if (true)
		return;
#define MAX_LOADSTRING 100
	//float double
#define XROT 0
#define YROT 1
#define XSPEED 2
#define YSPEED 3
#define Z 4
#define LightAmbient 5
#define LightDiffuse 9
#define LightPosition 13
	//int
#define Filter 0
#define Textures 1
#define LP 4
#define FP 5
#define LIGHT 6
}
void DefineInBlend(){
	if (true)
		return;
#define BLEND	7
#define BP		8
}
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
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
int InitGL(GLvoid);
int DrawGLScene(GLvoid);
void DrawLight();
bool Quete(GLfloat& value,GLuint index,bool Dir);
bool Quete(GLfloat value,GLuint index);

bool QueteUINT(GLuint& value, GLuint index, bool Dir);
bool QueteUINT(GLuint value, GLuint index);

void DrawInitLithtValues();
GLvoid KillGLWindow(GLvoid);
BOOL CreateGLWindow(int width, int height, int bits, bool fullscreenflag);
AUX_RGBImageRec* LoadBMP(WCHAR* Filename);
int LoadGLTextures(int index, int &texture);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow);

int main(int argc, WCHAR* argv[]){
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
	GLuint tempvalue = 0;
	GLfloat fvaluez = 0;
	GLfloat fvaluex = 0;
	GLfloat fvaluey = 0;
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
					QueteUINT(tempvalue, LP,false);
					if (keys['L'] && !tempvalue){
						tempvalue = 1;
						QueteUINT(tempvalue, LP, true);
						QueteUINT(tempvalue, LIGHT, false);
						if (!tempvalue){
							glDisable(GL_LIGHTING);
						}
						else
							glEnable(GL_LIGHTING);
					}
					if (!keys['L']){
						tempvalue = 0;
						QueteUINT(tempvalue, LP, true);
					}
					QueteUINT(tempvalue, FP, false);
					if (keys['F'] && !tempvalue){
						tempvalue = 1;
						QueteUINT(tempvalue, FP, true);
						QueteUINT(tempvalue, Filter, false);
						tempvalue += 1;
						if (tempvalue > 2)
							tempvalue = 1;
						QueteUINT(tempvalue, Filter, true);
					}
					if (!keys['F']){
						tempvalue = 0;
						QueteUINT(tempvalue, FP, true);
					}
					QueteUINT(tempvalue, BP, false);
					if (keys['B'] && !tempvalue){
						tempvalue = 1;
						QueteUINT(tempvalue, BP, true);
						QueteUINT(tempvalue, BLEND, false);
						tempvalue = !tempvalue;
						if (tempvalue){
							glEnable(GL_BLEND);
							glDisable(GL_DEPTH_TEST);
						}
						else{
							glDisable(GL_BLEND);
							glEnable(GL_DEPTH_TEST);
						}
						QueteUINT(tempvalue, BLEND, true);
					}
					if (!keys['B']){
						tempvalue = 0;
						QueteUINT(tempvalue, BP, true);
					}
					KeyIFOperIndex(keys[VK_PRIOR], Z, -0.02);
					KeyIFOperIndex(keys[VK_NEXT], Z, 0.02);
					KeyIFOperIndex(keys[VK_UP], XSPEED, -0.01);
					KeyIFOperIndex(keys[VK_DOWN], XSPEED, 0.01);
					KeyIFOperIndex(keys[VK_RIGHT], YSPEED, 0.01);
					KeyIFOperIndex(keys[VK_LEFT], YSPEED, -0.01);
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
int InitGLInLight(){
	DrawInitLithtValues();
	GLfloat arrs[4] = { 0.0f };
	for (int i = 0; i < 4; i++)
	{
		Quete(arrs[i], LightAmbient + i, false);
	}
	glLightfv(GL_LIGHT1, GL_AMBIENT, arrs);

	for (int i = 0; i < 4; i++)
	{
		Quete(arrs[i], LightDiffuse + i, false);
	}
	glLightfv(GL_LIGHT1, GL_DIFFUSE, arrs);

	for (int i = 0; i < 4; i++)
	{
		Quete(arrs[i], LightPosition + i, false);
	}
	glLightfv(GL_LIGHT1, GL_POSITION, arrs);
	glEnable(GL_LIGHT1);
	return true;
}
void InitGLInBlend(){
}
int InitGL(GLvoid) {
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);//������Ӱƽ��
	glClearColor(0.0, 0.0, 0.0, 0.5);//����ɫ
	glClearDepth(1.0f);//������Ȼ���
	glEnable(GL_DEPTH_TEST);//������Ȳ���
	glDepthFunc(GL_LEQUAL);//��Ȳ�������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//��͸�ӽ�������
	InitGLInLight();
	InitGLInBlend();
	glColor4f(0.25f, 1.0f, 0.82f, 0.5);//ȫ�� ��͸��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);//����Դ����alphaͨ���İ�͸����Ϻ���
	return TRUE;
}
int DrawGLScene(GLvoid) {
	DrawLight();
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
bool Quete(GLfloat value, GLuint index){
	return Quete(value, index, true);
}
bool Quete(GLfloat& value, GLuint index, bool Dir){
	static GLfloat GLfloatValues[100] = { 0 };
	if (index >= 100)
		return false;
	if (Dir)
		GLfloatValues[index] = value;
	else
		value = GLfloatValues[index];
	return true;
}
bool QueteUINT(GLuint& value, GLuint index, bool Dir){
	static GLuint GLuintValues[100] = { 0 };
	if (index > 100)
		return false;
	if (Dir)
		GLuintValues[index] = value;
	else
		value = GLuintValues[index];
	return true;
}
bool QueteUINT(GLuint value, GLuint index){
	return QueteUINT(value, index, true);
}
GLuint filter;//�˲�����
#ifdef _UNICODE
static FILE *fopen(wchar_t *filename, wchar_t *mode){
	return ::fopen(TW2C(filename).c_str(), TW2C(mode).c_str());
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
int LoadGLTexture(){
	int State = FALSE;
	AUX_RGBImageRec* TextureImage[1] = { 0 };
	memset(TextureImage, 0, sizeof(void*));
	GLuint texture[3];//3������Ĵ洢�ռ�
	if (TextureImage[0] = LoadBMP(TEXT("Glass.bmp"))){
		State = TRUE;
		glGenTextures(3, &texture[0]);
		//���� Nearest�˲���ͼ
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		//���� ����(��ֵ)�˲���ͼ
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);


		//����MipMapped�˲���ͼ
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		free(TextureImage[0]->data);
		free(TextureImage[0]);
		TextureImage[0] = NULL;

		QueteUINT(texture[0], Textures);
		QueteUINT(texture[1], Textures+1);
		QueteUINT(texture[2], Textures+2);
	}
	return State;
}

void DrawInitLithtValues(){
	Quete(-5.0f, Z);

	Quete(0.5, LightAmbient);
	Quete(0.5, LightAmbient+1);
	Quete(0.5, LightAmbient+2);
	Quete(1, LightAmbient+3);

	Quete(1, LightDiffuse);
	Quete(1, LightDiffuse+1);
	Quete(1, LightDiffuse+2);
	Quete(1, LightDiffuse+3);

	Quete(0, LightPosition);
	Quete(0, LightPosition+1);
	Quete(2, LightPosition + 2);
	Quete(1, LightPosition + 3);

	GLfloat tempvalues = 0.21;
	Quete(tempvalues, XSPEED, true);
	Quete(tempvalues, YSPEED, true);
	QueteUINT(0, Filter);
	LoadGLTexture();
}
void DrawLight(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	GLfloat value = 0,xrot = 0,yrot = 0;
	GLuint uvalue = 0;
	Quete(value, Z, false);
	glTranslatef(0, 0, value);
	Quete(xrot, XROT, false);
	glRotatef(xrot, 1, 0, 0);
	Quete(yrot, YROT, false);
	glRotatef(yrot, 0, 1, 0);
	QueteUINT(uvalue, Filter, false);
	QueteUINT(uvalue, Textures + uvalue, false);
	glBindTexture(GL_TEXTURE_2D, uvalue);
	glBegin(GL_QUADS);

	//ǰ����
	glNormal3f(0, 0, 1);//������۲���
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0);
	glVertex3f(1, -1, 1);
	glTexCoord2f(1, 1);
	glVertex3f(1, 1, 1);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 1, 1);
	
	//�����
	glNormal3f(0, 0, -1);//���߱��۲���
	glTexCoord2f(1, 0);
	glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 1);
	glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 1);
	glVertex3f(1, 1, -1);
	glTexCoord2f(0, 0);
	glVertex3f(1, -1, -1);

	//����
	glNormal3f(0, 1, 0);//��������
	glTexCoord2f(0, 1);
	glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 0);
	glVertex3f(-1, 1, 1);
	glTexCoord2f(1, 0);
	glVertex3f(1, 1, 1);
	glTexCoord2f(1, 1);
	glVertex3f(1, 1, -1);

	//����
	glNormal3f(0, -1, 0);//��������
	glTexCoord2f(1, 1);
	glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 1);
	glVertex3f(1, -1, -1);
	glTexCoord2f(0, 0);
	glVertex3f(1, -1, 1);
	glTexCoord2f(1,0);
	glVertex3f(-1, -1, 1);

	//�Ҳ���
	glNormal3f(1, 0, 0);//��������
	glTexCoord2f(1, 0);
	glVertex3f(1, -1, -1);
	glTexCoord2f(1, 1);
	glVertex3f(1, -1, 1);
	glTexCoord2f(0, 1);
	glVertex3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex3f(1, 1, -1);

	//�����
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, -1);
	glTexCoord2f(1, 0);
	glVertex3f(-1, -1, 1);
	glTexCoord2f(1,1);
	glVertex3f(-1, 1, 1);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 1, -1);
	glEnd();
	Quete(value, XSPEED, false);
	xrot += value;
	Quete(xrot, XROT, true);
	Quete(value, YSPEED, false);
	yrot += value;
	Quete(yrot, YROT, true);
}