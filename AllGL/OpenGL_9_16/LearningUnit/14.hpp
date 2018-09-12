#include "../LearningUnit.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
class Unit_14:public LearningUnit
{
private:
private:
public:
	Unit_14(){
	}
	~Unit_14(){
	}
	void OnDraw();
	void OnInit();
	void OnKeyBoard(int Key, bool Touch);
	void OnIdle();
};

void Unit_14::OnInit(){}
void Unit_14::OnKeyBoard(int Key, bool Touch){
	if (Touch == false)
		return;
	switch (Key)
	{
	case VK_ESCAPE:
		AppFrame::getInstance()->Stop();
		break;
	}
}
void Unit_14::OnDraw(){}
void Unit_14::OnIdle(){}