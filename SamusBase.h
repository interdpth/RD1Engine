#pragma once
#include <stdio.h>
#include <Windows.h>
#include "..\MemFile\MemFile.h"
#include "SprGBuf.h"
#include "FrameManager.h"
#include "SamusPiece.h"
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

struct PieceSize {
	unsigned char BotomHalfLen;
	unsigned char  Top;
	unsigned char  c;
};

/* 78 */

/* 75 */
struct FrameData
{
	int OAMTablePointer;
	int Timer;
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
	unsigned char* DMA3SAD;
	unsigned char* DMA3DAD;
	unsigned char  Sprite_tiles[0x8000];
	unsigned long DMA3CNT_L;
	unsigned short   Suit_color[512];
	unsigned long   OAMPal;
	SamusPiece* SamusTop;
	SamusPiece* SamusBottom;
	SamusPiece* SamusCannonTop;
	SamusPiece* SamusCannonBottom;
	
	unsigned long SamusOAMPointer;
	int SuitPalLen;
	int	SamusPose;
	int gfxsize;
	unsigned long GfxTablePnt;
	SamusBase();
	~SamusBase();

	SpriteObject* theSprite;

	void Load();
	void UnpackSamus();
};

