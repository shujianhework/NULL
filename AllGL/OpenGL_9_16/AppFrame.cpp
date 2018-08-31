#include "stdafx.h"
#include "AppFrame.h"
#include <Windows.h>
AppFrame *AppFrame::instan = NULL;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
AppFrame::AppFrame() :Notify(nullptr), fullscreen(false), szWindowClass(TEXT("zuokanyoukan")), szTitle(TEXT("测试窗口"))
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
	if (MessageBox(NULL, L"你想在全屏模式下运行么？", L"设置全屏模式", MB_YESNO | MB_ICONQUESTION) == IDNO){
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
	GLuint PixelFormat = 0;//保存匹配查找结果
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
		FullDynamicParamSendData(AppFrameBackData::APBDType::ErrorAssert,TEXT("注册窗口失败"));
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
			if (FullDynamicParamSendData(AppFrameBackData::APBDType::MessageNotify, TEXT("全屏模式在当前显卡上设置失败!\n使用窗口模式?"))) {
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
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);//调整窗口达到真正要求大小
	// 
	//WS_CHILDWINDOW
	//
	hwnd = CreateWindowEx(dwExStyle, szWindowClass, szTitle, dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInst, NULL);
	if (!hwnd){
		ErrorCode = GetLastError();
		KillGLWindow();
		FullDynamicParamSendData(TEXT("不能创建一个窗口"));
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
	if (!(hdc = GetDC(hwnd))) {
		KillGLWindow();
		FullDynamicParamSendData(TEXT("不能创建一种相匹配的像素格式"));
		return false;
	}
	//windows找相应的像素格式
	if (!(PixelFormat = ChoosePixelFormat(hdc, &pfd))) {
		KillGLWindow();
		FullDynamicParamSendData(TEXT("不能找到像素格式"));
		return FALSE;
	}
	if (!SetPixelFormat(hdc, PixelFormat, &pfd)) {//设置像素格式
		KillGLWindow();
		FullDynamicParamSendData(TEXT("不能设置像素格式"));
		return false;
	}


	if (!(hrc = wglCreateContext(hdc))) {
		ErrorCode = GetLastError();
		KillGLWindow();
		FullDynamicParamSendData(TEXT("不能创建OpenGL渲染描述表"));
		return false;
	}
	if (!wglMakeCurrent(hdc, hrc)) {
		KillGLWindow();
		FullDynamicParamSendData(TEXT("不能激活OpenGL渲染描述表"));
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ReSizeGLScene(width, height);
	if (!InitGL()) {
		ErrorCode = GetLastError();
		KillGLWindow();
		FullDynamicParamSendData(TEXT("不能创建一个窗口设备描述表"));
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
			FullDynamicParamSendData(TEXT("释放DC或RC失败"));
		}
		if (!wglDeleteContext(hrc)) {
			FullDynamicParamSendData(TEXT("释放Rc失败"));
		}
		hrc = NULL;
	}
	if (hdc && !ReleaseDC(hwnd, hdc)) {
		FullDynamicParamSendData(TEXT("释放hdc失败"));
		hdc = NULL;
	}
	if (hwnd && !DestroyWindow(hwnd)) {
		FullDynamicParamSendData(TEXT("释放窗口句柄失败"));
		hwnd = NULL;
	}
	if (!UnregisterClass(szWindowClass, hInst)) {
		FullDynamicParamSendData(TEXT("不能注销窗口类"));
		hInst = NULL;
	}
}
GLvoid AppFrame::ReSizeGLScene(GLsizei width, GLsizei height) {
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
int AppFrame::InitGL(GLvoid) {
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);//启用阴影平滑
	glClearColor(0.0, 0.0, 0.0, 0.0);//设置背景色
	glClearDepth(1.0f);//设置深度缓存
	glEnable(GL_DEPTH_TEST);//启用深度测试
	glDepthFunc(GL_LEQUAL);//深度测试类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//对透视进行修正
	return TRUE;
}
LRESULT AppFrame::LoopWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
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