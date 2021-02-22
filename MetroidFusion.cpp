#include "MetroidFusion.h"
#include "..\MemFile\MemFile.h"
#include "GBAGraphics.h"
const char* MetroidFusion::CodeName = "AMTE";

void MetroidFusion::LoadGameData()
{
	DataContainer* sprSize = GameConfiguration::mainCFG->GetDataContainer("SpriteSizes");
	MemFile::currentFile->seek(sprSize->Value);
	MemFile::currentFile->fread(&this->MFSprSize[0], 2, 0x180);
	MemFile::currentFile->seek(0x3e419c);
	MemFile::currentFile->fread(&GBAGraphics::VRAM->SprVRAM[0x900], 1, 0x36E0);
	MemFile::currentFile->seek(0x58b466);
	MemFile::currentFile->fread(&GBAGraphics::VRAM->GBASprPal[0x40], 1, 0x14);
	MemFile::currentFile->seek(0x3E40F2);
	MemFile::currentFile->fread(&GBAGraphics::VRAM->GBASprPal[0x56], 1, 0x3E);
}

int MetroidFusion::GetPalSize(int sprID)
{
	return ((MFSprSize[(sprID - 0x10) << 1] / 2048) * 16);
}

int MetroidFusion::GetGFX(int sprID, unsigned char* gfxBuffer)
{
	SeekSpriteGFX(GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value, sprID);
	int size = this->MFSprSize[(sprID - 0x10) << 1];

	MemFile::currentFile->fread(gfxBuffer, 1, size);
	return size;
}

MetroidFusion::MetroidFusion(GBAMethods* g, MemFile* theGame) :BaseTitle(g, theGame, (char*)MetroidFusion::CodeName)
{
	/*memset(MFSprSize, 0,2 * 0x180);*/
}

const char* MetroidFusion::GetPoseFile()
{
	return this->PoseFile;
}



MetroidFusion::~MetroidFusion()
{

}


