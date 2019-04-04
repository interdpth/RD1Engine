#include "MetroidFusion.h"
#include "MemFile.h"

const char* MetroidFusion::CodeName = "AMTE";

void MetroidFusion::LoadGameData()
{
	DataContainer* sprSize = GameConfiguration::mainCFG->GetDataContainer("SpriteSizes");
	MemFile::currentFile->seek(sprSize->Value);
	MemFile::currentFile->fread(&MFSprSize, 2, 0x180);
}

int MetroidFusion::GetPalSize(int sprID)
{
	return ((MFSprSize[(sprID - 0x10) << 1] / 2048) * 16);
}

void MetroidFusion::GetGFX(int sprID, unsigned char* gfxBuffer)
{
	SeekSpriteGFX(GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value, sprID);
	int size = this->MFSprSize[(sprID - 0x10) << 1];

	MemFile::currentFile->fread(gfxBuffer, 1, size);
}

MetroidFusion::MetroidFusion(GBAMethods* g, MemFile* theGame) :BaseTitle(g, theGame, (char*)MetroidFusion::CodeName)
{

}

const char* MetroidFusion::GetPoseFile()
{
	return this->PoseFile;
}

MetroidFusion::~MetroidFusion()
{

}


