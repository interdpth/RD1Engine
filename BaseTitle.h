#pragma once
#include "MemFile.h"
#include "GBAMethods.h"

class BaseTitle
{
private:
    char* _codeName;
	MemFile* _thisTitle;
public:
	GBAMethods* GBA;
	BaseTitle(GBAMethods* g, MemFile* thisTitle, char* titlename);
	void SeekSpriteGFX(unsigned long tableOffset, int id);
	virtual void GetGFX(int sprID, unsigned char* buffer);
	virtual int GetPalSize(int sprID);
	virtual const char* GetPoseFile();
	~BaseTitle();
};

