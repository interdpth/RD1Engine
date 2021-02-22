#pragma once
#include "Structures.h"
#include <iostream>
#include "OAM.h"
#include "SpriteObjectManager.h"
#include "SprGBuf.h"
#include "FrameManager.h"
#include "Frames.h"
#include "GBAMethods.h"
#include <vector>
class cOAMManager
{
public:
	static int CalcSpriteBounds(SpriteObject* SpriteDetails);
	cOAMManager(std::map<int, std::vector<unsigned long>>* _oAMFrameTable, GBAMethods* gba, unsigned short* Size, int crf);
	~cOAMManager();
	GBAMethods* _gbaMethods;
	unsigned short*  MFSprSize;
	const unsigned char objSizes[3][4][2] =
	{
		{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
		{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
		{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	};
	
	std::map<int, std::vector<unsigned long>>* OAMFrameTable;
	sprite_entry roomSpriteIds[32];
	unsigned short oamPiece[17];
	static void CalcPartSizes(SpriteObject* SpriteDetails);
	static int DrawPSprite(SpriteObject* SpriteDetails );
	static unsigned char maxsprite;
	static char* GetSpriteSize(int a, int b);
	int LoadRoomOAM();
	int DrawOAM();
	int SaveSprite(SaveOptions savetype, SpriteObject* tSprite, unsigned long Offset);
	static int DecodeOAM(bool OAMED, SpriteObject* tSprite, unsigned long Offset);
	static int LoadSpriteToMem(bool romSwitch,GBAMethods* gba, GFXData* ginfo, sprite_entry* spriteset, unsigned char* GraphicsBuffer, TileBuffer* tb);
	int currentRomType;
	static int SetupPreview(GBAMethods* methods, int TitleChoice, Frame* targetFrame);
};

