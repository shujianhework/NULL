#include "../LearningUnit.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef CLASSNAME
#define CLASSNAME new Unit_11();
#endif
class Unit_11:public LearningUnit
{
private:
	float points[45][45][3];
	int wiggle_count;
	GLfloat hold;
	float xrot;
	float yrot;
	float zrot;
public:
	Unit_11() :wiggle_count(0),xrot(0),yrot(0),zrot(0){
	}
	~Unit_11(){
	}
	void OnDraw();
	void OnInit();
	void OnKeyBoard(int Key, bool Touch);
	void OnIdle();
};

void Unit_11::OnInit(){
	Textures *texturesp = Textures::getInstance();
	iaTextrus[0] = texturesp->GetTextureData(TEXT("Data/Tim.bmp"), TextureFilterType::TFT_LINEAR);
	if (iaTextrus[0] == -1)
		printf("加载纹理失败");
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_BACK, GL_FILL);
	glPolygonMode(GL_FRONT, GL_LINE);
	for (int i = 0; i < 45; i++)
	{
		for (int j = 0; j < 45; j++)
		{
			//波浪效果
			points[i][j][0] = float((i / 5.0f) - 4.5);
			points[i][j][1] = float((j / 5.0f) - 4.5);
			//z = sin(x对应弧度值)
			points[i][j][2] = float(sin((((i/5.0f)*40)/360.0f)*2.0f*3.141592654));
		}
	}
}
void Unit_11::OnKeyBoard(int Key, bool Touch){
	if (Touch == false)
		return;
	switch (Key)
	{
	case VK_ESCAPE:
		AppFrame::getInstance()->Stop();
		break;
	}
}
void Unit_11::OnDraw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -12.0f);
	glRotatef(xrot, 1, 0, 0);
	glRotatef(yrot, 0, 1, 0);
	glRotatef(zrot, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, iaTextrus[0]);
	float x, y, w = 1.0f/44.0f, h = 1.0f/44.0f;
	glBegin(GL_QUADS);
	for (int i = 0; i < 44; i++)
	{
		for (int j = 0; j < 44; j++)
		{
			x = ((float)i) / 44.0f;
			y = ((float)j) / 44.0f;
			glTexCoord2f(x, y);
			glVertex3f(points[i][j][0], points[i][j][1], points[i][j][2]);
			glTexCoord2f(x, y+h);
			glVertex3f(points[i][j+1][0], points[i][j+1][1], points[i][j+1][2]);
			glTexCoord2f(x+w, y+h);
			glVertex3f(points[i+1][j+1][0], points[i+1][j+1][1], points[i+1][j+1][2]);
			glTexCoord2f(x+w, y);
			glVertex3f(points[i+1][j][0], points[i+1][j][1], points[i+1][j][2]);
		}
	}
	wiggle_count = rand() % 4;
	for (int w = 0; w < wiggle_count;w++){
		for (int j = 0; j < 45; j++)
		{
			hold = points[0][j][2];
			for (int i = 0; i < 44; i++)
			{
				points[i][j][2] = points[i + 1][j][2];
			}
			points[44][j][2] = hold;
		}
	}
	
	glEnd();
	xrot += 0.3;
	yrot += 0.2;
	zrot += 0.4;
}
void Unit_11::OnIdle(){}