#include "../LearningUnit.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
class Unit_9:public LearningUnit
{

	BOOL twinkle;
	BOOL tp;
	const int Num = 200;
	typedef struct{
		int r, g, b;
		GLfloat dist;
		GLfloat angle;
	}starts;
	std::vector<starts*> star;
	GLfloat zoom = -15.0f;
	GLfloat tilt = 90.0f;
	GLfloat spin;
	GLuint loop;
public:
	Unit_9():Num(200),zoom(-15.0f),tilt(90.0f),star(200){
		char *p = NULL;
		for (int i = 0; i < Num; i++)
		{
			p = (char*)new starts();
			memset(p, 0, sizeof(starts));
			star[i] = (starts*)p;
		}
	}
	~Unit_9(){
		for (int i = 0; i < Num; i++)
		{
			delete star[i];
		}
		star.clear();
	}
	void OnDraw();
	void OnInit();
	void OnKeyBoard(int Key, bool Touch);
	void OnIdle();
};

void Unit_9::OnInit(){
	Textures *texturesp = Textures::getInstance();
	iaTextrus[0] = texturesp->GetTextureData(TEXT("Data/Tim.bmp"), TextureFilterType::TFT_LINEAR);
	//iaTextrus[0] = texturesp->GetTextureData(TEXT("Star.bmp"), TextureFilterType::TFT_LINEAR);
	if (iaTextrus[0] == -1)
		printf("加载纹理失败");
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	srand(time(NULL));
	for (loop = 0; loop < Num; loop++)
	{
		star[loop]->angle = 0.0f;
		star[loop]->dist = (float(loop) / Num)*5.0f;
		star[loop]->r = rand() % 256;
		star[loop]->g = rand() % 256;
		star[loop]->b = rand() % 256;
	}
}
void Unit_9::OnKeyBoard(int Key, bool Touch){
	switch (Key)
	{
	case VK_ESCAPE:
		if (Touch)
			AppFrame::getInstance()->Stop();
		break;
	}
}
void Unit_9::OnDraw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕和深度缓存
	glBindTexture(GL_TEXTURE_2D, iaTextrus[0]);
	for (int i = 0; i < Num; i++)
	{
		glLoadIdentity();
		glTranslatef(0, 0, zoom);
		glRotatef(tilt, 1, 0, 0);
		glRotatef(star[i]->angle, 0, 1, 0);
		glTranslatef(star[i]->dist, 0, 0);
		glRotatef(-star[i]->angle, 0, 1, 0);
		glRotatef(-tilt, 1, 0, 0);
		if (twinkle){
			glColor4ub(star[Num - i - 1]->r, star[Num - i - 1]->g, star[Num - i - 1]->b, 255);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);			glVertex3f(-1, -1, 0);
			glTexCoord2f(1, 0);			glVertex3f(1, -1, 0);
			glTexCoord2f(1, 1);			glVertex3f(1, 1, 0);
			glTexCoord2f(0, 1);			glVertex3f(-1, 1, 0);
			glEnd();
		}
		glRotatef(spin, 0, 0, 1);
		glColor4ub(star[Num - i - 1]->r, star[Num - i - 1]->g, star[Num - i - 1]->b, 255);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);		glVertex3f(-1, -1, 0);
		glTexCoord2f(1, 0);		glVertex3f(1, -1, 0);
		glTexCoord2f(1, 1);		glVertex3f(1, 1, 0);
		glTexCoord2f(0, 1);		glVertex3f(-1, 1, 0);
		glEnd();
		spin += 0.2f;
		star[i]->angle += (float)i / Num;
		star[i]->dist -= 0.02f;
		if (star[i]->dist < 0){
			star[i]->dist = 5;
			star[i]->r = rand() % 256;
			star[i]->g = rand() % 256;
			star[i]->b = rand() % 256;
		}
	}
}
void Unit_9::OnIdle(){
	auto frame = AppFrame::getInstance();
	if (FK('T') && !tp){
		tp = TRUE;
		twinkle = !twinkle;
	}
	if (!FK('T')){
		tp = false;
	}
	if (FK(VK_UP))
		tilt -= 0.5f;
	if (FK(VK_DOWN))
		tilt += 0.5f;
	if (FK(VK_PRIOR))
		zoom -= 0.2f;
	if (FK(VK_NEXT))
		zoom += 0.2f;
}