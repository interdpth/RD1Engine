#pragma once
#include <vector>
#include "RoomClass.h"
class GameArea
{
	std::vector<RoomClass> _rooms;
	int roomIndex;
public:
	RoomClass* GetCurrentRoom();
	void AddBlankRoom();
	void Save();
	GameArea(unsigned long areaOffset);
	~GameArea();
};

