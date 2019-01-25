#pragma once
#include "GameConfiguration.h"
#include "MemFile.h"
#include "Structures.h"
#include "RD1Title.h"
class ZeroMission
{
public:
	ZeroMission();
	~ZeroMission();
	int GetPalSize(int sprID);
	const char* PoseFile = "ZeroMissionSpritePositions.txt";
	static const char* CodeName;
};

