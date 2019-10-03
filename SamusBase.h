#include <stdio.h>
#include <Windows.h>
#include "..\MemFile\MemFile.h"
#include "SprGBuf.h"
#include "FrameManager.h"
#pragma once
enum enumSamusPoses
{
	Standing = 0x0,
	TurningAround = 0x1,
	LOOKFORTARGET = 0x2,
	Running = 0x3,
	Jumping_Falling = 0x4,
	TurningAndJumpFall = 0x5,
	Landing = 0x6,
	Ducking = 0x7,
	TurnAndDuck = 0x8,
	ShootAndDuck = 0x9,
	StartSpinJump = 0xA,
	Spinning = 0xB,
	Morphing = 0xC,
	MorphBall = 0xD,
	Rolling = 0xE,
	Unmorphing = 0xF,
	InAirMorpball = 0x10,
	GettingHurt = 0x11,
	GettingHurtInMorphball = 0x12,
	GettingKnockedBack = 0x13,
	GettingKnockedBackInMorph = 0x14,
	StartWalljump = 0x15,
	WallJump = 0x16,
	Elevator_ = 0x17,
	HangLedge = 0x18,
	PullUpFromHang = 0x19,
	PullForwardFromHang = 0x1A,
	PullDownToHang = 0x1C,
	SpaceJumping = 0x1D,
	ScrewAttacking = 0x1E,
	Skidding = 0x1F,
	OnSave = 0x20,
	RechargeSecurity = 0x21,
	TurnAroundBeCharged = 0x22,
	ShineSparkDelay = 0x23,
	ShineSparking = 0x24,
	DelayAfterShineSpark = 0x25,
	HangingLadder = 0x26,
	BegingArmCannonLadder = 0x27,
	CannonLadder = 0x28,
	ShootUpLadder = 0x29,
	VertLadderCanonMove = 0x2A,
	HangingHorizLadder = 0x2B,
	MovingOnHorizlader = 0x2C,
	TurningHorizLAdder = 0x2D,
	DelayAfterShootingHOrizladder = 0x2E,
	ShootingOnHorizladder = 0x2F,
	Frozen = 0x30,
	FrozenAndFalling = 0x31,
	FrozenInMorphBall = 0x32,
	FrozenInMorphballFalling = 0x33,
	UnlockSecurity = 0x34,
	Saving = 0x35,
	OnNavPad = 0x36,
	DownloadAbility = 0x37,
	FacingForward = 0x39,
	FacingBackward = 0x3A,
	LoadSave = 0x3D,
	SamusDying = 0x3E,
	HitByOmega = 0x3F,
	GrabbedByYakuza = 0x40,
};

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
	unsigned char*  DMA3SAD;
	unsigned char*  DMA3DAD;
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
	int	SamusPose;
	int gfxsize;
	unsigned long GfxTablePnt;
	SamusBase();
	~SamusBase();

	SpriteObject* theSprite;
	virtual void Logic() {}
	void Load()
	{
		memset(Sprite_tiles, 0, 0x8000);
		if (SamusGFXTophalfTileLen)
		{
			DMA3CNT_L = (SamusGFXTophalfTileLen);

			DMA3SAD = &MemFile::currentFile->GetFile()[SamusGFXTopHalfOffset];

			DMA3DAD = Sprite_tiles;

			for (unsigned int i = 0; i < DMA3CNT_L; i++) {
				DMA3DAD[i] = DMA3SAD[i];
			}
		}

		if (SamusGFXBottomHalfLen)
		{

			DMA3SAD = &MemFile::currentFile->GetFile()[SamusGFXBottomOffset];
			DMA3DAD = &Sprite_tiles[0x400];
			DMA3CNT_L = (SamusGFXBottomHalfLen);
			for (int i = 0; i < DMA3CNT_L; i++) {
				DMA3DAD[i] = DMA3SAD[i];
			}
		}

		if (SamusGFXLegsTopLen)
		{
			DMA3SAD = &MemFile::currentFile->GetFile()[SamusGFXLegsTopOffset];
			DMA3DAD = &Sprite_tiles[0x280];
			DMA3CNT_L = (SamusGFXLegsTopLen);
			for (int i = 0; i < DMA3CNT_L; i++) {
				DMA3DAD[i] = DMA3SAD[i];
			}
		}

		if (SamusGFXLegsBottomLen)
		{
			DMA3SAD = &MemFile::currentFile->GetFile()[SamusGFXLegsBottomOffset];
			DMA3DAD = &Sprite_tiles[0x680];
			DMA3CNT_L = (SamusGFXLegsBottomLen);
			for (int i = 0; i < DMA3CNT_L; i++) {
				DMA3DAD[i] = DMA3SAD[i];
			}
			//memcpy(DMA3DAD, DMA3SAD, DMA3CNT_L);
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
		MemFile::currentFile->seek(SamusOAMPointer - 0x8000000+2);
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

		int x = 256/8;
		int y = 512/8;

		HorizMiddleOfSamus = ((theSprite->Borders.right - theSprite->Borders.left) /2);
		//BottomOfSamus = ((theSprite->Borders.bottom - theSprite->Borders.top) / 2) & 0xFFFF;
		Midscreensamus = ((((theSprite->Borders.bottom - theSprite->Borders.top) / 2)));
		if (SamusPose > MorphBall || SamusPose < Morphing)
		{

			MiddleOFScreen = x/2;


			HorizMiddleOfSamus = (MiddleOFScreen + MidScreenModifier) ;
			MidVertOfScreen = (y/2) ;
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
		
		for (int OamCount = 0 ; OamCounter<count;OamCounter = (OamCounter + 1) & 0xFF)
		{
			OverAllOAM thisOAM;
			memset(&thisOAM, 0, sizeof(OverAllOAM));
			MemFile::currentFile->fread(&thisOAM.enOAM.OAM0, 2, 1);
				MemFile::currentFile->fread(&thisOAM.enOAM.OAM1, 2, 1);
				MemFile::currentFile->fread(&thisOAM.enOAM.OAM2, 2, 1);

			thisOAM.enOAM.OAM0 = thisOAM.enOAM.OAM0&0xFF00|((thisOAM.enOAM.OAM0 + Midscreensamus)&0xFF);


			thisOAM.enOAM.OAM1 = thisOAM.enOAM.OAM1 & 0xFE00 | (thisOAM.enOAM.OAM1 + HorizMiddleOfSamus) & 0x1FF;
	
		/*	unsigned short up =  (thisOAM.enOAM.OAM2 & 0xFF00)>>8 & 0xF3;
			thisOAM.enOAM.OAM2 = up << 8 | thisOAM.enOAM.OAM2 & 0xFF;*/
				FrameManager::UnpackOAM(&thisOAM.enOAM, &thisOAM.deOAM);

				theSprite->OAM.push_back(thisOAM);
				theSprite->maxparts++;
		}
	}

};

