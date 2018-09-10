#pragma once
#include <math.h>
#define ISZEOR(x) (abs((x)-0) < 0.0000001)
#define S_PI 3.1415926535897
#define CORRECTANG(x) (x) = (x)%360;(x) = (x)>=0 ? (x) : 360+(x)
#define Ang2Rag(x) (S_PI*(x))/180
#define Rag2Ang(x) ((x)*180)/S_PI
struct Vector3
{
	double x;
	double y;
	double z;
	void set(double x, double y = 0.0, double z = 0.0);
	void set(Vector3 v){
		set(v.x, v.y, v.z);
	}
	Vector3(double x,double y=0.0,double z = 0.0);
	~Vector3();
};

