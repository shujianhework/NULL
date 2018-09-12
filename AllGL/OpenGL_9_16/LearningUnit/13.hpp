#include "../LearningUnit.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
class Unit_13:public LearningUnit
{
private:
	GLuint base;
	GLfloat cnt1;
	GLfloat cnt2;
private:
	GLvoid BuildLists();
	GLvoid BuildFont(GLvoid);
	GLvoid glPrint(const char *fmt, ...);
public:
	Unit_13(){
		GLfloat t[5][3] = {
			{ 1, 0, 0 }, { 0, 1, 0 }, { 1, 0.5, 0 }, { 1, 1, 0 }, { 1, 0, 1 }
		};
		GLfloat t1[5][3] = {
			{ 0, 1, 1 }, { 0, 1, 0.5 }, { 0, 0.5, 1 }, { 1, 0, 0.5 }, { 0.25, 1, 0.75 }
		};
		base = 0;
		cnt1 = 0;
		cnt2 = 0;
	}
	~Unit_13(){
		glDeleteLists(base, 96);
	}
	void OnDraw();
	void OnInit();
	void OnKeyBoard(int Key, bool Touch);
	void OnIdle();
};

void Unit_13::OnInit(){
	BuildFont();
	//Textures *texturesp = Textures::getInstance();
	/*iaTextrus[0] = texturesp->GetTextureData(TEXT("Data/Cube.bmp"), TextureFilterType::TFT_LINEAR);
	if (iaTextrus[0] == -1)
		printf("¼ÓÔØÎÆÀíÊ§°Ü");
	BuildLists();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);*/
}
void Unit_13::OnKeyBoard(int Key, bool Touch){
	if (Touch == false)
		return;
	switch (Key)
	{
	case VK_ESCAPE:
		AppFrame::getInstance()->Stop();
		break;
	}
}
void Unit_13::OnDraw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -1);
	glColor3f(1.0f*float(cos(cnt1)), 1.0f* float(sin(cnt2)), 1.0f - 0.5f*float(cos(cnt1 + cnt2)));
	glRasterPos2f(-0.45f + 0.0f*float(cos(cnt1)), 0.35*float(sin(cnt2)));
	glPrint("Activate OpenGL Text With -%f7.2f", cnt1);
	cnt1 += 0.051f;
	cnt2 += 0.005f;
}
void Unit_13::OnIdle(){
}
GLvoid Unit_13::BuildFont(GLvoid){
	HFONT font;
	HFONT oldfont;
	base = glGenLists(96);
	font = CreateFont(-24,
		0, 0, 0,
		FW_BOLD,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "ËÎÌå");
	HDC hdc = AppFrame::getInstance()->getDC();
	oldfont = (HFONT)SelectObject(hdc, font);
	wglUseFontBitmaps(hdc, 32, 96, base);
	SelectObject(hdc, oldfont);
}
GLvoid Unit_13::glPrint(const char *fmt, ...){
	char text[256];
	va_list ap;
	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}