#pragma once
#include "MemFile.h"
#include "GBAMethods.h"
#include "GameConfiguration.h"
#include "structures.h"
class BaseTitle
{
private:
    char* _codeName;
	MemFile* _thisTitle;
public:
	GBAMethods* GBA;
	BaseTitle(GBAMethods* g, MemFile* thisTitle, char* titlename);
	void SeekSpriteGFX(unsigned long tableOffset, int id);
	void SeekSpriteTable(int spriteID);
	void GetBaseTilesetGFX(gTileData* tileset);
	void SeekPointer(unsigned long pointer);
	virtual int GetGFX(int sprID, unsigned char* buffer);
	virtual int GetPalSize(int sprID);
	virtual const char* GetPoseFile();
	vector<string>* GetAreaNames();
	int GetRoomCount(int area);
	~BaseTitle();
};

