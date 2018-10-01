#include "nMapBuffer.h"



nMapBuffer::nMapBuffer()
{
	BImage = NULL;
	//TileBuf2D = new unsigned short[0x10000]; 
	//memset(TileBuf2D, 0, 0x10000);
	UndoBuff = new cUndoBuffer();
	CopyData = new cCopyData();
	DrawRect = new RECT();
	TileBuf2D = NULL;
	ThisCompsize=0;
	ThisDecompedpageSize = 0;
	TilePageCompsize = 0;
	size = 0;
	oSize = 0;
	SDirty = 0;
	Dirty = 0;
	X = 0;
	Y = 0;
	newX = 0;
	newY = 0;
	CompMode = 0;

}


nMapBuffer::~nMapBuffer()
{
	if (BImage != NULL)
	{
		delete BImage;
	}

	if (TileBuf2D)
	{
		delete[] TileBuf2D;
	}

	delete UndoBuff;
	delete CopyData;
	delete DrawRect;
}
