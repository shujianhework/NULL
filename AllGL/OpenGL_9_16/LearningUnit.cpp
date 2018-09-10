#include "stdafx.h"
#include "LearningUnit.h"

#if UNIT_N == 9
#include "LearningUnit\9.hpp"
#elif UNIT_N == 10
#include "LearningUnit\10.hpp"
#else
#endif
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