#pragma once
#include "GameConfiguration.h"
#include "Structures.h"
#include "GBAMethods.h"
#include "MemFile.h"
struct sctype {
	unsigned char xStart;
	unsigned char xEnd;
	unsigned char yStart;
	unsigned char yEnd;
};


struct scrollData {
	MousePointer rect;
	unsigned long unkData;

};
class Scroller {
	sctype privScrolls[0xFF];
public:
	Scroller()
	{
	}
	~Scroller()
	{
	}
	unsigned char Room;
	unsigned char Number;
	int oldScrollCount;
	std::vector<scrollData> Scrolls;
};


class clsRoomScrolls
{

private:
	Scroller mScrollInfo;

public:
	clsRoomScrolls();
	~clsRoomScrolls();
	DataContainer* scrollInfoContainer;
	int GetScroll(FILE* fp);
	int SaveScroll(GBAMethods* mGBA);
	int initScroll(FILE* fp, int area, int room);
	int EditScrollData(GBAMethods* mGBA);
	Scroller* GetScrollInfo();
	long RoomScrollOffset;
	int             Findmeascroll(int X, int Y, int ScrollNum);
	long		pnt2ScrollPointer;
	long        scrollpnt;
	void GetScrollArray(FILE* fp);
};

