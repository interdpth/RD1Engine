#include "ZeroMission.h"

const char* ZeroMission::CodeName = "BMXE";
int ZeroMission::GetPalSize(int sprID)
{
	MemFile* rom = MemFile::currentFile;
	CompHeader thiscompheader;
	unsigned long off;
	rom->seek(GameConfiguration::mainCFG->GetDataContainer("SpritePalSizes")->Value + (sprID - 0x10) * 4);
	rom->fread(&off, 4, 1);
	rom->seek(off - 0x8000000);
	rom->fread(&thiscompheader.check_ten, 1, 1);       //Check byte should be 0x10 for lz
	rom->fread(&thiscompheader.l3, 1, 1);
	rom->fread(&thiscompheader.l2, 1, 1);
	rom->fread(&thiscompheader.l1, 1, 1);

	return  (thiscompheader.l2 & 0xF8) * 2;

}
ZeroMission::ZeroMission()
{

}


ZeroMission::~ZeroMission()
{

}
