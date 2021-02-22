#pragma once
#include "GameConfiguration.h"
#include "Structures.h"
#include "GBAMethods.h"
#include "..\MemFile\MemFile.h"
#include "ObjectScroll.h"


class clsRoomScrolls
{


public:
	unsigned char Room;
	int oldScrollCount;
	std::vector<ObjectScroll*> Scrolls;
	clsRoomScrolls();
	~clsRoomScrolls();
	DataContainer* scrollInfoContainer;
	int GetScroll();
	int SaveScroll(GBAMethods* mGBA);
	int initScroll( int area, int room);
	int EditScrollData(GBAMethods* mGBA);
	void LoadScrollControls();

	void LoadScrollInfo(int s);
	long RoomScrollOffset;
	int             Findmeascroll(int X, int Y, int ScrollNum);
	long		pnt2ScrollPointer;
	long        scrollpnt;
	void GetScrollArray();
};

