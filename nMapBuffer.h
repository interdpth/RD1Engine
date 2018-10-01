#pragma once
#include "cCopyData.h"
#include "cUndoBuffer.h"
#include "image.h"
class nMapBuffer
{
public:
	nMapBuffer();
	~nMapBuffer();

	Image *BImage;
	
	cCopyData* CopyData;
	RECT* DrawRect;
	cUndoBuffer* UndoBuff;
	long ThisCompsize;
	long ThisDecompedpageSize;
	long TilePageCompsize;
	long size;
	long oSize;
	unsigned char SDirty;
	unsigned char Dirty;
	unsigned char X;
	unsigned char Y;
	unsigned char newX;
	unsigned char newY;
	unsigned char CompMode;
	unsigned short* TileBuf2D;
	
};

