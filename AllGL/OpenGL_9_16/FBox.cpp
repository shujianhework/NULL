#include "stdafx.h"
#include "FBox.h"
#include <cmath>
#include <ctime>
const static int arrs[] = { 0, 1, 2, 3, 4, 5, 6, 7,
							0, 1, 5, 4, 3, 2, 6, 7,
							0, 4, 7, 3, 1, 5, 6, 2 };
const static int arrcolorpage[] = { 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6};
FBox::FBox(Vector3 Point, Size size) :Point(Point), size(size), PointSequences(8), ColorSequences(24), PointMapping(24), Rorate({0})
{
	for (int i = 0; i < 8; i++)
		PointSequences[i] = new Vector3(0);
	for (int i = 0; i < 24; i++)
		ColorSequences[i] = new Vector3(0);
	srand(time(NULL));
	double r, g, b;
	for (int i = 0; i < 24; i++)
	{
		r = rand() % 100;
		g = rand() % 100;
		b = rand() % 100;
		ColorSequences[i]->set( r / 100, g / 100, b / 100);
	}
	
	updatePoints();
}


FBox::~FBox()
{
	for (int i = 0; i < PointSequences.size(); i++)
	{
		delete PointSequences[i];
	}
	PointSequences.clear();
}
void FBox::updatePoints(){
	//ÕýÃæ
	*(PointSequences[0]) = Point;
	
	PointSequences[1]->set(Point.x, Point.y + size.w, Point.z);
	PointSequences[2]->set(Point.x+size.l, Point.y + size.w, Point.z);
	PointSequences[3]->set(Point.x+size.l, Point.y, Point.z);
	//±³Ãæ
	PointSequences[4]->set(Point.x, Point.y, Point.z + size.h);
	PointSequences[5]->set(Point.x, Point.y + size.w, Point.z + size.h);
	PointSequences[6]->set(Point.x + size.l, Point.y + size.w, Point.z + size.h);
	PointSequences[7]->set(Point.x + size.l, Point.y, Point.z + size.h);
	for (int i = 0; i < 24; i++)
	{
		PointMapping[i] = (char)arrs[i];
	}
}
bool FBox::setVertexColor(int index, Vector3 rgb){
	if (index < 1 || index > 8)
		return false;
	index--;
	Vector3 *v = NULL;
	for (int i = 0; i < 24; i++)
	{
		if (arrs[i] == index){
			ColorSequences[i]->set(rgb);
		}
	}
	return true;
}
bool FBox::setPageColor(int index, Vector3 rgb){
	for (int i = 0; i < 24; i++)
	{
		if (index == arrcolorpage[i]){
			printf("i = %d",i);
			ColorSequences[i]->set(rgb);
		}
	}
	return true;
}
void FBox::AddRotate(double x, double y, double z){
	Vector3 v3 = { x, y, z };
	AddRotate(v3);
}
void FBox::AddRotate(Vector3 ang){
	int iang = 0;

	if (ang.x > 1){
		iang = ang.x;
		ang.x = CORRECTANG(iang);
	}
	if (ang.y > 1){
		iang = ang.y;
		ang.y = CORRECTANG(iang);
	}
	if (ang.z > 1){
		iang = ang.z;
		ang.z = CORRECTANG(iang);
	}
	ang.x = Ang2Rag(ang.x);
	ang.y = Ang2Rag(ang.y);
	ang.z = Ang2Rag(ang.z);
	if (ISZEOR(ang.x) && ISZEOR(ang.y) && ISZEOR(ang.z)){
		return;
	}
}
void FBox::setRotate(double x, double y, double z){
	if (ISZEOR(x) && ISZEOR(y) && ISZEOR(z)){
		return;
	}
}
void FBox::setRotate(Vector3 ang){
	setRotate(ang.x, ang.y, ang.z);
}
void FBox::Draw(){
	Vector3* var = NULL;	
	for (int i = 0; i < 24; i++)
	{
		var = PointSequences[PointMapping[i]];
		glVertex3f(var->x, var->y, var->z);
		var = ColorSequences[i];
		glColor3f(var->x, var->y, var->z);
	}
}