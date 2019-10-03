#include "GameArea.h"
GameArea::GameArea(unsigned long areaOffset, int count)
{
	MemFile::currentFile->seek(areaOffset-0x8000000);
	_rooms.clear();
	for (int roomCounter = 0; roomCounter < count; roomCounter++)
	{
		RHeader currHeader;
		memset(&currHeader, 0, sizeof(RHeader));
		MemFile::currentFile->fread(&currHeader,1, sizeof(RHeader));
		_rooms.push_back(currHeader);
	}
}

int GameArea::GetRoomCount()
{
	return _rooms.size();
}

RHeader* GameArea::GetCurrentRoom()
{
	return &_rooms[roomIndex];
}

int GameArea::SetCurrentRoom(int index)
{
	roomIndex = index;
	return index;
}

GameArea::~GameArea()
{

}
