#include "FusionSamus.h"

#include "..\MemFile\MemFile.h"



unsigned long GetPointer(unsigned long addr)
{
	unsigned long theAddress = addr;
	if (addr > 0x8000000)
	{
		addr -= 0x8000000;
	}
	unsigned char* rawFile = MemFile::currentFile->GetFile();
	unsigned long pnt = 0;
	memcpy(&pnt, &rawFile[addr], 4);
	return pnt - 0x8000000;
}

unsigned long Getlong(unsigned long addr)
{
	unsigned long theAddress = addr;
	if (addr > 0x8000000)
	{
		addr -= 0x8000000;
	}
	unsigned char* rawFile = MemFile::currentFile->GetFile();
	unsigned long pnt = 0;
	memcpy(&pnt, &rawFile[theAddress], 4);
	return pnt;
}

unsigned char* GetData(unsigned long addr, unsigned long size)
{
	return NULL;;
}

FusionSamus::FusionSamus()
{
	_8BitFrameCounter = 0;

	BeamStatus = 0;
	MissilesSelected = 0;
	SamusSuitStatus = 0;
	SamusDirection = 0;
	byte_3001247 = 0;
	Button_input = 0;
	ChargeBeamCounter = 0;
	SamusPose = 0;
	CurrentAnimation = 0;
	//SamusPose = 0;
	Invincibilitytimer = 0;
	AbsorbXTimer = 0;
	SpeedBoosting = 0;
	SpeedBoostingindicator = 0;
	SpeedBoostingindicatorCurrent = 0;
	lastPreviousPositionIndex = 0;
	Previous_64_X_positions = new unsigned long[64];
	Previous_64_Y_positions = new unsigned long[64];
	Previous_SamusX_position = 0;
	startNewPrevPositions = 0;
	ArmCannonDirection = 0;
	Previous_SamusY_position = 0;
	screwAttackAnimationCounter = 0;
	ScrewAttackState = 0;
	Direction = 0;
	isFacingleft = 0;
	Shinespark_timer = 0;
	SpeedBoostCounter = 0;
	GameMode = 8;
	EventController = 0;
	byte_3001304 = 0;

}


FusionSamus::~FusionSamus()
{
}

int FusionSamus::UpdateSAXSamusPal(unsigned short* a1, int palIndex, int palCount, int issax_1)
{
	int issax; // r7
	unsigned short* v5; // r3
	int v6; // r4
	int v7; // r6
	unsigned short* saxpal = NULL; // r5
	unsigned short* sampal; // r1
	int v10; // r4
	int v12; // [sp-4h] [bp-4h]

	issax = issax_1;
	v5 = a1;
	v6 = palIndex;
	v7 = palIndex + palCount;
	if (palIndex < palIndex + palCount)
	{
		//saxpal = (SA_X_suit_colour + 2 * palIndex);
		sampal = (Suit_color + 2 * palIndex);
		v10 = v7 - v6;
		do
		{
			/*if (issax)
				*saxpal = *v5;
			else*/
			*sampal = *v5;
			++v5;
			//++saxpal;
			++sampal;
			--v10;
		} while (v10);
	}
	return true;
}

void FusionSamus::DrawMe()
{

	long PalPnt = 0;
	long palsize = 0;
	long addybuf = 0;
	long size = 0;
	int i = 0;
	int ii = 0;
	unsigned char* compBuffer = new unsigned char[64691];
	GBAMethods* GBA = new GBAMethods();


	unsigned char* decompbuf = new unsigned char[32687];

	unsigned short transferpal[256] = { 0 };

	int  X = 0;
	long off = 0;
	int x = 0;

	
	GBA->DecodePal((short*)this->Suit_color, theSprite->PreviewPal, 16, 0);
	//theSprite->PreviewSprite.SetPaletteRaw(theSprite->PreviewPal);
	memcpy(theSprite->PreRAM, Sprite_tiles, 0x8000);

	theSprite->id = 0xFF;


	RD1Engine::theGame->mgrOAM->DrawPSprite(theSprite);
	
	delete[] compBuffer;
	delete[] decompbuf;
}

void FusionSamus::CannonCode(int SamusPose, unsigned long cannonGfxSizeTable, int samusDirection, int facingDirections)
{
	//unsigned char* rawFile = MemFile::currentFile->GetFile();

	//unsigned long* CannonBottomTemp;
	//
	//signed __int16 bottomgfxsize; // r0
	//if (SamusPose - 23 > 0x29)
	//{
	//HandleDirection:
	//	cannonGfxSizeTable = cannonGfxSizeTable + (4 * 2 * CurrentAnimation);
	//	//GfxTablePnt = ;
	//	gfxsize = rawFile[cannonGfxSizeTable];
	//	if (MissilesSelected & 1)
	//	{
	//		if (SamusDirection & DirectionRight)
	//		{
	//			facingDirections = 4 * samusDirection;

	//			SamusCannonTop = new SamusPiece( GetPointer(0x28F99C + facingDirections), rawFile);
	//			SamusCannonBottom = new SamusPiece(GetPointer(0x28F9B4 + facingDirections), rawFile);
	//		}
	//		else
	//		{
	//			facingDirections = 4 * samusDirection;
	//			SamusCannonTop = new SamusPiece(GetPointer(0x28F9CC + facingDirections), rawFile);
	//			SamusCannonBottom = new SamusPiece(GetPointer(0x28F9E4 + facingDirections), rawFile);
	//		}
	//	}
	//	else if (SamusDirection & DirectionRight)
	//	{
	//		facingDirections = 4 * samusDirection;
	//		SamusCannonTop = new SamusPiece( GetPointer(0x28F93C + facingDirections), rawFile);
	//		SamusCannonBottom = new SamusPiece(GetPointer(0x28F954 + facingDirections), rawFile);
	//	}
	//	else
	//	{
	//		facingDirections = 4 * samusDirection;
	//		SamusCannonTop = new SamusPiece( GetPointer(0x28F96C + facingDirections), rawFile);
	//		SamusCannonBottom = new SamusPiece(GetPointer(0x28F984 + facingDirections), rawFile);
	//	}
	//SetCannonBottomHalfFXOffsetTemp:
	//	CannonBottomTemp = (facingDirections + CannonBottomHalfFXOffset);
	//SetCannonBottomHalfGFXOffset:
	//	CannonBottomHalfGFXOffset = (unsigned long)CannonBottomTemp;
	//	bottomgfxsize = 192;
	//SetCannonTopHalfLen:
	//	CannonTopHalfGFXLen = bottomgfxsize;
	//SetCannonBottomHalfFXOffsetLen:
	//	CannonBottomHalfGFXLen = bottomgfxsize;
	//}
	//else
	//{
	//	switch (SamusPose)
	//	{
	//	case Elevator_:
	//		//GfxTablePnt = &((unsigned long*)rawFile[0x28BF98])[2 * screwAttackAnimationCounter];
	//		gfxsize = 0x2000;
	//		SamusCannonTop = new SamusPiece(0x2DE9B0 + samusDirection * 4, rawFile);
	//		CannonBottomTemp = ((unsigned long*)rawFile[0x2DEA70 + samusDirection * 4]);
	//		goto SetCannonBottomHalfGFXOffset;
	//	case ScrewAttacking:
	//		//	GfxTablePnt = &((unsigned long **)rawFile[0x28D66C])[someIndex][2 * screwAttackAnimationCounter];
	//		gfxsize = 4096;
	//		SamusCannonTop = new SamusPiece(0x28FABC + samusDirection * 4 + screwAttackAnimationCounter, rawFile);
	//		//CannonBottomHalfFXOffset = &((unsigned long**)rawFile[0x28FACC])[samusDirection][screwAttackAnimationCounter];
	//		bottomgfxsize = 256;
	//		goto SetCannonTopHalfLen;
	//	case CannonLadder:
	//	case ShootUpLadder:
	//		//GfxTablePnt = (unsigned long*)&rawFile[GetPointer(GfxSizeTable + (4 * 2 * CurrentAnimation + 1))];
	//		//gfxsize = *GfxTablePnt;
	//		if (MissilesSelected & 1)
	//		{
	//			if (SamusDirection & DirectionRight)
	//			{
	//				facingDirections = 4 * samusDirection;
	//				SamusCannonTop = new SamusPiece(0x28F99C + facingDirections, rawFile);
	//				CannonBottomHalfFXOffset = ((unsigned long*)rawFile[0x28F9B4 + facingDirections]);
	//			}
	//			else
	//			{
	//				facingDirections = 4 * samusDirection;
	//				SamusCannonTop = new SamusPiece(0x28F9CC + facingDirections, rawFile);
	//				CannonBottomHalfFXOffset = ((unsigned long*)rawFile[0x28F9E4 + facingDirections]);
	//			}
	//		}
	//		else if (SamusDirection & DirectionRight)
	//		{
	//			facingDirections = 4 * samusDirection;
	//			SamusCannonTop = new SamusPiece(0x28FA5C + facingDirections, rawFile);
	//			CannonBottomHalfFXOffset = ((unsigned long*)rawFile[0x28FA74 + facingDirections]);
	//		}
	//		else
	//		{
	//			facingDirections = 4 * samusDirection;
	//			SamusCannonTop = new SamusPiece(0x28FA8C + facingDirections, rawFile);
	//			CannonBottomHalfFXOffset = ((unsigned long*)rawFile[0x28FAA4 + facingDirections]);
	//		}
	//		goto SetCannonBottomHalfFXOffsetTemp;
	//	case HangingHorizLadder:
	//	case DelayAfterShootingHOrizladder:
	//	case ShootingOnHorizladder:
	//	{
	//		//THIS IS WRONMG
	//		cannonGfxSizeTable = cannonGfxSizeTable + (4 * 2 * CurrentAnimation);
	//		//GfxTablePnt = ;
	//		gfxsize = rawFile[cannonGfxSizeTable];
	//		if (MissilesSelected & 1)
	//		{
	//			if (SamusDirection & DirectionRight)
	//			{
	//				facingDirections = 4 * samusDirection;
	//				SamusCannonTop = new SamusPiece(0x28F99C + facingDirections, rawFile);
	//				CannonBottomHalfFXOffset = ((unsigned long*)rawFile[0x28F9B4 + facingDirections]);
	//			}
	//			else
	//			{
	//				facingDirections = 4 * samusDirection;
	//				SamusCannonTop = new SamusPiece(0x28F9CC + facingDirections, rawFile);
	//				CannonBottomHalfFXOffset = ((unsigned long*)rawFile[0x28F9E4 + facingDirections]);
	//			}
	//		}
	//		else if (SamusDirection & DirectionRight)
	//		{
	//			facingDirections = 4 * samusDirection;
	//			SamusCannonTop = new SamusPiece(0x28F9FC + facingDirections, rawFile);
	//			CannonBottomHalfFXOffset = ((unsigned long*)rawFile[0x28FA14 + facingDirections]);
	//		}
	//		else
	//		{
	//			facingDirections = 4 * samusDirection;
	//			SamusCannonTop = new SamusPiece(0x28FA2C + facingDirections, rawFile);
	//			CannonBottomHalfFXOffset = ((unsigned long*)rawFile[0x28FA44 + facingDirections]);
	//		}
	//		goto SetCannonBottomHalfFXOffsetTemp;
	//	}
	//	case UnlockSecurity:
	//		//GfxTablePnt = ((unsigned long **)rawFile[0x28D774 + someIndex])[2 * screwAttackAnimationCounter];
	//		gfxsize = 0x2000;
	//		SamusCannonTop = new SamusPiece(0x2E4250 + facingDirections, rawFile);
	//		CannonBottomHalfGFXOffset = 0x2E4450 + facingDirections;
	//		bottomgfxsize = 512;
	//		goto SetCannonTopHalfLen;
	//	case LoadSave:
	//		if (ScrewAttackState != 1)
	//			break;
	//		//GfxTablePnt = &((unsigned long rawFile[0x28CE8C])[samusDirection][2 * screwAttackAnimationCounter];
	//		gfxsize = 4096;
	//		SamusCannonTop = new SamusPiece(0x28FADC + 4 * screwAttackAnimationCounter, rawFile);
	//		CannonBottomHalfGFXOffset = 0x28FB9C + 4 * screwAttackAnimationCounter;
	//		CannonTopHalfGFXLen = 384;
	//		bottomgfxsize = 256;
	//		goto SetCannonBottomHalfFXOffsetLen;
	//	case SamusDying:
	//		switch (ScrewAttackState)
	//		{
	//		case 0:
	//			/*	DMA3SAD = ((unsigned short *)rawFile[0x2DF730;
	//				DMA3DAD = &Sprite_tiles[576];
	//				DMA3CNT_L = 0x80000030;
	//				v19 = DMA3CNT_L;
	//				DMA3SAD = ((unsigned short *)rawFile[0x2DF790;
	//				DMA3DAD = &Sprite_tiles[832];
	//				DMA3CNT_L = 0x80000020;
	//				DMAReturn2 = DMA3CNT_L;*/
	//			return;
	//		case 2:
	//			//	GfxTablePnt = &((unsigned long **)rawFile[0x28D77C])[samusDirection][someIndex];// [2 * screwAttackAnimationCounter];

	//			gfxsize = 0x2000;
	//			/*SamusCannonTop = new SamusPiece(((SamusAnim*)rawFile[0x28FC5C])[screwAttackAnimationCounter][samusDirection].Tophalf;
	//			CannonBottomHalfGFXOffset = ((SamusAnim*)rawFile[0x28FC74])[screwAttackAnimationCounter][samusDirection].BottomHalf;*/
	//			bottomgfxsize = 256;
	//			goto SetCannonTopHalfLen;
	//		case 3:
	//			gfxsize = 0x2000;
	//			break;
	//		}
	//		break;
	//	case GrabbedByYakuza:
	//		samusDirection = 5;

	//	default:
	//		goto HandleDirection;
	//	}
	//}

}

void FusionSamus::PalCode(int CurrentPose)
{
	unsigned char* rawFile = MemFile::currentFile->GetFile();
	unsigned short* NewPalPointer = NULL; // r4
	unsigned int speedBoostIndex; // r1
	int indexByXTimer; // r1
	unsigned short* PalTablePointer = NULL; // r0
	unsigned short* FinalPalPointer = NULL; // r0
	int PalIndex; // r1
	int PalSize; // r2
	unsigned short* AnotherPalPointer = NULL; // r4
	unsigned int CurAnimState; // r1
	int SavingPalIndex; // r1
	unsigned short* APalTable = NULL; // r0
	unsigned short* APalPointer = { 0 }; // r1
	__int32 ChargeBeamStartPalCounter; // r1
	int chargeCounter; // r1
	unsigned short* PalPointer = NULL; // r4
	if (SamusPose != SamusDying)
	{
		if (Invincibilitytimer && (_8BitFrameCounter & 3u) <= 1)
			goto SomeSuitLen;
		if (AbsorbXTimer)
		{
			SuitPalLen = 64;
			indexByXTimer = 8 * ((48 - AbsorbXTimer) >> 1);
			PalTablePointer = (unsigned short*)XabsorbTable;
		ChangePalPointer:
			NewPalPointer = &PalTablePointer[indexByXTimer];
			goto AnUpdatePal;
		}
		if (((byte_3001304 - 1) & 0xFFu) <= 3)
		{
		SomeSuitLen:
			SuitPalLen = 64;
			NewPalPointer = ((unsigned short*)rawFile[0x28DDFC]);
		AnUpdatePal:
			UpdateSAXSamusPal(NewPalPointer, 0, 16, 0);
			goto SetPalPointer;
		}
		if (SamusPose - 30 > 0x1F)
		{
		BasicPal:
			SuitPalLen = 64;
			if (SpeedBoosting || Shinespark_timer)
			{
				NewPalPointer = ((unsigned short*)rawFile[0x28DEBC]);
				if (_8BitFrameCounter & 8)
					NewPalPointer = ((unsigned short*)rawFile[0x28DE9C]);
				goto AnUpdatePal;
			}
			if (ChargeBeamCounter <= 0x3Fu)
			{
				if (SamusSuitStatus & SaxSuit)
				{
					PalPointer = (unsigned short*)(&rawFile[0x28DDDC]);
				}
				else if (SamusSuitStatus & 0x20)
				{
					PalPointer = (unsigned short*)(&rawFile[0x28DDBC]);
				}
				else
				{
					PalPointer = (unsigned short*)(&rawFile[0x28DD7C]);
					if (SamusSuitStatus & 0x10) {
						PalPointer = (unsigned short*)(&rawFile[0x28DD9C]);//0x28DD7C + 8]);
					}
				}
				UpdateSAXSamusPal(PalPointer, 0, 16, 0);
				if (CurrentPose != 52)
					PalPointer = (unsigned short*)(&rawFile[0x28DE1C]);
				UpdateSAXSamusPal(PalPointer, 16, 16, 0);
				return;
			}
			if (SamusSuitStatus & SaxSuit)
			{
				ChargeBeamStartPalCounter = 12;
			}
			else if (SamusSuitStatus & GravitySuit)
			{
				ChargeBeamStartPalCounter = 8;
			}
			else
			{
				ChargeBeamStartPalCounter = (-(SamusSuitStatus & Varia) >> 31) & 4;
			}
			chargeCounter = ChargeBeamStartPalCounter + ((ChargeBeamCounter - 64) >> 2);
			if (BeamStatus & 0x10)
			{
				indexByXTimer = 8 * chargeCounter;
				PalTablePointer = ((unsigned short*)rawFile[0x28F4FC]);
			}
			else if (BeamStatus & 8)
			{
				indexByXTimer = 8 * chargeCounter;
				PalTablePointer = ((unsigned short*)rawFile[0x28F2FC]);
			}
			else if (BeamStatus & 4)
			{
				indexByXTimer = 8 * chargeCounter;
				PalTablePointer = ((unsigned short*)rawFile[0x28F0FC]);
			}
			else if (BeamStatus & 2)
			{
				indexByXTimer = 8 * chargeCounter;
				PalTablePointer = ((unsigned short*)rawFile[0x28EEFC]);
			}
			else if (BeamStatus & 1)
			{
				indexByXTimer = 8 * chargeCounter;
				PalTablePointer = ((unsigned short*)rawFile[0x28ECFC]);
			}
			else
			{
				indexByXTimer = 8 * chargeCounter;
				PalTablePointer = ((unsigned short*)rawFile[0x28EAFC]);
			}
			goto ChangePalPointer;
		}
		switch (SamusPose)
		{
		case ScrewAttacking:
			SuitPalLen = 64;
			if (CurrentAnimation > 4u)
			{
				if (CurrentAnimation <= 7u)
				{
					AnotherPalPointer = ((unsigned short*)rawFile[0x28DE3C]);
					goto LABEL_111;
				}
				AnotherPalPointer = ((unsigned short*)rawFile[0x28DE3C]);
				if (CurrentAnimation <= 0xCu)
					goto LABEL_110;
			}
			else
			{
				if (SamusSuitStatus & SaxSuit)
				{
					AnotherPalPointer = ((unsigned short*)rawFile[0x28DDDC]);
					goto LABEL_111;
				}
				if (SamusSuitStatus & GravitySuit)
				{
					AnotherPalPointer = ((unsigned short*)rawFile[0x28DDBC]);
					goto LABEL_111;
				}
				AnotherPalPointer = ((unsigned short*)rawFile[0x28DD7C]);
				if (SamusSuitStatus & Varia)
				{
				LABEL_110:
					AnotherPalPointer += 8;
					goto LABEL_111;
				}
			}
		LABEL_111:
			UpdateSAXSamusPal(AnotherPalPointer, 0, 16, 0);
			NewPalPointer = ((unsigned short*)rawFile[0x28DE7C]);
			goto SetPalPointer;
		case Frozen:
		case FrozenAndFalling:
		case FrozenInMorphBall:
		case FrozenInMorphballFalling:
			SuitPalLen = 64;
			FinalPalPointer = ((unsigned short*)&rawFile[0x28E0FC]);
			PalIndex = 0;
			PalSize = 32;
			goto JustUpdatePal;
		case Saving:
			SuitPalLen = 32;
			CurAnimState = (CurrentAnimation >> 1) & 3;
			if (SamusSuitStatus & SaxSuit)
			{
				SavingPalIndex = 8 * CurAnimState;
				APalTable = ((unsigned short*)rawFile[0x28F87C]);
			}
			else if (SamusSuitStatus & GravitySuit)
			{
				SavingPalIndex = 8 * CurAnimState;
				APalTable = ((unsigned short*)rawFile[0x28F7FC]);
			}
			else if (SamusSuitStatus & Varia)
			{
				SavingPalIndex = 8 * CurAnimState;
				APalTable = ((unsigned short*)rawFile[0x28F77C]);
			}
			else
			{
				SavingPalIndex = 8 * CurAnimState;
				APalTable = ((unsigned short*)rawFile[0x28F6FC]);
			}
			FinalPalPointer = &APalTable[SavingPalIndex];
			PalIndex = 0;
			goto DefaultPalsSize;
		case LoadSave:
			SuitPalLen = 64;
			if (SamusSuitStatus & SaxSuit)
			{
				APalPointer = ((unsigned short*)rawFile[0x28EA8C]);
			}
			else if (SamusSuitStatus & GravitySuit)
			{
				APalPointer = ((unsigned short*)rawFile[0x28EA1C]);
			}
			else if (SamusSuitStatus & Varia)
			{
				APalPointer = ((unsigned short*)&rawFile[0x28E9AC]);
			}
			else
			{
				APalPointer = ((unsigned short*)&rawFile[0x28E93C]);
			}
			UpdateSAXSamusPal(&APalPointer[CurrentAnimation], 0, 16, 0);
			NewPalPointer = ((unsigned short*)&rawFile[0x28E61C]);
			goto SetPalPointer;
		default:
			goto BasicPal;
		}
	}
	SuitPalLen = 64;
	UpdateSAXSamusPal(((unsigned short*)&rawFile[0x28DEDC]), 0, 16, 0);
	NewPalPointer = ((unsigned short*)&rawFile[0x28DEFC]);
	if (screwAttackAnimationCounter > 4u)
	{
		if (SpeedBoostCounter > 0x4Fu)
		{
			GameMode = 8;
			EventController = 0;
		}
		else
		{
			++SpeedBoostCounter;
		}
		if (SpeedBoostCounter > 0x3Bu)
			speedBoostIndex = 15;
		else
			speedBoostIndex = SpeedBoostCounter << 24 >> 26;
		NewPalPointer = &((unsigned short*)rawFile[0x28DEFC])[8 * speedBoostIndex];
	}
SetPalPointer:
	FinalPalPointer = NewPalPointer;
	PalIndex = 16;
DefaultPalsSize:
	PalSize = 16;
JustUpdatePal:
	UpdateSAXSamusPal(FinalPalPointer, PalIndex, PalSize, 0);

}
//Loads up the logic for the specific animation.
void FusionSamus::GetOAM(int anim, int samusDirection, int facingDirections, int CurrentPose, unsigned long cannonGfxSizeTable)
{
	unsigned char* rawFile = MemFile::currentFile->GetFile();
	int IsSamusFacingLeft; //
// r6
	unsigned long currentSizeTable = NULL; // r1
	unsigned long* DifferentAnimTable = NULL; // r2
	int TheDirectionIndex; // r1

	int someIndex; // r5
	SamusAnim* theCurrentAnim = NULL; // r6
	unsigned  long* tophalf; // r3
	unsigned long* legspointer = NULL; // r3


	IsSamusFacingLeft = true;// isFacingLeft;
	//loopie loop

	theCurrentAnim = &Animtable[anim];
	SamusOAMPointer = theCurrentAnim->OAMPointer;

	SamusTop = new SamusPiece(theCurrentAnim->Tophalf - 0x8000000, rawFile);
	
	/*tophalf = (unsigned long*)&rawFile[(unsigned long)theCurrentAnim->Tophalf - 0x8000000];
	SamusGFXTophalfTileLen = 32 * rawFile[(unsigned long)theCurrentAnim->Tophalf - 0x8000000];
	SamusGFXBottomHalfLen = 32 * rawFile[(unsigned long)theCurrentAnim->Tophalf + 1 - 0x8000000];


	SamusGFXTopHalfOffset = (unsigned long)theCurrentAnim->Tophalf + 2 - 0x8000000;
	SamusGFXBottomOffset = (unsigned long)theCurrentAnim->Tophalf + 2 - 0x8000000 + SamusGFXTophalfTileLen;*/

	SamusBottom = new SamusPiece(theCurrentAnim->BottomHalf - 0x8000000, rawFile);
	//legspointer = (unsigned long*)&rawFile[(unsigned long)theCurrentAnim->BottomHalf - 0x8000000];
	//SamusGFXLegsTopLen = 32 * rawFile[(unsigned long)theCurrentAnim->BottomHalf - 0x8000000];
	//SamusGFXLegsBottomLen = 32 * rawFile[(unsigned long)theCurrentAnim->BottomHalf + 1 - 0x8000000];
	//SamusGFXLegsTopOffset = (unsigned long)theCurrentAnim->BottomHalf + 2 - 0x8000000;
	//SamusGFXLegsBottomOffset = (unsigned long)theCurrentAnim->BottomHalf + 2 - 0x8000000 + SamusGFXLegsTopLen;
	gfxsize = 0;

	CannonCode(SamusPose, cannonGfxSizeTable, samusDirection, facingDirections );
	PalCode(CurrentPose);
	
}


