#include "GameArea.h"
GameArea::GameArea(unsigned long areaOffset, int count)
{
	if (areaOffset == 0)
	{
		throw new exception("oh no");
	}
	
	_rooms.clear();

	for (int roomCounter = 0; roomCounter < count; roomCounter++)
	{		
		_rooms.push_back((RHeader*)&MemFile::currentFile->GetFile()[areaOffset - 0x8000000+(0x3c*roomCounter)]);	
	}
}

int GameArea::GetRoomCount()
{
	return _rooms.size();
}

RHeader* GameArea::GetCurrentRoom()
{
	return _rooms[roomIndex];
}

int GameArea::SetCurrentRoom(int index)
{
	roomIndex = index;
	return index;
}

GameArea::~GameArea()
{

}
