#pragma once
#include "BaseGame.h"
class WarioLand :public BaseTitle
{
public:
	WarioLand(GBAMethods* g, MemFile* theGame);
	~WarioLand();


	const char* PoseFile = "WarioLandSpritePositions.txt";
	const char* GetPoseFile() override;
	static const char* CodeName;
};

