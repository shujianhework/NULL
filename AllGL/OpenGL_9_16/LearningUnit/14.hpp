#include "../LearningUnit.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
class Unit_14:public LearningUnit
{
private:
private:
	GLuint base;
	GLfloat rot;
	GLYPHMETRICSFLOAT gmf[256];
public:
	Unit_14(){
	}
	~Unit_14(){
	}
	void OnDraw();
	void OnInit();
	void OnKeyBoard(int Key, bool Touch);
	void OnIdle();
};

void Unit_14::OnInit(){
	base = glGenLists(256);
	HDC hdc = AppFrame::getInstance()->getDC();
	//(HDC, DWORD, DWORD, DWORD, FLOAT,
	//FLOAT, int, LPGLYPHMETRICSFLOAT);
	wglUseFontOutlines(hdc, 32, 96, base, 0, 0, 0, NULL);
}
void Unit_14::OnKeyBoard(int Key, bool Touch){
	if (Touch == false)
		return;
	switch (Key)
	{
	case VK_ESCAPE:
		AppFrame::getInstance()->Stop();
		break;
	}
}
void Unit_14::OnDraw(){
}
void Unit_14::OnIdle(){
}