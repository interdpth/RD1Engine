#include "AreasManager.h"


AreasManager::AreasManager(char** areaList, int areaMax, SupportedTitles title)
{
	_areas = areaList;
	_areaCount = areaMax;
	_myTitle = title;
}