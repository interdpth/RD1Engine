#include "ZeroMission.h"

const char* ZeroMission::CodeName = "BMXE";

ZeroMission::ZeroMission(GBAMethods* g, MemFile* theGame) :BaseTitle(g, theGame, (char*)ZeroMission::CodeName) 
{

}


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
//&SprG->PreRAM[0x4000]
void ZeroMission::GetGFX(int sprID,unsigned char* buffer)
{
	unsigned char* compBuffer = new unsigned char[32687];
	unsigned char* decompbuf = new unsigned char[64691];
	SeekSpriteGFX(GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value, sprID);

	MemFile::currentFile->fread(compBuffer, 1, 32687);
	int size = GBA->LZ77UnComp(compBuffer, decompbuf);
	memcpy(buffer, &decompbuf, size);
	delete[] compBuffer;
	delete[] decompbuf;
}
const char* ZeroMission::GetPoseFile()
{
	return this->PoseFile;
}
ZeroMission::~ZeroMission()
{

}
