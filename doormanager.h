#pragma once
//#include "MainHeader.h"
#include "GameConfiguration.h"
#include "ConnectionsEditor.h"
#include "structures.h"
#include "..\MemFile\MemFile.h"
#include "GBAMethods.h"
#include <vector>
#include "ObjectDoor.h"
using namespace std;

class DoorManager
{
private:
	GBAMethods* _gbaMethods;
public:
	int SaveThisDoor(int DoorNo);
	int ConnectDoor(unsigned char TD);
	int AddDoor(int RoomNum);
	int OriginalDoorCount; 
	DoorManager(GBAMethods* gba);
	~DoorManager();
	int GetCount();
	int DeleteDoor(int Room, int doorNum);//Send it the currentroomindexes 
	int SetupDoors(long area);
	int LoadDoors(int Room);
	
	int SaveDoors(int area);
	int LoadDoor(int DoorNo);

	vector<ObjectDoor*> Doors;
	unsigned short DoorNum(); 
	//DoorStruct  DoorBuf;
	vector<int> CurrentRoomDoorIndexes;
	int GetDoorArray();
	int SaveDoorOffsets();
	int DoorCount;
	int  GetDoor(int Room, int X, int Y);
	unsigned long GetAreaOffset(int Area);
	DataContainer* doorInfoContainer;
	unsigned long SetAreaOffset(int Area, int newOffset);
};

