#pragma once
#include "FNode.h"
#include "Vector3.h"
#include "Size.h"
#include <vector>
class FBox :
	public FNode
{
	Vector3 Point;
	Vector3 Rorate;
	Size size;
	FBox();
	std::vector<Vector3*> PointSequences;
	std::vector<Vector3*> ColorSequences;
	int PointIdMapColor[24];
	void updatePoints();
	std::vector<char> PointMapping;
public:
	//原点和长宽高
	FBox(Vector3 Point,Size size);
	void AddRotate(double x,double y,double z);
	void AddRotate(Vector3 ang);
	void setRotate(double x, double y, double z);
	void setRotate(Vector3 ang);
	bool setPageColor(int index, Vector3 rgb);
	bool setVertexColor(int index, Vector3 rgb);
	inline bool setBoxColor(Vector3 rgb){
		for each (auto var in ColorSequences)
		{
			var->set(rgb);
		}
		return true;
	}
	void Draw();
	~FBox();
};

