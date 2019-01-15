#pragma once
#define UNIT_N 13

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLAux.h>
#include <vector>
#include "Textures.h"
#include "AppFrame.h"
#define piover180 0.0174532925f
#define FK(K) (frame->FindKey((K)))
class LearningUnit
{
protected:
	std::vector<unsigned int> iaTextrus;
public:
	LearningUnit();
	virtual ~LearningUnit();
	virtual void OnDraw() = 0;
	virtual void OnInit() = 0;
	virtual void OnKeyBoard(int Key, bool Touch) = 0;
	virtual void OnIdle() = 0;
};
LearningUnit* GetLearningUnit();
void FreeLearningUnit();