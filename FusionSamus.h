#pragma once
#include "SamusBase.h"

/* 33 */
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

struct PieceSize {
	unsigned char BotomHalfLen;
	unsigned char  Top;
	unsigned char  c;
};

/* 78 */
struct SamusAnim
{
	unsigned long* Tophalf;
	unsigned long* BottomHalf;
	unsigned long* OAMPointer;
	int FrameTimerMaybe;
};
/* 75 */
struct FrameData
{
	int OAMTablePointer;
	int Timer;
};

/* 76 */
enum SamusDirStates
{
	UnknownDirection = 0x0,
	DirectionRight = 0x10,
	DirectionLeft = 0x20,
};

/* 77 */
enum SuitStatus
{
	None = 0,
	HiJump = 0x1,
	SpeedBooster = 0x2,
	SpaceJump = 0x4,
	ScrewAttack_ = 0x8,
	Varia = 0x10,
	GravitySuit = 0x20,
	MorphBall_ = 0x40,
	SaxSuit = 0x80,
};
class FusionSamus :
	public SamusBase
{
public:
	unsigned char _8BitFrameCounter;
	int UpdateSAXSamusPal(unsigned short* a1, int palIndex, int palCount, int issax_1);
	unsigned short BeamStatus;
	bool MissilesSelected;
	unsigned short SamusSuitStatus;
	unsigned short SamusDirection;
	unsigned char byte_3001247;
	unsigned short  Button_input;
	int ChargeBeamCounter;
#define DirectionRight 0x20
#define DirectionLeft 0x10
	/* 35 */
	enum enumEvent
	{
		DataRooMTro = 0x16,
		EscapedTro = 0x17,
		GotHighJumpSpookyBlowup = 0x19,
		ENTERDATAROOM = 0x25,
		EnteredNOCNav = 0x2E,
		IceXHurtYou = 0x2F,
		SaXEnteredARoom = 0x30,
		escapedSAX = 0x31,
		PowerBombGet = 0x42,
		SaxAfterPowerBombs = 0x43,
		BeatYakuzaOnWayToTro = 0x4B,
		SecondTroSAXEncounter = 0x4C,
		SECRETMESSAGETRIGGERED = 0x5C,
		LeaveRestrictedAreaBlowup = 0x5D,
		FINALSAXFIGHTTRIGGERED = 0x65,
		OmegaMetroidKIllsSAXV = 0x6A,
		END_6D = 0x6D,
	};

	/* 36 */
	enum BeamStats
	{
		Basic = 0x0,
		ChargeBeam = 0x1,
		WideBeam = 0x2,
		PlasmaBeam = 0x4,
		WaveBeam = 0x8,
		IceBeam = 0x10,
	};
	int CurrentAnimation;
	int SamusPose;
	int Invincibilitytimer;
	int AbsorbXTimer;
	int SpeedBoosting;
	int SpeedBoostingindicator;
	int SpeedBoostingindicatorCurrent;
	int lastPreviousPositionIndex;
	unsigned long* Previous_64_X_positions;
	unsigned long* Previous_64_Y_positions;
	int Previous_SamusX_position;
	int startNewPrevPositions;
	int ArmCannonDirection;
	int Previous_SamusY_position;
	int screwAttackAnimationCounter;
	int ScrewAttackState;
	int Direction;
	int isFacingleft;
	int Shinespark_timer;
	int SpeedBoostCounter;
	int GameMode = 8;
	int	EventController;
	unsigned char byte_3001304;
	//Tables 
	unsigned long XabsorbTable = 0x828E13C;
	FusionSamus();
	~FusionSamus();
	void Logic();
};

