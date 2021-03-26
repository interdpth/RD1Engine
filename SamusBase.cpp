#include "SamusBase.h"



SamusBase::SamusBase()
{
	theSprite = new SpriteObject();
}


SamusBase::~SamusBase()
{
	delete SamusTop;
	delete SamusBottom;
	//delete  SamusCannonTop;
	//delete  SamusCannonBottom;
}

void SamusBase::Load()
{
	memset(Sprite_tiles, 0, 0x8000);
	if (SamusTop->topLen)
	{
		
		DMA3DAD = Sprite_tiles;

		memcpy(DMA3DAD, SamusTop->topGFX, SamusTop->topLen*32);
	}

	if (SamusTop->bottomLen)
	{
		DMA3DAD = &Sprite_tiles[0x400];	

		memcpy(DMA3DAD, SamusTop->bottomGFX, SamusTop->bottomLen * 32);
	}

	if (SamusBottom->topLen)
	{
		DMA3DAD = &Sprite_tiles[0x280];

		memcpy(DMA3DAD, SamusBottom->topGFX, SamusBottom->topLen * 32);
	}

	if (SamusBottom->bottomLen)
	{
		
		DMA3DAD = &Sprite_tiles[0x680];
	
		memcpy(DMA3DAD, SamusBottom->bottomGFX, SamusBottom->bottomLen * 32);
	}



	//if (SamusCannonBottom->topLen)
	//{
	//	DMA3DAD = &Sprite_tiles[512];

	//	memcpy(DMA3DAD, SamusBottom->topGFX, SamusBottom->topLen * 32);
	//}

	//if (SamusCannonBottom->bottomLen)
	//{

	//	DMA3DAD = &Sprite_tiles[768];

	//	memcpy(DMA3DAD, SamusBottom->bottomGFX, SamusBottom->bottomLen * 32);
	//}
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
void SamusBase::UnpackSamus()
{
	theSprite = new SpriteObject();

	unsigned int sloutCounter; // r7
	unsigned int OamCounter; // r6

	unsigned int HorizMiddleOfSamus = 0; // r8
	unsigned int Midscreensamus; // r12

	unsigned int nextDrawSlot; // r7
	unsigned short v3 = 0;
	int SamusFront = 0;
	int MiddleOFScreen = 0;
	int Next_OAM_slot = 0;
	bool PutSamusFront = false;
	int MidVertOfScreen = 0;
	sloutCounter = Next_OAM_slot;
	OamCounter = Next_OAM_slot;
	v3 = 0;
	HorizMiddleOfSamus = 0;
	Midscreensamus = 2;
	int startOAMIndex = 0;
	int MidScreenModifier = 0;
	MemFile::currentFile->seek(SamusOAMPointer - 0x8000000);
	int count = MemFile::currentFile->fgetc();
	MemFile::currentFile->seek(SamusOAMPointer - 0x8000000 + 2);
	SamusFront = 2;
	if (SamusPose == SamusDying)
	{
		SamusFront = 0;
		Next_OAM_slot = 0;
	}
	else if (PutSamusFront)
	{
		SamusFront = 1;
	}
	if (Next_OAM_slot > 0x6Au)
	{
		Next_OAM_slot = 0x6A;
	}

	int x = 256 / 8;
	int y = 512 / 8;


	/*
	
	if ( SamusPose > INMORPHBALL || SamusPose < Morphing )
  {
    if ( (SlopeType & 0x10) != 0 )
    {
      MiddleOFScreen = HorizMiddleOfSamus << 0x10;
      MidScreenModifier = 0x20000;
    }
    else
    {
      if ( (SlopeType & 0x20) == 0 )
      {
        goto LABEL_23;
      }
      MiddleOFScreen = HorizMiddleOfSamus << 0x10;
      MidScreenModifier = 0xFFFE0000;
    }
    HorizMiddleOfSamus = (MiddleOFScreen + MidScreenModifier) >> 0x10;
    MidVertOfScreen = (BottomOfSamus << 0x10) + 0x10000;
  }
  else
  {
    if ( !SlopeType )
    {
      goto LABEL_23;
    }
    MidVertOfScreen = (BottomOfSamus << 0x10) + 0x20000;
  }
  LOBYTE(BottomOfSamus) = BYTE2(MidVertOfScreen);
	
	
	*/

	//HorizMiddleOfSamus = ((theSprite->Borders.right - theSprite->Borders.left) / 2);
	//BottomOfSamus = ((theSprite->Borders.bottom - theSprite->Borders.top) / 2) & 0xFFFF;
	//Midscreensamus = ((((theSprite->Borders.bottom - theSprite->Borders.top) / 2)));

	HorizMiddleOfSamus = ((0 >> 2) - (x/2 >> 2));
	int BottomOfSamus = ((0 >> 2) - (y/2 >> 2) + 2);


	if (SamusPose > MorphBall || SamusPose < Morphing)
	{

		/*MiddleOFScreen = x / 2;


		HorizMiddleOfSamus = (MiddleOFScreen + MidScreenModifier);
		MidVertOfScreen = (y / 2);*/

		MiddleOFScreen = HorizMiddleOfSamus << 0x10;
		MidScreenModifier = 0xFFFE0000;
	
	HorizMiddleOfSamus = (MiddleOFScreen + MidScreenModifier) >> 0x10;
	MidVertOfScreen = (BottomOfSamus << 0x10) + 0x10000;


	}

	//Midscreensamus = MidVertOfScreen;
//
	//	OamCounter = (unsigned __int8)Next_OAM_slot;
	theSprite->OAM.clear();

	//for (unsigned int countn = 0; countn < count; countn++)
	//{
	//	OverAllOAM thisOAM;
	//	memset(&thisOAM, 0, sizeof(OverAllOAM));
	//	DecodedOAM* decodedOam = &thisOAM.deOAM;

	//	MemFile::currentFile->fread(&thisOAM.enOAM.OAM0, 2, 1);
	//	MemFile::currentFile->fread(&thisOAM.enOAM.OAM1, 2, 1);
	//	MemFile::currentFile->fread(&thisOAM.enOAM.OAM2, 2, 1);

		//thisOAM.enOAM.OAM0 = thisOAM.enOAM.OAM0 & 0xFF00 | (thisOAM.enOAM.OAM0 + Midscreensamus) & 0xFF;

	//	thisOAM.enOAM.OAM1 = thisOAM.enOAM.OAM1 & 0xFE00 | (thisOAM.enOAM.OAM1 + HorizMiddleOfSamus) & 0x1FF;//thisOAM.enOAM.OAM2 = thisOAM.enOAM.OAM2 & 0xFE00 | (thisOAM.enOAM.OAM1 ) & 0x1FF;
	//	//unsigned short backup = thisOAM.enOAM.OAM2;
	//thisOAM.enOAM.OAM2 = (thisOAM.enOAM.OAM2 & 0xF3);// | 4 * SamusFront;


	//	FrameManager::UnpackOAM(&thisOAM.enOAM, &thisOAM.deOAM);

	//	theSprite->OAM.push_back(thisOAM);
	//	theSprite->maxparts++;
	//}

	for (int OamCount = 0; OamCounter < count;OamCounter = (OamCounter + 1) & 0xFF)
	{
		OverAllOAM thisOAM;
		memset(&thisOAM, 0, sizeof(OverAllOAM));
		MemFile::currentFile->fread(&thisOAM.enOAM.OAM0, 2, 1);
		MemFile::currentFile->fread(&thisOAM.enOAM.OAM1, 2, 1);
		MemFile::currentFile->fread(&thisOAM.enOAM.OAM2, 2, 1);

		thisOAM.enOAM.OAM0 = thisOAM.enOAM.OAM0 & 0xFF00 | ((thisOAM.enOAM.OAM0 + Midscreensamus) & 0xFF);


		thisOAM.enOAM.OAM1 = thisOAM.enOAM.OAM1 & 0xFE00 | (thisOAM.enOAM.OAM1+ HorizMiddleOfSamus)& 0x1FF  ;
		/*thisOAM.enOAM.OAM2 = (thisOAM.enOAM.OAM2 & 0xF3) | 4 * SamusFront;*/
		/*	unsigned short up =  (thisOAM.enOAM.OAM2 & 0xFF00)>>8 & 0xF3;
			thisOAM.enOAM.OAM2 = up << 8 | thisOAM.enOAM.OAM2 & 0xFF;*/
		FrameManager::UnpackOAM(&thisOAM.enOAM, &thisOAM.deOAM);
	
//			thisOAM.deOAM.xCoord -= HorizMiddleOfSamus*8;			
		
		theSprite->OAM.push_back(thisOAM);
		theSprite->maxparts++;
	}
}
