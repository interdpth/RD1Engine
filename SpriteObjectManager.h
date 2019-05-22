#pragma once
#include "Structures.h"
#include "cOAMManager.h"
#include <stdio.h>
//#include "GlobalVars.h"
#include "GBAGraphics.h"
#include <Windows.h>
#include "Image.h"
#include "backbuffer.h"
#include "FrameManager.h"
#include "GBAMethods.h"
using namespace std;
class FrameManager;
class SpriteObjectManager
{
private:
	FrameManager* _currentFrames; 
	std::map<int, std::vector<unsigned long>>* _OAMFrameTable;
	GBAMethods* _gbaMethods;
	SpritesetData* SpriteSet;
public:

	SpriteObjectManager(SpritesetData* spriteset, GBAMethods* gba, std::map<int, std::vector<unsigned long>>* OAMFrameTable,FrameManager* currentFrames, int ObjCount=3);
	~SpriteObjectManager();
	
	
    unsigned char compare;
	vector<GFXData> gfxpnt_dst;  //0 = pointer to gfx, 1 = destination in VRAM
	PalData paltransfer[16]; //0 = pointer to pal, 1 = destination, 2 = WORD count (*2 for bytes)
	RECT OverallSize[16];
	vector<FrameManager*> RoomSprites; 
	CompHeader thiscompheader;
	vector<nEnemyList> SpriteObjects;
	void AddSpriteObject(int ObjectSet);
	void DeleteSpriteObject(int ObjectSet, int ObjID);
	int LoadEnemies(RHeader* roomHeader);
	int ShowSprites(bool show, unsigned char Number, BackBuffer* drawBuffer);
	int SaveSprites(RHeader* roomHeader);
	int ExportPal();
	int ImportPal(int TitleChoice);
	int ExportGFX();
	int ImportGFX();
	int SavePal(int RomSwitch);
	int SaveGFX(int RomSwitch);
	int SavePal(PalData* palinfo, sprite_entry* spriteset, long *Palette);

	int GetZMSetSZ(long* GFXSizes, long*PalSizes, sprite_entry* SpriteInfo);

	int GetMFSetSZ(long* GFXSizes, long*PalSizes, sprite_entry* SpriteInfo);
};

