#pragma once
#include "BaseGame.h"
class AreasManager
{
public:
	int GetCurrentArea();
	AreasManager(char** areaList, int areaMax, SupportedTitles title);
private:
	char** _areas;
	int  _areaCount;
	int _currentArea;
	SupportedTitles _myTitle;
};