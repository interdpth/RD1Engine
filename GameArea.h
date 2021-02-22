#pragma once
#include <vector>
#include "structures.h"
#include "..\MemFile\MemFile.h"
class GameArea
{
	std::vector<RHeader*> _rooms;
	int roomIndex;
	int originalCount;
public:
	
	RHeader* GetCurrentRoom();
	int SetCurrentRoom(int index);
	int GetRoomCount();
	void AddBlankRoom();
	void Save();
	GameArea(unsigned long areaOffset, int count);
	~GameArea();
};

