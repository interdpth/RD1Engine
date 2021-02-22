#pragma once
#include "Structures.h"
#include "OAM.h"
#include "clsPreviewSprite.h"
#include "..\Win32GBAImageClasses\image.h"
class SpriteObject {
public: 
	SpriteObject(unsigned char* gfxRam, long* pal);
	SpriteObject();
	~SpriteObject();
	void Create(unsigned char* gfxRam, long* pal);
	bool IsEdited;
	bool selfInitPal;
	bool selfInitGFX;
	int id;
	bool created;
	RECT  Borders;
	clsPreviewSprite PreviewSprite;
	long*  PreviewPal;
	unsigned char*  PreRAM;
	unsigned short PartNo;            
	std::vector<OverAllOAM>  OAM;
	TileBuffer* sprTileBuffer;
	int maxparts;
	unsigned char details;
	unsigned short palsize;
	unsigned long graphicsize;
};

