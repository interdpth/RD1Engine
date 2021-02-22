#pragma once

#include <stdio.h>
//#include "globals.h"
#include "TilesetManager.h"
#include "DoorManager.h"
#include "cEnitityManager.h"
#include "MapManager.h"
#include "SpriteObjectManager.h"
#include "BaseGame.h"
#include "FrameManager.h"
#include "IObject.h"
#include <vector>

using namespace std;
class SpriteObjectManager;
class cEntityManager;
class FrameManager;
class RoomClass
{
private:
	GBAMethods* _gbaMethods;
public:
//	SpriteObjectManager* RD1Engine::theGame->mainRoom->mgrSpriteObjects;
	//RHeader RoomHeader;
	int Area;
	int Room;
	int currentRomType;
	int currentHorizScroll;
	int currentVertScroll;
	RoomClass(int romType, Image* tilsetsrc, SpritesetData* spriteset, GBAMethods* gba, std::map<int, std::vector<unsigned long>>* OAMFrameTable, FrameManager* currentFrames, int area, int room, unsigned long offset);
	RoomClass(int romType, Image* tilsetsrc, SpritesetData* spriteset, GBAMethods* gba, std::map<int, std::vector<unsigned long>>* OAMFrameTable, FrameManager* currentFrames);
	RoomClass(int romType, Image* tilsetsrc, SpritesetData* spriteset, GBAMethods* gba, std::map<int, std::vector<unsigned long>>* OAMFrameTable, FrameManager* currentFrames, int area, int room, RHeader* offset);

	void Setup(Image* tilesetsrc, int area, int room);
	~RoomClass();
	MapManager * mapMgr;
	//editingStates, pointer to objects
	map<int, vector<RD1Object*>*> MapObjects;
	SpriteObjectManager* mgrSpriteObjects;
	cEntityManager* mgrEntities;
	RHeader* roomHeader;
	RHeader copyHeader;
	unsigned originalOffset;
	int LoadData();
	void LoadUpSprites(int SpriteSetIndex, TileBuffer *     SpriteImage);
	int LoadHeader(long Offset);
	void Load(Image* tileset, int area, int room, unsigned long offset);
	void Load(Image* tileset, int area, int room, RHeader* offset);
	int GetLayerData(unsigned char compression, unsigned char Layer, unsigned long offset);
	void Export(char* name);
	void Import(char* name);
	void SaveHeader(unsigned long offset);
	RD1Object* GetRD1Object(editingStates type, int index);
	RD1Object* GetRD1Object(editingStates type, int x, int y);
	bool lockRoom;
	bool lockSprites;
};

