#include "BaseTitle.h"



BaseTitle::BaseTitle(GBAMethods* g, MemFile* thisTitle, char* titlename)
{
	_thisTitle = thisTitle;
	_codeName = titlename;
	GBA = g;
}

void BaseTitle::GetBaseTilesetGFX(gTileData* tileset)
{

}
void BaseTitle::SeekSpriteTable(int spriteID)
{
	unsigned long off = 0;
	int val = GameConfiguration::mainCFG->GetDataContainer("SpriteSetTable")->Value;
	_thisTitle->seek((spriteID * 4) + val);
	_thisTitle->fread(&off, sizeof(long), 1);
	_thisTitle->seek(off - 0x8000000);

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
int BaseTitle::GetRoomCount(int area)
{
	return GameConfiguration::mainCFG->GetDataContainer("RoomsPerArea")->DataArray[area];
}
vector<string>* BaseTitle::GetAreaNames()
{
	DataContainer* areaNames = GameConfiguration::mainCFG->GetDataContainer("AreaNames");
	return &areaNames->stringArray;
}

void BaseTitle::SeekPointer(unsigned long pointer)
{
	unsigned long addybuf = 0;
	_thisTitle->seek(pointer);
	_thisTitle->fread(&addybuf, 4, 1);
	_thisTitle->seek(addybuf - 0x8000000);
}
int BaseTitle::GetGFX(int sprID, unsigned char* buffer) {
	return 0;
}
int BaseTitle::GetPalSize(int sprID) {
	return -1;
}

const char* BaseTitle::GetPoseFile()
{
	return "";
}
