#pragma once
#include "GameConfiguration.h"
#include "MemFile.h"
#include "Structures.h"
#include "RD1Title.h"
#include "BaseTitle.h"
class ZeroMission:public BaseTitle
{
public:
	ZeroMission(GBAMethods* g, MemFile* theGame);
	~ZeroMission();
	int GetPalSize(int sprID) override;
	int GetGFX(int sprID, unsigned char* buffer) override;

	const char* PoseFile = "ZeroMissionSpritePositions.txt";
	const char* GetPoseFile() override;
	static const char* CodeName;
};

