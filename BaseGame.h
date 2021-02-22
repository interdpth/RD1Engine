#pragma once

#include "MetroidFusion.h"
#include "ZeroMission.h"
#include "WarioLand.h"
#include "RoomClass.h"
#include "cOAMManager.h"
//#include "clsScroll.h"
#include "Logger.h"
#include "clsRoomScrolls.h"
#include <vector>
#include <map>
#include "LeakFinder.h"
#include "..\Win32GBAImageClasses\backbuffer.h"
#include "OamFrameTable.h"
#include "BaseTypes.h"
#include "AreasManager.h"
//#include "RoomsManager.h"
using namespace std;
int  DrawTileRect(HDC hdc, long theColor, MousePointer mpointer, int mul);
class RoomClass;
class DoorManager;
class TilesetManager;
class cOAMManager;
int GetBitField(long reg, long bitStart, long bitLength);
class RD1Engine
{
public:
	TileBuffer *_bgBuffer;
	Image* _tileset;
	Logger* _theLog;
	std::map<int, PosModify> poseModifier;
	static RD1Engine* theGame;
	GBAMethods* _gbaMethods;
	TilesetManager* mgrTileset;
	BaseTitle* titleInstance;
	//ZeroMission* titleInstance;
	
	//AreasManager* AreaManager;
	RoomClass* mainRoom;
	SupportedTitles thisTitle;
	cOAMManager*mgrOAM;
	OamFrameTable* frameTables;
	AreasManager* areaManager;
	int DisplayDoors(unsigned char Room);
	drawstate DrawStatus;
	int             DrawClipIdent();
	int DrawScrolls(int ScrollToDraw);
	int LoadAreaTable();
	void LoadModifiers(char* fn);
	void LoadRoomSpriteSet(int Area, int Offset, Image* Tileset, TileBuffer* SpriteImage, int spriteset = 0);
	void LoadRoom(int Area, int Offset, Image* Tileset, TileBuffer* SpriteImage, unsigned long offset);
	
	
	void LoadRoom(int Area, int Offset, Image* Tileset, TileBuffer* SpriteImage, RHeader*currHeader
	);

	void LoadROM();
	void DrawSprites(Image* pic);
	int DrawLayer(nMapBuffer* Map, Image* pic, unsigned char ctype);
	int DrawRoom(TileBuffer* TileImage, TileBuffer* BGImage, int ScrollIndex);
	void DumpAreaAsImage(char* fn, Image* Tileset, TileBuffer* SpriteImage, TileBuffer* tileImage, TileBuffer* bgImage);
	void DrawDoorIndicators(HDC g);
	

	clsRoomScrolls * mgrScrolls;
	RD1Engine(SupportedTitles theTitle, OamFrameTable* frametables, TileBuffer * bg, TileBuffer* TileImage, Image* Tileset);
	int GetPalSize(int sprID);
	BackBuffer ThisBackBuffer;
	int currentRomType;
	DoorManager * mgrDoors;
	sSpritev SpriteVRAM;
	int             Save(MemFile * fp);
	int SaveLevel();
	bool CheckHeader(RHeader* tstHeader);
	bool IsValidPointer(unsigned long pnt);
	bool IsValidCompression(unsigned char tstByte);
		int GetRoomCount(long RoomTableOffset);
    void GetArrays();
	~RD1Engine();
	bool roomLock;
};

