#include "../LearningUnit.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define PLKK Unit_##UNIT_N
#ifndef CLASSNAME
#define CLASSNAME new Unit_13();//PLKK();
#endif
class Unit_13:public LearningUnit
{
private:
private:
	GLvoid BuildLists();
public:
	Unit_13(){
		GLfloat t[5][3] = {
			{ 1, 0, 0 }, { 0, 1, 0 }, { 1, 0.5, 0 }, { 1, 1, 0 }, { 1, 0, 1 }
		};
		GLfloat t1[5][3] = {
			{ 0, 1, 1 }, { 0, 1, 0.5 }, { 0, 0.5, 1 }, { 1, 0, 0.5 }, { 0.25, 1, 0.75 }
		};
	}
	~Unit_13(){
	}
	void OnDraw();
	void OnInit();
	void OnKeyBoard(int Key, bool Touch);
	void OnIdle();
};

void Unit_13::OnInit(){
	Textures *texturesp = Textures::getInstance();
	/*iaTextrus[0] = texturesp->GetTextureData(TEXT("Data/Cube.bmp"), TextureFilterType::TFT_LINEAR);
	if (iaTextrus[0] == -1)
		printf("加载纹理失败");
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
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, iaTextrus[0]);
	float tx, ty, tz = -20.0f, rx, ry;
	for (int i = 1; i < 6; i++)
	{
		for (int j = 0; j < i; j++)
		{
			glLoadIdentity();
			tx = 1.4f + (float(j)*2.8f) - (float(i)*1.4f);
			ty = ((6.0f - float(i))*2.4f) - 7.0f;
			rx = 45.0f - (2.0f*i) + xrot;
			ry = 45.0f + yrot;
			glTranslatef(tx,ty,-20.0f);
			glRotatef(rx, 1.0f, 0, 0);
			glRotatef(ry, 0, 1, 0);
			glColor3fv(boxcol[i - 1]);
			glCallList(box);
			glColor3fv(topcol[i - 1]);
			glCallList(top);
			glFlush();
		}
	}*/
}
void Unit_13::OnIdle(){
	/*auto frame = AppFrame::getInstance();
	if (FK(VK_LEFT))
	{
		yrot -= 0.2f;
	}
	if (FK(VK_RIGHT))
	{
		yrot += 0.2f;
	}
	if (FK(VK_UP))
	{
		xrot -= 0.2f;
	}
	if (FK(VK_DOWN))
	{
		xrot += 0.2f;
	}*/
}
GLvoid Unit_13::BuildLists(){
	//box = glGenLists(2);
	//glNewList(box, GL_COMPILE);
	//glBegin(GL_QUADS);
	////底部
	//glTexCoord2f(1, 1); glVertex3f(-1, -1, -1);
	//glTexCoord2f(0, 1); glVertex3f(1, -1, -1);
	//glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
	//glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
	////前部
	//glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);
	//glTexCoord2f(1, 0); glVertex3f(1, -1, 1);
	//glTexCoord2f(1, 1); glVertex3f(1, 1, 1);
	//glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);
	////后部
	//glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	//glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
	//glTexCoord2f(0, 1); glVertex3f(1, 1, -1);
	//glTexCoord2f(0, 0); glVertex3f(1, -1, -1);

	////右部
	//glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
	//glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
	//glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
	//glTexCoord2f(0, 0); glVertex3f(1, -1, 1);

	////左部
	//glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	//glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
	//glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);
	//glTexCoord2f(0, 0); glVertex3f(-1, -1, 1);

	////上部
	//glTexCoord2f(1, 0); glVertex3f(-1, 1, -1);
	//glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
	//glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
	//glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
	//glEnd();
	//glEndList();
	//top = box + 1;
	//glNewList(top, GL_COMPILE);
	//glBegin(GL_QUADS);
	////上面
	//glTexCoord2f(1, 0); glVertex3f(-1, 1, -1);
	//glTexCoord2f(1, 1); glVertex3f(1, 1, -1);
	//glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
	//glTexCoord2f(0, 0); glVertex3f(-1, 1, 1);
	//glEnd();
	//glEndList();
}
