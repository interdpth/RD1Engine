#pragma once
#include "Structures.h"
#include "OAM.h"
#include "clsPreviewSprite.h"
#include "image.h"
class SprGBuf {
public: 
	SprGBuf(unsigned char* gfxRam, long* pal);
	SprGBuf();
	~SprGBuf();
	void Create(unsigned char* gfxRam, long* pal);
	bool selfInitPal;
	bool selfInitGFX;
	int id;
	bool created;
	RECT  Borders;
	clsPreviewSprite PreviewSprite;
	long*  PreviewPal;// [512];
	unsigned char*  PreRAM;// [0x8000];//Funny name XD
	unsigned short PartNo;            
	std::vector<OverAllOAM>  OAM;
	TileBuffer* Tiles;
	int maxparts;
	unsigned char details;
	unsigned short palsize;
	unsigned long graphicsize;
};

