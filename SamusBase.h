#include <stdio.h>
#include <Windows.h>
#include "MemFile.h"
#include "SprGBuf.h"
#include "FrameManager.h"
#pragma once

/* 112 */
struct OAMMemoryEntry
{
	__int16 OAM0;
	__int16 OAM1;
	__int16 OAM2;
	__int16 OAMRot;
};
/* 71 */
struct RawOAM
{
	__int16 OAM1;
	__int16 OAM2;
	__int16 OAM3;
};

/* 113 */
struct SamusOamData
{
	char startOAMIndex;
	char field_1;
	char OAMStart;
	char field_3;
	char field_4;
	char field_5;
	char field_6;
	char field_7;
	char field_8;
	char field_9;
	char field_A;
	char field_B;
	char field_C;
	char field_D;
	char field_E;
	char field_F;
	char field_10;
	char field_11;
};

class SamusBase
{
public:
	void*  DMA3SAD;
	void*  DMA3DAD;
	unsigned char  Sprite_tiles[0x8000];
	unsigned long DMA3CNT_L;
	unsigned short   Suit_color[512];
	unsigned long   OAMPal;
	int SamusGFXTophalfTileLen;
	int SamusGFXBottomHalfLen;
	unsigned long SamusGFXTopHalfOffset;
	unsigned long SamusGFXBottomOffset;
    
	unsigned long SamusGFXLegsTopOffset;
	int SamusGFXLegsTopLen;

    int SamusGFXLegsBottomLen;
	unsigned long SamusGFXLegsBottomOffset;

	int CannonTopHalfGFXLen;
	unsigned long  CannonTopHalfGFXOffset;
	int CannonBottomHalfGFXLen;
	unsigned long  CannonBottomHalfGFXOffset;

	unsigned long SamusOAMPointer;
	int SuitPalLen;

	int gfxsize;
	unsigned long GfxTablePnt;
	SamusBase();
	~SamusBase();

	SprGBuf* theSprite;
	virtual void Logic(){}
	void Load()
	{
		memset(Sprite_tiles, 0, 0x8000);
		if (SamusGFXTophalfTileLen)
		{
			DMA3CNT_L = (SamusGFXTophalfTileLen);
			
			DMA3SAD = &MemFile::currentFile->GetFile()[SamusGFXTopHalfOffset];
			
			DMA3DAD = Sprite_tiles;

			memcpy(DMA3DAD, DMA3SAD,DMA3CNT_L);
			
		}
	
		if (SamusGFXBottomHalfLen)
		{
		
			DMA3SAD = &MemFile::currentFile->GetFile()[SamusGFXBottomOffset];
			DMA3DAD = &Sprite_tiles[0x400];
			DMA3CNT_L = (SamusGFXBottomHalfLen);
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		
		if (SamusGFXLegsTopLen)
		{
			DMA3SAD = &MemFile::currentFile->GetFile()[SamusGFXLegsTopOffset];
			DMA3DAD = &Sprite_tiles[0x280];
			DMA3CNT_L = (SamusGFXLegsTopLen);

			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
	
		if (SamusGFXLegsBottomLen)
		{
			DMA3SAD = &MemFile::currentFile->GetFile()[SamusGFXLegsBottomOffset];
			DMA3DAD = &Sprite_tiles[0x680];
			DMA3CNT_L = (SamusGFXLegsBottomLen);
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		
	/*	if (CannonTopHalfGFXLen)
		{
			DMA3SAD = &MemFile::currentFile->GetFile()[CannonTopHalfGFXOffset];
			DMA3DAD = &Sprite_tiles[512];
			DMA3CNT_L = (CannonTopHalfGFXLen >> 2);
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		testme = fopen("hey.gfx", "w+b");
		if (testme)
		{
			fwrite(Sprite_tiles, 1, 32 * 4096, testme);
			fclose(testme);
		}
		if (CannonBottomHalfGFXLen)
		{
			DMA3SAD = &MemFile::currentFile->GetFile()[CannonBottomHalfGFXOffset];
			DMA3DAD = &Sprite_tiles[768];
			DMA3CNT_L = (CannonBottomHalfGFXLen >> 2);
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		testme = fopen("hey.gfx", "w+b");
		if (testme)
		{
			fwrite(Sprite_tiles, 1, 32 * 4096, testme);
			fclose(testme);
		}
		*/if (SuitPalLen)
		{
	/*		DMA3SAD = Suit_colour;
			DMA3DAD = &OAMPal;
			DMA3CNT_L = (SuitPalLen >> 1) | 0x80000000;
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);*/
		}
	}
	void PackSamus()
	{
		theSprite = new SprGBuf();
		
		unsigned int sloutCounter; // r7
		unsigned int OamCounter; // r6
		
		unsigned int HorizMiddleOfSamus=0; // r8
		unsigned int BottomOfSamus; // r12
	
		unsigned int nextDrawSlot; // r7
unsigned short v3 = 0;
		
			
			int Next_OAM_slot = 0;
		
			sloutCounter = Next_OAM_slot;
			OamCounter = Next_OAM_slot;
			v3 = 0;
			HorizMiddleOfSamus = 0;
			BottomOfSamus = 2;
			int startOAMIndex=0;

			MemFile::currentFile->seek(SamusOAMPointer-0x8000000);

			MemFile::currentFile->fread(&startOAMIndex, 1, 1);

			theSprite->maxparts = startOAMIndex;
			nextDrawSlot = (startOAMIndex) & 0xFF;
			MemFile::currentFile->seek(SamusOAMPointer - 0x8000000 + 2);
			
			for (int count =0; count < nextDrawSlot; count++) 
			{
				OverAllOAM thisOAM;
				memset(&thisOAM, 0, sizeof(OverAllOAM));
				DecodedOAM* decodedOam = &thisOAM.deOAM;

				MemFile::currentFile->fread(&thisOAM.enOAM.OAM0, 2, 1);
				MemFile::currentFile->fread(&thisOAM.enOAM.OAM1, 2, 1);
				MemFile::currentFile->fread(&thisOAM.enOAM.OAM2, 2, 1);

				//LOBYTE(OAMMemoryPointer->OAM0) = currOAM0 + BottomOfSamus; --v
				thisOAM.enOAM.OAM0 += 2;
	//			thisOAM.enOAM.OAM1 = thisOAM.enOAM.OAM1 & 0xFE00 | (thisOAM.enOAM.OAM1 + HorizMiddleOfSamus) & 0x1FF;
	///*			thisOAM.enOAM.OAM2 = thisOAM.enOAM.OAM2 & 0xFE00 | (thisOAM.enOAM.OAM1 ) & 0x1FF;
	//		*/	
	//			thisOAM.enOAM.OAM2 = ((thisOAM.enOAM.OAM2&0xFF00) & 0xF300) | 4 * 0 | (thisOAM.enOAM.OAM2 & 0xFF);
	//			

				FrameManager::UnpackOAM(&thisOAM.enOAM, &thisOAM.deOAM);

				theSprite->OAM.push_back(thisOAM);

			}
			Next_OAM_slot = nextDrawSlot;

	}

};

