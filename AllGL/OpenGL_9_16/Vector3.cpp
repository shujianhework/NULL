#include "stdafx.h"
#include "Vector3.h"

Vector3::Vector3(double x, double y, double z) :x(x), y(y), z(z)
{
}


Vector3::~Vector3()
{
}

void Vector3::set(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}