#pragma once
#include "Structures.h"
#include "GBAGraphics.h"
#include "GBAMethods.h"
#include <vector>
using namespace std;
class cEntityManager
{
public:
	cEntityManager(GBAMethods *methods);
	~cEntityManager();
	GBAMethods* _gbaMethods;
	GFXData *gfxinfo;// [16];
	PalData *palinfo;// [16];
	sprite_entry *spriteset;// [16];
	int LoadPal(PalData* palinfo, sprite_entry* spriteset, long *Palette);
	int LoadSet(bool ReadObjectDetailsFromROM, GFXData* spritedata, PalData*Palettedata, sprite_entry* SpriteInfo, unsigned char SpriteSetNo);
	int MFLoadSet(bool ReadObjectDetailsFromROM, GFXData* spritedata, PalData*Palettedata, sprite_entry* SpriteInfo, unsigned char SpriteSetNo);
};

