#pragma once
#include "..\MemFile\MemFile.h"
#include "BaseTypes.h"
#include "GameConfiguration.h"
#include "GameArea.h"
#include <vector>

using namespace std;
class AreasManager
{
public:
	GameArea* GetCurrentArea();
	int SetCurrentArea(int index);
	
	AreasManager(MemFile* file, vector<string>*, int areaMax, SupportedTitles title);
	void LoadAreas(unsigned long offset);
	void Save();

private:
	MemFile* _rom;
	vector<string> _areaNames;
	int  _areaCount;
	int _currentArea;
	unsigned long _currentOffset;
	SupportedTitles _myTitle;
	vector<GameArea> areas;
	unsigned long tableOffset;
};