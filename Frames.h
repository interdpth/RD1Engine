#pragma once
#include <stdio.h>
#include <vector>
#include <time.h>
#include "SprGBuf.h"
#include "cOAMManager.h"
#include "backbuffer.h"
#include "Structures.h"
#include "BaseGame.h"
using namespace std;
#define SET_BIT(val,bitIndex) val& = (val << bitIndex)
#define CLEAR_BIT(val, bitIndex) val &= ~(1 << bitIndex)
#define TOGGLE_BIT(val, bitIndex) val ^= (1 << bitIndex)
#define BIT_IS_SET(val, bitIndex) (val & (1 << bitIndex)) 
class Frame
{
private:
	GBAMethods* _gbaMethods; 
public:
	
	Frame(GBAMethods* _gbaMethods, unsigned long sourceOffset, FILE* fp, int frameIndex, int spriteIndex, unsigned char* tileGFX, long* pal);
	Frame(GBAMethods* _gbaMethods, int frameIndex, int spriteID);
	Frame(GBAMethods* _gbaMethods, Frame* src);
	Frame(GBAMethods* _gbaMethod);
	~Frame();
	int gfxrow;
	void ReadSprite();
	void SaveInfo(unsigned long sourceOffset, FILE* fp, int itemSize);
	int DeletePart(int partIndex);
	void Update(); 
	void AddPart( EncodedOAM* rawOAM);
	void Empty();
	unsigned long frameOffset;
	unsigned long frameTimer; 
	SprGBuf* theSprite;
	int index;
	bool animUpdated; 
	bool frameInited;
};
