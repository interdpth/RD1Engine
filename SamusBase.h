#include <stdio.h>
#include <Windows.h>
#pragma once



class SamusBase
{
public:
	void*  DMA3SAD;
	void*  DMA3DAD;
	unsigned char  Sprite_tiles[32 * 4096];
	unsigned long DMA3CNT_L;
	unsigned short   Suit_color[512];
	unsigned long   OAMPal;
	int SamusGFXTophalfTileLen;
	int SamusGFXBottomHalfLen;
	unsigned long* SamusGFXTopHalfOffset;
	unsigned long* SamusGFXBottomOffset;
    
	unsigned long*SamusGFXLegsTopOffset;
	int SamusGFXLegsTopLen;

    int SamusGFXLegsBottomLen;
	unsigned long* SamusGFXLegsBottomOffset;

	int CannonTopHalfGFXLen;
	unsigned long*  CannonTopHalfGFXOffset;
	int CannonBottomHalfGFXLen;
	unsigned long*  CannonBottomHalfGFXOffset;

	unsigned long* SamusOAMPointer;
	int SuitPalLen;

	int gfxsize;
	unsigned long* GfxTablePnt;
	SamusBase();
	~SamusBase();
	virtual void Logic(){}
	void Load()
	{
		if (SamusGFXTophalfTileLen)
		{
			DMA3SAD = SamusGFXTopHalfOffset;
			DMA3DAD = Sprite_tiles;
			DMA3CNT_L = (SamusGFXTophalfTileLen >> 1);
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		FILE * testme = fopen("hey.gfx", "w+b");
		if (testme)
		{
			fwrite(Sprite_tiles, 1, 32 * 4096, testme);
			fclose(testme);
		}
		if (SamusGFXBottomHalfLen)
		{
			DMA3SAD = SamusGFXBottomOffset;
			DMA3DAD = &Sprite_tiles[256];
			DMA3CNT_L = (SamusGFXBottomHalfLen >> 1);
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		testme = fopen("hey.gfx", "w+b");
		if (testme)
		{
			fwrite(Sprite_tiles, 1, 32 * 4096, testme);
			fclose(testme);
		}
		if (SamusGFXLegsTopLen)
		{
			DMA3SAD = SamusGFXLegsTopOffset;
			DMA3DAD = &Sprite_tiles[160];
			DMA3CNT_L = (SamusGFXLegsTopLen >> 1);

			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		testme = fopen("hey.gfx", "w+b");
		if (testme)
		{
			fwrite(Sprite_tiles, 1, 32 * 4096, testme);
			fclose(testme);
		}
		if (SamusGFXLegsBottomLen)
		{
			DMA3SAD = SamusGFXLegsBottomOffset;
			DMA3DAD = &Sprite_tiles[416];
			DMA3CNT_L = (SamusGFXLegsBottomLen >> 1);
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		testme = fopen("hey.gfx", "w+b");
		if (testme)
		{
			fwrite(Sprite_tiles, 1, 32 * 4096, testme);
			fclose(testme);
		}
		if (CannonTopHalfGFXLen)
		{
			DMA3SAD = CannonTopHalfGFXOffset;
			DMA3DAD = &Sprite_tiles[512];
			DMA3CNT_L = (CannonTopHalfGFXLen >> 1);
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
			DMA3SAD = CannonBottomHalfGFXOffset;
			DMA3DAD = &Sprite_tiles[768];
			DMA3CNT_L = (CannonBottomHalfGFXLen >> 1);
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
		}
		testme = fopen("hey.gfx", "w+b");
		if (testme)
		{
			fwrite(Sprite_tiles, 1, 32 * 4096, testme);
			fclose(testme);
		}
		if (SuitPalLen)
		{
	/*		DMA3SAD = Suit_colour;
			DMA3DAD = &OAMPal;
			DMA3CNT_L = (SuitPalLen >> 1) | 0x80000000;
			memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);*/
		}
	}
};

