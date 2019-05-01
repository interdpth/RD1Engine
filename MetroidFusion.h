#pragma once
#include "GameConfiguration.h"
#include <stdio.h>
#include "BaseTitle.h"
class MetroidFusion:public BaseTitle 
{
public:
	unsigned short MFSprSize[0x180];
	void LoadGameData();
	int GetPalSize(int sprID) override;
	MetroidFusion(GBAMethods* g, MemFile* theGame);
	~MetroidFusion();
	int GetGFX(int sprID, unsigned char* gfxBuffer) override;
	const char* GetPoseFile() override;
	const char* PoseFile = "fusionSpritePositions.txt";
	static const char* CodeName;
};

