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
#include "backbuffer.h"
#include "OamFrameTable.h"

using namespace std;
int  DrawTileRect(HDC hdc, long theColor, MousePointer mpointer, int mul);
struct PosModify
{
	int x;
	int y;
};

enum SupportedTitles
{
	titleZM = 0,
	titleMF,
	titleWL,
	titleUnsupport
};

struct drawstate
{
	bool dirty;
	bool BG0;
	bool BG1;
	bool BG2;
	bool BG3;
	bool Doors;
	bool Sprites;
};
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
	MetroidFusion* fusionInstance;
	ZeroMission* zmInstance;
	WarioLand* wlInstance;
	RoomClass* mainRoom;
	SupportedTitles thisTitle;
	cOAMManager*mgrOAM;
	OamFrameTable* frameTables;
	int DisplayDoors(unsigned char Room);
	drawstate DrawStatus;
	int DrawBackGround(Image* img, TileBuffer* bimage);
	int LoadAreaTable();
	void LoadModifiers(char* fn);
	void LoadRoom(int Area, int Offset, Image* Tileset, TileBuffer* SpriteImage, int spriteset = 0);
	void LoadRoom(int Area, int Offset, Image* Tileset, TileBuffer* SpriteImage, unsigned long offset, FILE* fp);
	void DrawSprites(Image* pic);
	int DrawLayer(nMapBuffer* Map, Image* pic, unsigned char ctype);
	int DrawRoom(TileBuffer* TileImage, TileBuffer* BGImage, bool DrawBackLayer, bool DrawLevelLayer, 
		bool DrawForeground, bool HideSprites, bool ShowScrolls, bool ShowClip, int ScrollIndex);
	void DumpAreaAsImage(char* fn, Image* Tileset, TileBuffer* SpriteImage);
	void DrawDoorIndicators(HDC g);
	vector<long> RoomOffsets;
	DoorManager * mgrDoors;
	clsRoomScrolls * mgrScrolls;
	RD1Engine(SupportedTitles theTitle, OamFrameTable* frametables,TileBuffer * bg, TileBuffer* TileImage, Image* Tileset);
	int GetPalSize(int sprID);
	BackBuffer ThisBackBuffer;
	int currentRomType;
	sSpritev idkVRAM;
	void GetArrays();
	~RD1Engine();
};
