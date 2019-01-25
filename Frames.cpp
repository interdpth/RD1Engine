#include "Frames.h"
#include <stdio.h>

void Frame::Empty()
{
	frameOffset = 0;
	frameTimer = 0;
	if (theSprite)
	{
		delete theSprite;
	}
	theSprite = NULL;
	index = 0;
	animUpdated = false;
	frameInited = false;
}
Frame::Frame(GBAMethods* gbaMethods)
{
	_gbaMethods = gbaMethods;
	theSprite = NULL;
	Empty();
	theSprite = new SprGBuf();
}

Frame::Frame(GBAMethods* gbaMethods, int newIndex, int SpriteID)
{
	_gbaMethods = gbaMethods;
	theSprite = NULL;
	Empty();
	frameOffset = 0x8BADBEEF;
	frameTimer = 1;
	index = newIndex;
	animUpdated = true;
	frameInited = false;
	this->theSprite = new SprGBuf(NULL, NULL);
	OverAllOAM  tmp;
	tmp.enOAM.OAM0 = 0;
	tmp.enOAM.OAM1 = 0;
	tmp.enOAM.OAM2 = 0x81ff;
	memset(&tmp.deOAM, 0, sizeof(DecodedOAM));
	tmp.deOAM.Palette = 8;
	tmp.deOAM.TileNumber = 0x200;

	theSprite->OAM.push_back(tmp);
	theSprite->id = SpriteID;
}



Frame::Frame(GBAMethods* gbaMethods, unsigned long sourceOffset,  int frameIndex, int spriteID, unsigned char* tileGFX, long* pal,bool NoFrameTable)
{
	_gbaMethods = gbaMethods;
	theSprite = NULL;
	Empty();
	if (!NoFrameTable)
	{
		MemFile::currentFile->seek(sourceOffset);
		MemFile::currentFile->fread(&frameOffset, 4, 1);
		MemFile::currentFile->fread(&frameTimer, 4, 1);
	}
	else {
		frameOffset = sourceOffset;
	}
	this->theSprite = new SprGBuf(tileGFX, pal);
	/*this->theSprite->PreviewSprite.Create(1, 1);*/
	index = frameIndex;
	theSprite->id = spriteID;
	animUpdated = true;
	frameInited = false;
}


Frame::~Frame()
{
	Empty();
	if (this->theSprite != NULL)
	{
		delete this->theSprite;
	}
}

void Frame::AddPart(EncodedOAM* rawOAM)
{
	OverAllOAM newEntry;
	memset(&newEntry, 0, sizeof(newEntry));




	theSprite->OAM.push_back(newEntry);
	if (rawOAM != NULL)
	{

		theSprite->OAM[theSprite->maxparts].enOAM.OAM0 = rawOAM->OAM0;
		theSprite->OAM[theSprite->maxparts].enOAM.OAM1 = rawOAM->OAM1;
		theSprite->OAM[theSprite->maxparts].enOAM.OAM2 = rawOAM->OAM2;

	}
	memset(&theSprite->OAM[theSprite->maxparts].deOAM, 0, sizeof(DecodedOAM));
	theSprite->maxparts++;
}
int Frame::DeletePart(int partIndex)
{
	///So we're deleting OAM, meaning we need to move ever
	if (theSprite->OAM.size() > 1)
	{
		theSprite->OAM.erase(theSprite->OAM.begin() + partIndex);
		theSprite->maxparts--;
	}
	return theSprite->OAM.size();
}


void Frame::SaveInfo(unsigned long sourceOffset,  int itemSize)
{
	MemFile::currentFile->seek(sourceOffset);
	if (frameOffset == 0x8BADBEEF)
	{
		frameOffset = _gbaMethods->FindFreeSpace(itemSize + 16, 0xFF);
	}
	MemFile::currentFile->fread(&frameOffset, 4, 1);
	MemFile::currentFile->fread(&frameTimer, 4, 1);
}




void Frame::Update()
{

}



//returns true when animation updated
