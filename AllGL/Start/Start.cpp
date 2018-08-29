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
#include "Tool.h"
//#pragma comment(lib,"legacy_stdio_definitions_lib")
#define MAX_LOADSTRING 100

// 全局变量: 
HGLRC hrc = NULL;//窗口着色描述表句柄
HDC hdc = NULL;//OpenGL渲染描述表句柄
HWND hwnd = NULL;
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING] = TEXT("左看右看");                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("zuokanyoukan");            // 主窗口类名

bool keys[256];//保存键盘按键的数组
bool active = TRUE;
bool fullscreen = false;

// 此代码模块中包含的函数的前向声明: 
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

	// TODO: 在此放置代码。
	hInst = hInstance;
	// 初始化全局字符串
	/*if (MessageBox(NULL, L"你想在全屏模式下运行么？", L"设置全屏模式", MB_YESNO | MB_ICONQUESTION) == IDNO){
		fullscreen = false;
	}*/

	HDC dispalyhdc = CreateDC(_T("display"), NULL, NULL, NULL);//dispalyhdc
	int nBitsPerPixel = GetDeviceCaps(dispalyhdc, BITSPIXEL);
	DeleteDC(dispalyhdc);
	dispalyhdc = NULL;
	if (!CreateGLWindow(640, 480, nBitsPerPixel, fullscreen)) {
		return 0;
	}
	// 执行应用程序初始化: 
	ShowWindow(hwnd, true);

	MSG msg;
	BOOL done = false;
	// 主消息循环: 
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

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
	case WM_ACTIVATE://监视窗口激活
	{
		if (!HIWORD(wParam)) {//检查最小化状态
			active = true;//激活
		}
		else {
			active = false;//不激活
		}
		return 0;
	}
	case WM_SYSCOMMAND: {
		switch (wParam)
		{
		case SC_SCREENSAVE://屏保要运行
		case SC_MONITORPOWER://显示器要进入节点模式
			return 0;//阻止发生
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

// “关于”框的消息处理程序。
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
	glMatrixMode(GL_PROJECTION);//投影矩阵
	glLoadIdentity();//重置投影矩阵
	//设置视口大小
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);//选择模型观察矩阵
	glLoadIdentity();//重置模型观察矩阵
}
int InitGL(GLvoid) {
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);//启用阴影平滑
	glClearColor(1.0, 1.0, 1.0, 0.0);//设置背景色
	glClearDepth(1.0f);//设置深度缓存
	glEnable(GL_DEPTH_TEST);//启用深度测试
	glDepthFunc(GL_LEQUAL);//深度测试类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//对透视进行修正
	return TRUE;
}
//绘制三角形和一条直线做圆周运动
void DrawTRIANGLESAndLine()
{
#define UnitAngle (3.141592627/180)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕和深度缓存
	glLoadIdentity();//重置当前模型观察矩阵
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
//绘制三棱锥
void DrawMitsubishiCone()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕和深度缓存
	glLoadIdentity();//重置当前模型观察矩阵
	glTranslatef(-1.5f, 0.0f, -6.0f);
	static double RotateAng = 0.0f;
	glRotated(RotateAng, 1, 1, 0);
	RotateAng += 0.15;
	glBegin(GL_TRIANGLES);
	glVertex3f(2.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 2.0f, 0.5f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glColor3f(1, 0, 0);
	//左侧
	glVertex3f(0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 2.0f, 0.5f);
	glVertex3f(1.0f, 0.5f, 0.0f);
	glColor3f(1, 0, 1);
	//右侧
	glVertex3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.0f, 2.0f, 0.5f);
	glVertex3f(2.0f, 1.0f, 1.0f);
	glColor3f(1, 1, 0);
	//底部
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
		printf("文件打开失败");
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
			MessageBox(NULL, L"释放DC或RC失败", L"关闭错误", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hrc)) {
			MessageBox(NULL, L"释放Rc失败", L"关闭错误", MB_OK | MB_ICONINFORMATION);
		}
		hrc = NULL;
	}
	if (hdc && !ReleaseDC(hwnd, hdc)) {
		MessageBox(NULL, L"释放hdc失败", L"关闭错误", MB_OK | MB_ICONINFORMATION);
		hdc = NULL;
	}
	if (hwnd && !DestroyWindow(hwnd)) {
		MessageBox(NULL, L"释放窗口句柄失败", L"关闭错误", MB_OK | MB_ICONINFORMATION);
		hwnd = NULL;
	}
	if (!UnregisterClass(szWindowClass, hInst)) {
		MessageBox(NULL, L"不能注销窗口类", L"关闭错误", MB_OK | MB_ICONINFORMATION);
		hInst = NULL;
	}
}
BOOL CreateGLWindow(int width, int height, int bits, bool fullscreenflag) {
	GLuint PixelFormat = 0;//保存匹配查找结果
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
		MessageBox(NULL, L"注册窗口失败", L"错误", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	ErrorCode = GetLastError();
	RECT WindowRect;//取得矩形左上角 右下角 坐标值
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.bottom = (long)height;
	WindowRect.top = (long)0;
	fullscreen = fullscreenflag;
	ErrorCode = GetLastError();
	if (fullscreen) {
		DEVMODE dmScreenSettings = { 0 };//设备模式
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);//DEvmode结构大小
		dmScreenSettings.dmPelsWidth = width;//所选屏幕宽度
		dmScreenSettings.dmPelsHeight = height;//所选屏幕高度
		dmScreenSettings.dmBitsPerPel = bits;//每像素所选的色彩深度
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ErrorCode = GetLastError();
		
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			if (MessageBox(NULL, L"全屏模式在当前显卡上设置失败!\n使用窗口模式?", L"NeHe GL", MB_YESNO | MB_ICONINFORMATION)) {
				fullscreen = false;
			}
			else {
				MessageBox(NULL, L"程序将被关闭", L"错误", MB_OK | MB_ICONINFORMATION);
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
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);//调整窗口达到真正要求大小
	ErrorCode = GetLastError();
	// 
	//WS_CHILDWINDOW
	//
	hwnd = CreateWindowEx(dwExStyle, szWindowClass, szTitle, dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInst, NULL);
	if (!hwnd){
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"不能创建一个窗口", L"错误", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	if (fullscreen)
		ShowCursor(FALSE);

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),//大小
		1,//版本号
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,//格式支持 窗口 opengl 双缓冲
		PFD_TYPE_RGBA,//申请rgba 格式
		bits,//选定色彩深度
		0, 0, 0, 0, 0, 0,//忽略色彩位
		0,//alpha
		0,//忽略shift bit
		0,//无累加缓存
		0, 0, 0, 0,//忽略聚集位
		16,//16位 z 缓存 深度缓存
		0,//无蒙版缓存
		0,//无辅助缓存
		PFD_MAIN_PLANE,//主绘图层
		0,//Reserved
		0, 0, 0//忽略层遮罩
	};
	if (!(::hdc = GetDC(hwnd))) {
		KillGLWindow();
		MessageBox(NULL, L"不能创建一种相匹配的像素格式", L"错误", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	//windows找相应的像素格式
	if (!(PixelFormat = ChoosePixelFormat(::hdc, &pfd))) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"不能找到像素格式", L"错误", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	if (!SetPixelFormat(::hdc, PixelFormat, &pfd)) {//设置像素格式
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"不能设置像素格式", L"错误", MB_OK | MB_ICONINFORMATION);
		return false;
	}


	if (!(hrc = wglCreateContext(::hdc))) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"不能创建OpenGL渲染描述表", L"错误", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	if (!wglMakeCurrent(::hdc, hrc)) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"不能激活OpenGL渲染描述表", L"错误", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ReSizeGLScene(width, height);
	if (!InitGL()) {
		ErrorCode = GetLastError();
		KillGLWindow();
		MessageBox(NULL, L"不能创建一个窗口设备描述表", L"错误", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	return true;
}

