#include "FrameManager.h"

FrameManager::~FrameManager()
{
	
	for each(Frame* theFrame in theFrames)
	{
		delete theFrame;
	}
	
	theFrames.clear();
	
	 _staticFrame=NULL;
	 _currentAnimatedFrame= NULL;
	
}


void FrameManager::AddFrame(Frame* src, int SpriteID)
{

	Frame* theFrame = src;
	int currSize = theFrames.size();
	if (src == NULL)
	{
		//construct new frame 

		theFrame = new Frame(_gbaMethods, currSize, SpriteID);

	}
	else
	{
		theFrame = new Frame(_gbaMethods );
		theFrame->frameOffset = 0x8BADBEEF;
		theFrame->frameTimer = src->frameTimer;
		theFrame->theSprite = new SprGBuf(theTileGFX, thePalette);
		memcpy(&theFrame->theSprite, &src->theSprite, sizeof(SprGBuf));
		theFrame->theSprite->id = src->theSprite->id;
		memcpy(&theFrame->theSprite->Borders, &src->theSprite->Borders, sizeof(RECT));
	
		memcpy(&theFrame->theSprite->PreviewPal, &src->theSprite->PreviewPal, 512 * 4);
		memcpy(&theFrame->theSprite->PreRAM, &src->theSprite->PreRAM, 0x8000);
		theFrame->theSprite->PartNo = src->theSprite->PartNo;
		memcpy(&theFrame->theSprite->OAM, &src->theSprite->OAM, sizeof(OverAllOAM) * 512);

		for (int OAMCounter = 0; OAMCounter < src->theSprite->OAM.size(); OAMCounter++)
		{
			theFrame->theSprite->OAM.push_back(src->theSprite->OAM[OAMCounter]);
		}

		theFrame->theSprite->Tiles->Load((BYTE*)&theFrame->theSprite->PreRAM, 1023);
		theFrame->theSprite->maxparts = src->theSprite->maxparts;
		theFrame->theSprite->details = src->theSprite->details;
		theFrame->theSprite->palsize = src->theSprite->palsize;
		theFrame->theSprite->graphicsize = src->theSprite->graphicsize;
		theFrame->index = currSize;
		theFrame->frameInited = false;
		theFrame->animUpdated = true;
	}
	theFrames.push_back(theFrame);
}


void FrameManager::Empty()
{
	animFrameIndex = 0;
	animCounter = 0;
	timeElapsed = 0;
	memset(&animStart, 0, sizeof(time_t));

	
	for each(Frame* theFrame in theFrames)
	{
		delete theFrame;
	}

	theFrames.clear();
	
	memset(&thePalette, 0, sizeof(long) * 512);
	//memset(&thePalette, 0, sizeof(unsigned char) * 0x8000);
	_staticFrame = NULL;;
	 _currentAnimatedFrame = NULL;  
	
}
///
///Frame maanger constructor
FrameManager::FrameManager(GBAMethods* GBA, unsigned long offset, FILE* ROM, int spriteID, int romType, bool NoFrameTable)
{
	_gbaMethods = GBA;
	Empty();
	
	LoadFrames(offset, ROM, spriteID, romType);
	originalFrameCount = theFrames.size();

}

///
///Frame maanger constructor
FrameManager::FrameManager(GBAMethods* GBA, unsigned long offset, FILE* ROM, int spriteID, int romType, unsigned char* tileGFX, long* pal, bool NoFrameTable )
{
	_gbaMethods = GBA;
	Empty();
	
	LoadFrames(offset, ROM, spriteID, romType,  tileGFX,  pal, NoFrameTable);
	originalFrameCount = theFrames.size();
}

//Handles animating
bool FrameManager::Animate()
{
	if (animCounter == 0xFF)
	{
		return false;
	}

	if (animCounter == 0)
	{

		animFrameIndex++;
		if (animFrameIndex >= theFrames.size())
		{
			animFrameIndex = 0;

		}
		_currentAnimatedFrame = theFrames.at(animFrameIndex);
//		_currentAnimatedFrame->theSprite->PreviewSprite.RefreshImage();
		_currentAnimatedFrame->animUpdated = true;
	}
	else if (animCounter == _currentAnimatedFrame->frameTimer)
	{
		animCounter = 0;
		return true;
	}
	else
	{
		animCounter++;
	}
	
	return true;
}

void FrameManager::LoadFrames(unsigned long offset, FILE* ROM, int spriteID, int romType, unsigned char* tileGFX, long* pal, bool NoFrameTable)
{
	if (!NoFrameTable)
	{

		memset(theTileGFX, 0, 0x8000);
		memset(&thePalette, 0, 512 * sizeof(long));
	}

	try
	{

		if (NoFrameTable)
		{
			Frame* currentFrame = new Frame(_gbaMethods, offset, ROM, 0, spriteID, tileGFX, pal, NoFrameTable);
			unsigned long frameOffset = currentFrame->frameOffset;
			if (frameOffset == 0 || frameOffset < 0x8000000 || frameOffset>0xA000000)
			{
				delete currentFrame;
				currentFrame = NULL;
				
			}
			else
			{
				theFrames.push_back(currentFrame);
			}
		}
		else

		{

			for (int frameCounter = 0; frameCounter < 254; frameCounter++)
			{
				Frame* currentFrame = new Frame(_gbaMethods, offset + frameCounter * 8, ROM, frameCounter, spriteID, tileGFX, pal,false);
				unsigned long frameOffset = currentFrame->frameOffset;
				if (frameOffset == 0 || frameOffset < 0x8000000 || frameOffset>0xA000000)
				{
					delete currentFrame;
					currentFrame = NULL;
					break;
				}
				else
				{
					theFrames.push_back(currentFrame);
				}
			}
		}
	}
	catch (...)
	{
		throw;
	}
	_staticFrame = theFrames.front();
	_currentAnimatedFrame = _staticFrame;
	animFrameIndex = 0;
}


void FrameManager::LoadFrames(unsigned long offset, FILE* ROM, int spriteID, int romType)
{
	memset(&theTileGFX, 0, 0x8000);
	memset(&thePalette, 0, 512 * 4);
	/*try
	{*/
		for (int frameCounter = 0; frameCounter < 32; frameCounter++)
		{
			Frame* currentFrame = new Frame(_gbaMethods, offset + frameCounter * 8, ROM, frameCounter, spriteID, (unsigned char*)&this->theTileGFX, (long*)&this->thePalette, false);
			unsigned long frameOffset = currentFrame->frameOffset;
			if (frameOffset == 0 || frameOffset < 0x8000000 || frameOffset>0xA000000)
			{
				delete currentFrame;
				currentFrame = NULL;
				break;
			}
			else
			{
				theFrames.push_back(currentFrame);
			}
		}
	//}
	//catch (...)
	//{
	//	throw;
	//}

	_currentAnimatedFrame = theFrames.front();
	animFrameIndex = 0;
}

Frame* FrameManager::GetAnimatedFrame()
{
	return _currentAnimatedFrame;
}

Frame* FrameManager::GetStaticFrame()
{
	if (_staticFrame == NULL)
	{
		return new Frame(_gbaMethods);
	}
	return _staticFrame;
}

void FrameManager::SetStaticFrame(int index)
{
	if (theFrames.size() != 0) {
		_staticFrame = theFrames.at(index);
		_staticFrame->Update();
	}
}

int FrameManager::maxframe()
{
	return theFrames.size();
}

int FrameManager::UpdateSprite(int frame, int thisPart, unsigned short tile, unsigned short xCoord, signed char yCoord, int objShape, int objSize, bool horizFlip, bool vertflip, int palIndex) {//Routine unpacks structure then repacks it 	
	unsigned short oamPiece = 0;

	DecodedOAM  deOAM;
	EncodedOAM* bitOAM = &theFrames[frame]->theSprite->OAM[thisPart].enOAM;
	memset(&deOAM, 0, sizeof(DecodedOAM));
	oamPiece = bitOAM->OAM0;
	deOAM.yCoord = yCoord;
	deOAM.objRot =GetBitField(oamPiece, 8, 1);
	deOAM.DSaOD =GetBitField(oamPiece, 9, 1);
	deOAM.ObjMode =GetBitField(oamPiece, 10, 2);
	deOAM.ObjMosiac =GetBitField(oamPiece, 12, 1);
	deOAM.Paltype =GetBitField(oamPiece, 13, 1);
	deOAM.ObjShape = objShape;


	oamPiece = bitOAM->OAM1;
	deOAM.ObjSize = objSize;
	int partWidth = RD1Engine::theGame->mgrOAM->objSizes[deOAM.ObjShape][deOAM.ObjSize][0];
	int partHeight = RD1Engine::theGame->mgrOAM->objSizes[deOAM.ObjShape][deOAM.ObjSize][1];

	if (deOAM.objRot == 1) {
		deOAM.rotation =GetBitField(oamPiece, 9, 5);
	}
	else {
		deOAM.HorizontalFlip = (oamPiece & 0x1000 ? 1 : 0);
		deOAM.VerticalFlip = (oamPiece & 0x2000 ? 1 : 0);
	}

	deOAM.xCoord = xCoord;
	int mathStyle = deOAM.HorizontalFlip && !horizFlip ? -1 : 1;
	if ((bool)deOAM.HorizontalFlip != horizFlip)
	{
		switch (partWidth)
		{
		case 8:
			if (!deOAM.HorizontalFlip && horizFlip)
			{
				deOAM.xCoord -= partWidth;
			}
			else {
				deOAM.xCoord += partWidth;
			}
			break;
		default:
			if (deOAM.HorizontalFlip && !horizFlip)
			{
				deOAM.xCoord -= partWidth;

			}
			else {
				deOAM.xCoord += partWidth;
			}
			break;

		}
	}

	deOAM.HorizontalFlip = horizFlip;
	deOAM.VerticalFlip = vertflip;
	
	oamPiece = bitOAM->OAM2;
	deOAM.TileNumber = (tile) & 0x3ff;// curoam & 0x3FF;
	deOAM.priority = ((oamPiece & 0xC00) >> 0xA) & 3; // (oamPiece >> 10) & 3;//cOAMEdit::GetBitField(oamPiece, 10, 2);
	if (deOAM.priority > 0)
	{
		deOAM.priority = deOAM.priority;
	}
	deOAM.Palette = palIndex;

	bitOAM->OAM0 = EncodeOAM0(deOAM.yCoord, deOAM.objRot, deOAM.DSaOD, deOAM.ObjMode, deOAM.ObjMosiac, deOAM.Paltype, deOAM.ObjShape);
	bitOAM->OAM1 = EncodeOAM1(deOAM.objRot, deOAM.xCoord, deOAM.rotation, deOAM.HorizontalFlip, deOAM.VerticalFlip, deOAM.ObjSize);
	bitOAM->OAM2 = EncodeOAM2(deOAM.TileNumber, deOAM.priority, deOAM.Palette);

	RD1Engine::theGame->mgrOAM->DrawPSprite(GetStaticFrame()->theSprite);

	return 0;
}


void FrameManager::Save(SaveOptions saveType, void* src, int FramesOffset)
{
	if (saveType != SaveOptions::CHEADER) {
		MemFile::currentFile->seek(FramesOffset);
	}
	else {
		//sprintf("%s")
	}
	for (int frameCounter = 0; frameCounter < theFrames.size(); frameCounter++)
	{
		MemFile::currentFile->fwrite(&theFrames[frameCounter]->frameOffset, 4, 1, (FILE*)src);
		MemFile::currentFile->fwrite(&theFrames[frameCounter]->frameTimer, 4, 1, (FILE*)src);
	}
}

int FrameManager::DeleteFrame(int frameIndex)
{
	if (theFrames.size() > 1)
	{
		theFrames.erase(theFrames.begin() + frameIndex);
	}
	return theFrames.size();
}
