#pragma once
#include "GameConfiguration.h"
#include <stdio.h>
class MetroidFusion 
{
public:
	unsigned short MFSprSize[0x180];
	void LoadGameData(FILE* fp);
	int GetPalSize(int sprID);
	MetroidFusion();
	~MetroidFusion();

	const char* PoseFile = "fusionSpritePositions.txt";
	static const char* CodeName;
};

