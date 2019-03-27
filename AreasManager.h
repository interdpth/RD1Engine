#pragma once
#include "BaseGame.h"
#include "GameArea.h"
#include <vector>
using namespace std;
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
	vector<GameArea> areas;
};