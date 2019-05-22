#pragma once
#include "BaseGame.h"
#include "GameArea.h"
#include "RoomsManager.h"
#include <vector>
#include "MemFile.h"
using namespace std;
class AreasManager
{
public:
	int GetCurrentArea();
	int SetCurrentArea(int index);
	AreasManager(MemFile* file, char** areaList, int areaMax, SupportedTitles title);
	void Load(unsigned long offset);
	void Save();
private:
	MemFile* _rom;
	char** _areas;
	int  _areaCount;
	int _currentArea;
	unsigned long _currentOffset;
	SupportedTitles _myTitle;
	vector<GameArea> areas;
};