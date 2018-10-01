#include "MetroidFusion.h"
#include "MemFile.h"

const char* MetroidFusion::CodeName = "AMTE";

void MetroidFusion::LoadGameData(FILE* fp)
{
	DataContainer* sprSize = GameConfiguration::mainCFG->GetDataContainer("SpriteSizes");
	MemFile::currentFile->seek(sprSize->Value);
	MemFile::currentFile->fread(&MFSprSize, 2, 0x180, fp);
}

int MetroidFusion::GetPalSize(int sprID)
{
	return ((MFSprSize[(sprID - 0x10) << 1] / 2048) * 16);
}
MetroidFusion::MetroidFusion()
{

}



MetroidFusion::~MetroidFusion()
{

}


