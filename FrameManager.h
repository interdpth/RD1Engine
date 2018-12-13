#pragma once
#include <vector>
#include "BaseGame.h"
#include "Frames.h"
//#include "cOAMEdit.h"
using namespace std;
class Frame;
///Holds info about the current animation
class FrameManager
{
public:
	
	vector<Frame*> theFrames;
	int maxframe();
	long thePalette[512];
	unsigned char theTileGFX[0x8000];
	void Empty();
private:
	Frame* _staticFrame;
	Frame* _currentAnimatedFrame;
	GBAMethods* _gbaMethods;
	int animFrameIndex;
	int timeElapsed;
	time_t animStart;
	int animCounter;
public:
	int DeleteFrame(int frameIndex);
	bool Animate();
	void Save(SaveOptions saveType, void* src, int FramesOffset);
	Frame* GetStaticFrame();
	int originalFrameCount = 0;
	Frame* GetAnimatedFrame();
	void AddFrame(Frame* src, int SpriteID);
	void LoadFrames(unsigned long offset, FILE* ROM, int spriteID, int romType, unsigned char* tileGFX, long* pal, bool NoFrameTable);

	void LoadFrames(unsigned long offset, FILE* ROM, int spriteID, int romType);
	void SetStaticFrame(int index);
	int  UpdateSprite(int frame, int thisPart, unsigned short tile, unsigned short xCoord, signed char yCoord, int objShape, int objSize, bool horizFlip, bool vertflip, int palIndex);
	FrameManager(GBAMethods* GBA, unsigned long offset, FILE* ROM, int spriteID, int romType, bool NoFrameTable = false);
	FrameManager(GBAMethods* GBA,unsigned long offset, FILE* ROM, int spriteID, int romType, unsigned char* tileGFX, long* pal, bool NoFrameTable=false);
	~FrameManager();
public:
	static void UnpackOAM(EncodedOAM* src, DecodedOAM* dst);
	static unsigned short FrameManager::EncodeOAM0(
		unsigned char yCoord,
		unsigned char rot,
		unsigned char objFlag,
		unsigned char objMode,
		unsigned char objMosaic,
		unsigned char palType,
		unsigned char objShape);

	static unsigned short FrameManager::EncodeOAM1(bool rotFlag,
		unsigned short xCoord, unsigned char rotBlob,
		bool hFlip, bool vFlip, unsigned char objSize);

	static unsigned short FrameManager::EncodeOAM2(unsigned short tileNumber, unsigned char bgPriority, unsigned char palette);

};