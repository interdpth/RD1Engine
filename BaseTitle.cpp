#include "BaseTitle.h"



BaseTitle::BaseTitle(GBAMethods* g, MemFile* thisTitle, char* titlename)
{
	_thisTitle = thisTitle;
	_codeName = titlename;
	GBA = g;
}

void BaseTitle::SeekSpriteGFX(unsigned long tableOffset, int id)
{
	unsigned long addybuf = 0;
	unsigned long GFXPnt = tableOffset + (id - 0x10) * 4;//GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (id- 0x10) * 4;
	_thisTitle->seek(GFXPnt);
	_thisTitle->fread(&addybuf, 4, 1);
	_thisTitle->seek(addybuf - 0x8000000);
}
BaseTitle::~BaseTitle()
{
}




void BaseTitle::GetGFX(int sprID, unsigned char* buffer) {
	return;
}
int BaseTitle::GetPalSize(int sprID) {
	return -1;
}

const char* BaseTitle::GetPoseFile()
{
	return "";
}
