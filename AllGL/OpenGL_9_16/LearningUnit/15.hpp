#include "../LearningUnit.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
class Unit_:public LearningUnit
{
private:
private:
public:
	Unit_(){
	}
	~Unit_(){
	}
	void OnDraw();
	void OnInit();
	void OnKeyBoard(int Key, bool Touch);
	void OnIdle();
};

void Unit_::OnInit(){}
void Unit_::OnKeyBoard(int Key, bool Touch){
	if (Touch == false)
		return;
	switch (Key)
	{
	case VK_ESCAPE:
		AppFrame::getInstance()->Stop();
		break;
	}
}
void Unit_::OnDraw(){}
void Unit_::OnIdle(){}
