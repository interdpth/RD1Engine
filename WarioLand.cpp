#include "WarioLand.h"

const char* WarioLand::CodeName = "AWAE";

WarioLand::WarioLand(GBAMethods* g, MemFile* theGame) :BaseTitle(g, theGame, (char*)WarioLand::CodeName)
{
}

const char* WarioLand::GetPoseFile() 
{ 
	return this->PoseFile; 
}

WarioLand::~WarioLand()
{
}
