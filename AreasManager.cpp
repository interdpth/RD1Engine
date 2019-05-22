#include "AreasManager.h"


AreasManager::AreasManager(MemFile* file, char** areaList, int areaMax, SupportedTitles title)
{
	_rom = file;
	_areas = areaList;
	_areaCount = areaMax;
	_myTitle = title;
}


int AreasManager::GetCurrentArea() { return _currentArea; }
int AreasManager::SetCurrentArea(int index) { _currentArea = index; return index; }
void AreasManager::Load(unsigned long offset) {}
void AreasManager::Save() {}