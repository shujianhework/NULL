#include "stdafx.h"
#include "LearningUnit.h"
LearningUnit::LearningUnit():iaTextrus(1000){}
LearningUnit::~LearningUnit(){}
static LearningUnit *LU = NULL;
LearningUnit* GetLearningUnit(){
	if (LU)
		return LU;
	else
	{
		LU = CLASSNAME;
	}
	return LU;
}
void FreeLearningUnit(){
	if (LU){
		delete LU;
	}
	LU = NULL;
}