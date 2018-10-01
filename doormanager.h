#pragma once
//#include "MainHeader.h"
#include "GameConfiguration.h"
#include "ConnectionsEditor.h"
#include <vector>
using namespace std;
class vDoor {
public: 
	sDoor rawDoor;
     MousePointer virtualDoor; 
	vDoor(sDoor src);
};
class DoorManager
{
public:
	int SaveThisDoor(int DoorNo);
	int ConnectDoor(unsigned char TD);
	int AddDoor(int RoomNum);
	int OriginalDoorCount; 
	DoorManager();
	~DoorManager();
	int GetCount();
	int DeleteDoor(int Room, int doorNum);//Send it the currentroomindexes 
	int SetupDoors(long area);
	int LoadDoors(int Room);
	int DisplayDoors(unsigned char Room);
	int SaveDoors();
	int LoadThisDoor(int DoorNo);
	//sDoor Doors[0xFF];
	vector< vDoor> Doors;
	unsigned short DoorNum(); 
	//DoorStruct  DoorBuf;
	vector<int> CurrentRoomDoorIndexes;
	int GetDoorArray(FILE* fp);
	int SaveDoorOffsets();
	int DoorCount;
	int             GetDoor(int X, int Y);
	unsigned long GetAreaOffset(int Area);
	DataContainer* doorInfoContainer;
	unsigned long SetAreaOffset(int Area, int newOffset);
};

