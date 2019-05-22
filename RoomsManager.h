#pragma once
#include "RoomClass.h"
class RoomsManager
{
private:
	unsigned long _romAddress;
	//std::vector<RoomClass> _rooms;
	//RoomClass* _currentRoom;
	int _roomIndex; 

	
public:
	void SetCurrentRoom(int index);
	RoomsManager(MemFile* rom, unsigned long address, int count);
	~RoomsManager();
};

