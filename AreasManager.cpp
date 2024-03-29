#include "AreasManager.h"

AreasManager::AreasManager(MemFile* file, vector<string>* areaList, int areaMax, SupportedTitles title)
{
	_rom = file;
	for (int n = 0; n < areaList->size(); n++)
	{
		_areaNames.push_back(areaList->at(n));
	}
	_areaCount = areaMax;
	_myTitle = title;
	_currentArea = 0;
	_currentOffset = 0;
}


GameArea* AreasManager::GetCurrentArea() { return areas[_currentArea]; }
int AreasManager::SetCurrentArea(int index) { _currentArea = index; return index; }
void AreasManager::LoadAreas(unsigned long offset) {
	MemFile::currentFile->seek(offset);
	DataContainer* areasData = GameConfiguration::mainCFG->GetDataContainer("RoomsPerArea");
	tableOffset = offset;
	for (int i = 0; i < _areaCount; i++)
	{
		unsigned long gamePointer = 0;
		unsigned long curDAD = 0;
		MemFile::currentFile->fread(&gamePointer, sizeof(long), 1);
		curDAD = MemFile::currentFile->ftell();
		if (gamePointer == 0)
		{
			throw new exception("oh no");
		}
		areas.push_back(new GameArea(gamePointer, areasData->DataArray[i]));
		MemFile::currentFile->seek(curDAD);

	}
}
void AreasManager::Save() {


}