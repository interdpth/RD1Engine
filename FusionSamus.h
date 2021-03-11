#pragma once
#include "SamusBase.h"
#include <vector>
using namespace std;

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

	void DrawMe();
	unsigned short BeamStatus;
	unsigned short MissileBombStatus;
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
	void SetOAM(int anim, int samusDirection, int facingDirections, int CurrentPose, unsigned long GfxSizeTable);
	int CurrentAnimation;

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
	SamusAnim* Animtable;
	unsigned long XabsorbTable = 0x828E13C;
	FusionSamus();
	~FusionSamus();
	
	void Copy(FusionSamus* dst)
	{
		dst->_8BitFrameCounter=_8BitFrameCounter;
		
		dst->BeamStatus= BeamStatus;
		dst->MissileBombStatus= MissileBombStatus;
		dst->MissilesSelected= MissilesSelected;
		dst->SamusSuitStatus= SamusSuitStatus;
		dst->SamusDirection= SamusDirection;
		dst->byte_3001247= byte_3001247;
		dst->Button_input=  Button_input;
		dst->ChargeBeamCounter= ChargeBeamCounter;

		dst->CurrentAnimation= CurrentAnimation;

		dst->Invincibilitytimer= Invincibilitytimer;
		dst->AbsorbXTimer= AbsorbXTimer;
		dst->SpeedBoosting= SpeedBoosting;
		dst->SpeedBoostingindicator= SpeedBoostingindicator;
		dst->SpeedBoostingindicatorCurrent = SpeedBoostingindicatorCurrent;
		dst->lastPreviousPositionIndex= lastPreviousPositionIndex;
		//Fix later
		/* long* Previous_64_X_positions;
		 long* Previous_64_Y_positions;*/
		dst->Previous_SamusX_position = Previous_SamusX_position;
		dst->startNewPrevPositions = startNewPrevPositions;
		dst->ArmCannonDirection = ArmCannonDirection;
		dst->Previous_SamusY_position = Previous_SamusY_position;
		dst->screwAttackAnimationCounter = screwAttackAnimationCounter;
		dst->ScrewAttackState = ScrewAttackState;
		dst->Direction = Direction;
		dst->isFacingleft = isFacingleft;
		dst->Shinespark_timer = Shinespark_timer;
		dst->SpeedBoostCounter = SpeedBoostCounter;
		dst->GameMode = GameMode;
		dst->EventController = 	EventController;
		dst->byte_3001304 =  byte_3001304;
		//Tables 
		dst->Animtable = Animtable;
	}
};

