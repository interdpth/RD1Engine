#include "SprGBuf.h"

SpriteObject::SpriteObject()
{
	this->PreviewPal=0;// [512];
	this->PreRAM = 0;// [0x8000];//Funny name XD
	this->PartNo=0;
	Tiles = NULL;
	memset(&Borders, 0, sizeof(Borders));
	this->Create(NULL, NULL);
	
//	PreviewSprite->_fullCache = NULL;
	
}
SpriteObject::SpriteObject(unsigned char* gfxRam, long* pal)
{
	
	this->PreviewPal = pal;// [512];
	this->PreRAM = gfxRam;// [0x8000];//Funny name XD
	this->PartNo = 0;
	Tiles = NULL;
	memset(&Borders, 0, sizeof(Borders));
	this->Create(gfxRam, pal);
}
///Constru
void SpriteObject::Create(unsigned char* gfxRam, long* pal)
{
	Tiles = new TileBuffer();
	if (gfxRam == NULL)
	{
		this->PreRAM = new unsigned char[0x8032];
		selfInitGFX = true;
	}
	else {
		this->PreRAM = gfxRam;
		selfInitGFX = false;
	}

	if (pal == NULL)
	{
		this->PreviewPal = new long[512];
		selfInitPal = true;
	}
	else
	{
		selfInitPal = false;
		this->PreviewPal = pal;
	}
	if (selfInitGFX != selfInitPal)
	{
		Logger::log->LogIt(Logger::LOGTYPE::ERRORZ, "SELF ININIT IS NOT CORRECT");
	}
	id=0;
	memset(&Borders,0,sizeof(RECT));
	//if (pal == NULL)
	//{
	//	memset(this->PreviewPal, 0, 2048);
	//}
	//if (gfxRam == NULL)
	//{
	//	memset(this->PreRAM, 0, 0x8000);//Funny name XD
	//}
	PartNo=0;    
	//PreviewSprite->Create(1, 1);
	OAM.clear();
	
	maxparts=0;
	details = 0;
	palsize = 0;
	graphicsize = 0;
}


SpriteObject::~SpriteObject()
{
	if (selfInitGFX)
	{
		delete[] PreRAM;
	}
	if (selfInitGFX)
	{
		delete[] PreviewPal;
	}
	if (Tiles)
	{
		delete Tiles;
	}
}
