#pragma once
#include "Structures.h"
#include <iostream>
#include "OAM.h"
#include "SpriteObjectManager.h"
#include "SprGBuf.h"
#include "FrameManager.h"
#include "Frames.h"
#include <vector>
class cOAMManager
{
public:
	static int CalcSpriteBounds(SprGBuf* SpriteDetails);
	cOAMManager();
	~cOAMManager();
	unsigned short*  MFSprSize;
	const unsigned char objSizes[3][4][2] =
	{
		{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
		{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
		{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	};

	const char* cOAMManager::sizesStr[3][4] = {
		{ "8, 8\0", "16,16\0","32,32\0", "64,64\0" },
		{ "16,8\0", "32,8\0" ,"32,16\0", "64,32\0" },
		{ "8,16\0", "8,32\0" ,"16,32\0", "32,64\0" },
	};

	sprite_entry roomSpriteIds[32];
	unsigned short oamPiece[17];
	OverAllOAM thisoam[0x100][512];
	int Create();
	int UpdateSize();
	int GetFrames(FILE* ROM);
	int SetFrames();
	int SetParts();
	int SetupPreview(FILE* ROM, int TitleChoice);
	static void CalcPartSizes(SprGBuf* SpriteDetails);
	static int DrawPSprite(SprGBuf* SpriteDetails );
	static unsigned char maxsprite;
	int AnimateSprite();
	int GetSpriteData();
	int ChangeFrame();
	static char* GetSpriteSize(int a, int b);
	int LoadRoomOAM();
	int DrawOAM();
	int SaveSprite(FILE* ROM, SaveOptions savetype, SprGBuf* tSprite, unsigned long Offset);
	static int DecodeOAM(FILE* ROM, bool OAMED, SprGBuf* tSprite, unsigned long Offset);
	static int LoadSpriteToMem(bool romSwitch, GFXData* ginfo, sprite_entry* spriteset, unsigned char* GraphicsBuffer, TileBuffer* tb);
	static int GetBitField(long reg, long bitStart, long bitLength) {
		return	(reg / (1 << bitStart)) & ((1 << bitLength) - 1);
	}
};

