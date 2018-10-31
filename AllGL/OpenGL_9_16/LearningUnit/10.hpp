#include "../LearningUnit.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
class Unit_10:public LearningUnit
{
	typedef struct tagVERTEX{//顶点结构
		float x, y, z;//3d 坐标
		float u, v;//纹理坐标
		void set(std::string s){
			sscanf(s.c_str(), "%f %f %f %f %f", &x, &y, &z, &u, &v);
		}
	}VERTEX;
	typedef struct tagTRIANGLE{//三角形
		VERTEX vertex[3];//矢量数组
	}TRIANGLE;
	typedef struct tagSECTOR{//创建Sector区段结构
		int numtriangles;
		TRIANGLE *triangle;
	}SECTOR;
private:
	const char *worldfile;
	SECTOR sector;
	float yrot;
	float xrot;
	float heading;
	float xpos;
	float zpos;
	float ypos;
	float walkbiasangle;
	float walkbias;
	float lookupdown;
private:
	void SetupWorld();
public:
	Unit_10() :worldfile("Data/World.txt"){
		sector.numtriangles = 0;
		sector.triangle = NULL;
		yrot= 0.0f;
		xrot= 0.0f;
		heading= 0.0f;
		xpos= 0.0f;
		zpos= 0.0f;
		ypos= 0.0f;
		walkbiasangle= 0.0f;
		walkbias= 0.0f;
		lookupdown= 0.0f;
	}
	~Unit_10(){
		if (sector.triangle)
			delete[] sector.triangle;
		sector.triangle = NULL;
	}
	void OnDraw();
	void OnInit();
	void OnKeyBoard(int Key, bool Touch);
	void OnIdle();
};

void Unit_10::OnInit(){
	SetupWorld();
	Textures *texturesp = Textures::getInstance();
	iaTextrus[0] = texturesp->GetTextureData(TEXT("Data/Tim.bmp"), TextureFilterType::TFT_LINEAR);
	//iaTextrus[0] = texturesp->GetTextureData(TEXT("Data/Mud.bmp"), TextureFilterType::TFT_LINEAR);
	if (iaTextrus[0] == -1)
		printf("加载纹理失败");
	glEnable(GL_TEXTURE_2D);
}
void Unit_10::OnKeyBoard(int Key, bool Touch){
	if (Touch == false)
		return;
	switch (Key)
	{
	case VK_ESCAPE:
		AppFrame::getInstance()->Stop();
		break;
	}
}
void Unit_10::OnDraw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	GLfloat xtrans = -xpos;
	GLfloat ytrans = -walkbias - 0.25f;
	GLfloat ztrans = -zpos;
	GLfloat sceneroty = 360.0f - yrot;
	glRotatef(lookupdown, 1, 0,0);
	glRotatef(sceneroty, 0, 1, 0);
	glTranslatef(xtrans, ytrans, ztrans);
	glBindTexture(GL_TEXTURE_2D, iaTextrus[0]);
	for (int i = 0; i < sector.numtriangles; i++){
		glBegin(GL_TRIANGLES);
		glNormal3f(0, 0, 1);
		for (int j = 0; j < 3; j++)
		{
			glTexCoord2f(sector.triangle[i].vertex[j].u, sector.triangle[i].vertex[j].v);
			glVertex3f(sector.triangle[i].vertex[j].x, sector.triangle[i].vertex[j].y, sector.triangle[i].vertex[j].z);
		}
		glEnd();
	}
}
void Unit_10::OnIdle(){
	AppFrame *frame = AppFrame::getInstance();
	if (FK(VK_RIGHT)){
		yrot += 0.15f;
	}
	if (FK(VK_LEFT)){
		yrot -= 0.15f;
	}

	if (FK(VK_PRIOR))
	{
		lookupdown -= 1.0f;
	}

	if (FK(VK_NEXT)){
		lookupdown += 1.0f;
	}
	if (FK(VK_UP)){
		xpos -= (float)sin(heading*piover180)*0.0f;
		zpos -= (float)cos(heading*piover180)*0.0f;
		if (walkbiasangle >= 359.0f){
			walkbiasangle = 0.0f;
		}
		else
			walkbiasangle += 10;
		walkbias = (float)sin(walkbiasangle*piover180) / 20.0f;
	}
	if (FK(VK_DOWN)){
		xpos += (float)sin(heading*piover180)*0.0f;
		zpos += (float)cos(heading*piover180)*0.0f;
		if (walkbiasangle <= 1.0f){
			walkbiasangle = 359.0f;
		}
		else
			walkbiasangle -= 10;
		walkbias = (float)sin(walkbiasangle*piover180) / 20.0f;
	}
}
void Unit_10::SetupWorld(){
	FILE *filein = fopen(worldfile, "r");
	if (filein == NULL)
		return;
	char *p = new char[4096];
	std::vector<std::string> datas;
	do
	{
		fgets(p, 4096, filein);
		if (strlen(p) > 10){
			if (p[0] == '/' && p[1] == '/')
				continue;
			else
				datas.push_back(p);
		}
	} while (!feof(filein));
	int len = 0;
	sscanf(datas[0].c_str(), "NUMPOLLIES %d\n", &len);
	int idx = 0;
	sector.numtriangles = len;
	sector.triangle = new TRIANGLE[len];
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			idx++;
			sector.triangle[i].vertex[j].set(datas[idx]);
		}
	}
	delete[] p;
	fclose(filein);
}