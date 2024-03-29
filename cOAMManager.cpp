#include "cOAMManager.h"
#include <Windows.h>
#include "MetroidFusion.h"
extern sSpritev idkVRAM;
extern int currentRomType;
void DumpLayers();

unsigned char cOAMManager::maxsprite;
const char* sizesStr[3][4] = {
	{ "8, 8\0", "16,16\0","32,32\0", "64,64\0" },
	{ "16,8\0", "32,8\0" ,"32,16\0", "64,32\0" },
	{ "8,16\0", "8,32\0" ,"16,32\0", "32,64\0" },
};

cOAMManager::cOAMManager(std::map<int, std::vector<unsigned long>>* _oAMFrameTable, GBAMethods* gba, unsigned short* Size, int crf)
{
	memset(&oamPiece, 0, 2 * 17);
	memset(&roomSpriteIds, 0, sizeof(sprite_entry) * 32);

	if (crf == 1)
	{		
		MFSprSize = Size;
	}
	OAMFrameTable = _oAMFrameTable;
	_gbaMethods = gba;
	currentRomType = crf;
}


cOAMManager::~cOAMManager()
{

}


char* cOAMManager::GetSpriteSize(int a, int b) {

	return (char*)&sizesStr[a][b][0];
}

void DrawPart(SpriteObject* SpriteDetails, int x, int y)
{
	return;
}

struct SRECT 
{
	signed short   left;
	signed short    top;
	signed short    right;
	signed short    bottom;
};
int cOAMManager::CalcSpriteBounds(SpriteObject* SpriteDetails) {

	int width = 0;
	int height = 0;
	int partCounter = 0;
	int Tile = 0;
	int bTile = 0;
	int Pal = 0;
	int fw = 0;
	int fh = 0;
	int ty = 0;
	int tx = 0;
	int sx = 0;
	int sy = 0;
	int xFlip = 0;
	int yFlip = 0;
	SRECT PartSize[128];
	const unsigned char objSizes[3][4][2] =
	{
		{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
		{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
		{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	};


	sx = 0;
	sy = 0;

	tx = 0;
	ty = 0;
	memset(&PartSize, 0, sizeof(SRECT) * 128);

	vector<OverAllOAM> partCopies;
	partCopies.insert(partCopies.end(), SpriteDetails->OAM.begin(), SpriteDetails->OAM.end());


	for (partCounter = 0; partCounter < SpriteDetails->maxparts; partCounter++)
	{
		OverAllOAM* thisPart = &partCopies.front() + partCounter;

		if (thisPart->deOAM.ObjShape == 3)
		{
			continue;
		}

		sx = (thisPart->enOAM.OAM1 & 511);
		sy = (thisPart->enOAM.OAM0 & 0xFF);
		if (sy & 0x80)
			sy = sy - 256; //check for the negative
		if (sx & 0x100)
			sx = sx - 511; //same here


		int Pal = ((thisPart->enOAM.OAM2 & 0xF000));


		Tile = Pal + (thisPart->enOAM.OAM2 & 0x3FF);
		

		//Now switch on this and find the sprite width and height.
		thisPart->deOAM.ObjShape = thisPart->enOAM.OAM0 >> 14;
		thisPart->deOAM.ObjSize = thisPart->enOAM.OAM1 >> 14;
		width = objSizes[thisPart->deOAM.ObjShape][thisPart->deOAM.ObjSize][0];
		height = objSizes[thisPart->deOAM.ObjShape][thisPart->deOAM.ObjSize][1];
		PartSize[partCounter].left = sx;
		PartSize[partCounter].top = sy;
		PartSize[partCounter].right = width;
		PartSize[partCounter].bottom = height;
		xFlip = yFlip = fh = fw = 0;
	}

	//Determine i
	SpriteDetails->Borders.left = 0;
		SpriteDetails->Borders.top = 0;
	SpriteDetails->Borders.right = 0;
	SpriteDetails->Borders.bottom = 0;

	for (partCounter = 0; partCounter < SpriteDetails->maxparts; partCounter++) {
		//Check for top coord 
		//0 = Starting X
		//1 = Startiny Y
		//2 = Width
		//3 = Height
		if (PartSize[partCounter].left < SpriteDetails->Borders.left)
			SpriteDetails->Borders.left = PartSize[partCounter].left;
		if (PartSize[partCounter].top < SpriteDetails->Borders.top)
			SpriteDetails->Borders.top = PartSize[partCounter].top;

		SpriteDetails->Borders.right =
			max(SpriteDetails->Borders.right, PartSize[partCounter].left + PartSize[partCounter].right);
		SpriteDetails->Borders.bottom =
			max(SpriteDetails->Borders.bottom, PartSize[partCounter].top + PartSize[partCounter].bottom);

	}

	char str[1024] = { 0 };
	sprintf(str,  "Borders:  %d %d %d %d", SpriteDetails->Borders.left, SpriteDetails->Borders.top, SpriteDetails->Borders.right, SpriteDetails->Borders.bottom);
		OutputDebugString(str);
	return 0;
}



int cOAMManager::DrawPSprite(SpriteObject* SpriteDetails) {

	if (SpriteDetails->id <= 0xF)
	{
		return 0;
	}
	int width = 0;
	int height = 0;
	int partCounter = 0;
	int Tile = 0;
	int bTile = 0;
	int Pal = 0;
	int fw = 0;
	int fh = 0;
	int ty = 0;
	int tx = 0;
	int sx = 0;
	int sy = 0;
	int xFlip = 0;
	int yFlip = 0;
	RECT PartSize[512] = { 0,0,0,0 };
	const unsigned char objSizes[3][4][2] =
	{
		{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
		{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
		{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	};


	sx = 0;
	sy = 0;

	tx = 0;
	ty = 0;


	if(SpriteDetails->sprTileBuffer!=NULL)delete SpriteDetails->sprTileBuffer;
	SpriteDetails->sprTileBuffer = new TileBuffer();

	SpriteDetails->PreviewSprite.Create(512, 512);


	SpriteDetails->PreviewSprite.SetPalette(SpriteDetails->PreviewPal);

	SpriteDetails->sprTileBuffer->Load(SpriteDetails->PreRAM, 1023);
	

	vector<OverAllOAM> partCopies;
	partCopies.resize(SpriteDetails->OAM.size());
		partCopies.insert(partCopies.begin() , SpriteDetails->OAM.begin(), SpriteDetails->OAM.end());
	CalcSpriteBounds(SpriteDetails);
	int adjustedXorigin = 0;
	int adjustedYorigin = 0;
	if (SpriteDetails->Borders.left < 0)
	{
		adjustedXorigin = 0 - SpriteDetails->Borders.left;
	}

	if (SpriteDetails->Borders.top < 0)
	{
		adjustedYorigin = 0 - SpriteDetails->Borders.top;
	}
	for (partCounter = 0; partCounter < SpriteDetails->maxparts; partCounter++)
	{
		/*for (int BGindex = 0; BGindex < 4; BGindex++)
		{*/

			OverAllOAM* thisPart = &partCopies.front() + partCounter;
			///*if (thisPart->deOAM.priority != BGindex)
			//{
			//	continue;
			//}*/
			if (thisPart->deOAM.ObjShape == 3)
			{
				continue;
			}

			//PosModify* modifer = &BaseGame::theGame->poseModifier[sprite_in->spriteID];
			sx = (thisPart->enOAM.OAM1 & 511);
			sy = (thisPart->enOAM.OAM0 & 0xFF);
			if (sy & 0x80)
				sy = sy - 256; //check for the negative
			if (sx & 0x100)
				sx = sx - 511; //same here

							   /*sx = sx + 8;
							   sy = sy + 16;*/
							   //Set up the basic tile

							   /* sx += 24;
							   sy += 104;*/


			int Pal = (thisPart->enOAM.OAM2 + SpriteDetails->details * 0x1000) & 0xF000;

			////MsgBox "Out of Memory Bounds": Exit Function
			//if (SpriteDetails->details > 0)
			//{
			//	SpriteDetails->details *= 16;
			//}
			Tile = Pal + (thisPart->enOAM.OAM2 + (SpriteDetails->details * 0x40) & 0x3FF);
			//Tile+= SpriteDetails->OAM[i];deOAM.VerticalFlip*0x800;

			//Now switch on this and find the sprite width and height.
			thisPart->deOAM.ObjShape = thisPart->enOAM.OAM0 >> 14;
			thisPart->deOAM.ObjSize = thisPart->enOAM.OAM1 >> 14;
			width = objSizes[thisPart->deOAM.ObjShape][thisPart->deOAM.ObjSize][0];
			height = objSizes[thisPart->deOAM.ObjShape][thisPart->deOAM.ObjSize][1];

			PartSize[partCounter].left = sx;

			PartSize[partCounter].left = sx;
			PartSize[partCounter].top = sy;
			PartSize[partCounter].right = width;
			PartSize[partCounter].bottom = height;
			xFlip = yFlip = fh = fw = 0;

			if (thisPart->enOAM.OAM1 & 0x1000) {
				//formula goes here;
				xFlip = width / 8 - 1; //if width = 16 y
				fw = 0x400;
			}
			if (thisPart->enOAM.OAM1 & 0x2000) {
				yFlip = height / 8 - 1;
				fh = 0x800;
			}

			for (ty = 0; ty < height / 8; ty++)
			{
				for (tx = 0; tx < width / 8; tx++)
				{
					int nextX = adjustedXorigin + sx;
					if (nextX > 255)
					{
						nextX -= 255;
					}
					SpriteDetails->PreviewSprite.Draw(*SpriteDetails->sprTileBuffer, nextX + (tx^xFlip) * 8,
						adjustedYorigin + sy + ((ty) ^ yFlip) * 8, Tile + tx + (ty * 32) + fh + fw);
				}
			}
		
	}

	partCopies.clear();

	SpriteDetails->Borders.left = PartSize[0].left;
	SpriteDetails->Borders.top = PartSize[0].top;
	SpriteDetails->Borders.right = PartSize[0].left + PartSize[0].right;
	SpriteDetails->Borders.bottom = PartSize[0].top + PartSize[0].bottom;

	for (partCounter = 1; partCounter < SpriteDetails->maxparts; partCounter++) {
		//Check for top coord 
		//0 = Starting X
		//1 = Startiny Y
		//2 = Width
		//3 = Height
		if (PartSize[partCounter].left < SpriteDetails->Borders.left)
			SpriteDetails->Borders.left = PartSize[partCounter].left;
		if (PartSize[partCounter].top < SpriteDetails->Borders.top)
			SpriteDetails->Borders.top = PartSize[partCounter].top;

		SpriteDetails->Borders.right =
			max(SpriteDetails->Borders.right,PartSize[partCounter].right);
		SpriteDetails->Borders.bottom =
			max(SpriteDetails->Borders.bottom, PartSize[partCounter].bottom);

	}

	SpriteDetails->Borders.right += SpriteDetails->Borders.left;

		SpriteDetails->Borders.bottom += SpriteDetails->Borders.top;
	return 0;
}





///Saves Serialized Program Interface Type Effects(SPRITES)
int cOAMManager::SaveSprite(SaveOptions savetype, SpriteObject* tSprite, unsigned long Offset)
{
	unsigned long theOffset = Offset;
	if (theOffset > 0x8000000)
	{
		theOffset -= 0x8000000;
	}


	MemFile::currentFile->seek(theOffset);
	MemFile::currentFile->fwrite(&tSprite->maxparts, 2, 1);
	OverAllOAM* thisOAM;
	for (int partCounter = 0; partCounter < tSprite->maxparts; partCounter++)
	{
		thisOAM = &tSprite->OAM[partCounter];
		MemFile::currentFile->fwrite(&thisOAM->enOAM.OAM0, 2, 1);
		MemFile::currentFile->fwrite(&thisOAM->enOAM.OAM1, 2, 1);
		MemFile::currentFile->fwrite(&thisOAM->enOAM.OAM2, 2, 1);
	}

	return tSprite->maxparts;
}


int cOAMManager::DecodeOAM(bool OAMED, SpriteObject* tSprite, unsigned long Offset) {

	unsigned short spritecount = 0;

	unsigned short i = 0;
	unsigned long so = 0;
	//Dim bam As String
	unsigned short OAM0 = 0;
	unsigned short OAM1 = 0;
	unsigned short OAM2 = 0;

	if (!Offset || Offset == 0x8BADBEEF || Offset == 0xBADBEEF || tSprite->IsEdited)
		return 0;


	MemFile::currentFile->seek(Offset);
	if (OAMED == false) {
		MemFile::currentFile->fread(&so, 4, 1);
		MemFile::currentFile->seek(so - 0x8000000);
	}


	MemFile::currentFile->fread(&tSprite->maxparts, 2, 1);
	tSprite->OAM.clear();

	for (i = 0; i < tSprite->maxparts; i++) 
	{
		OverAllOAM thisOAM;// &tSprite->OAM[i];
		memset(&thisOAM, 0, sizeof(OverAllOAM));
		DecodedOAM* decodedOam = &thisOAM.deOAM;
		
		MemFile::currentFile->fread(&thisOAM.enOAM.OAM0, 2, 1);
		MemFile::currentFile->fread(&thisOAM.enOAM.OAM1, 2, 1);
		MemFile::currentFile->fread(&thisOAM.enOAM.OAM2, 2, 1);
		
		FrameManager::UnpackOAM(&thisOAM.enOAM, &thisOAM.deOAM);
		
		tSprite->OAM.push_back(thisOAM);
	}
	return 0;
}

int cOAMManager::LoadRoomOAM() {
	LeakFinder::finder->PollHeap();
	unsigned short spritecount = 0;
	unsigned char spriteID = 0;
	unsigned short i = 0;
	unsigned long so = 0;
	//Dim bam As String
	unsigned short OAM0 = 0;
	unsigned short OAM1 = 0;
	unsigned short OAM2 = 0;
	char  windowText[1024] = { 0 };
	OverAllOAM* thisSpritesOAM = NULL;;
	cOAMManager* mgrOAM = RD1Engine::theGame->mgrOAM;
	int numSprites = RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.size();
	for (int spriteCount = 0; spriteCount < numSprites; spriteCount++)
	{
		delete RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.at(spriteCount);
	}

	RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.clear();

	for (spritecount = 0; spritecount < mgrOAM->maxsprite; spritecount++)
	{
		spriteID = mgrOAM->roomSpriteIds[spritecount].spriteID;
		if (spriteID == 0 || OAMFrameTable->count(spriteID)==0 || OAMFrameTable->at(spriteID).size() == 0)
		{
			continue;
		}

		MemFile::currentFile->seek(OAMFrameTable->at(spriteID)[0]);
		sprintf(windowText, "Processing %d\n", spriteID);
		Logger::log->LogIt(Logger::DEBUG, windowText);
		std::vector<unsigned long>& frameTable = (OAMFrameTable->at(spriteID));
		FrameManager* newFrameSet = new FrameManager(_gbaMethods,frameTable[0],spriteID, currentRomType, GBAGraphics::VRAM->SprVRAM, GBAGraphics::VRAM->SprPal);
		if (newFrameSet != NULL)
		{
			for each(Frame* f in newFrameSet->theFrames)
			{
				f->theSprite->details = mgrOAM->roomSpriteIds[spritecount].sprdetail;
			}
			RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.push_back(newFrameSet);
			sprintf(windowText, "Processed %d\n", spriteID);
			Logger::log->LogIt(Logger::DEBUG, windowText);
		}
		else
		{
			sprintf(windowText, "Failed to process %d\n", spriteID);
			Logger::log->LogIt(Logger::DEBUG, windowText);
			delete newFrameSet;
		}
	}
	LeakFinder::finder->PollHeap();
	return 0;
}

int cOAMManager::LoadSpriteToMem(bool romSwitch, GBAMethods* gba, GFXData* ginfo, sprite_entry* spriteset, unsigned char* GraphicsBuffer, TileBuffer* tb) {
	LeakFinder::finder->PollHeap();
	unsigned long addybuf = 0;
	unsigned long size = 0;
	unsigned short i = 0;
	unsigned short szCounter = 0;
	unsigned long dst = 0;
	unsigned char* decompbuf = new unsigned char[0x10000];
	unsigned char*  compBuffer = new unsigned char[64691];
	int usedGfx = 0;

	memset(&GraphicsBuffer[0x4000], 0, 0x4000);

	for (i = 0; i < 15; i++) {
		if (spriteset[i].spriteID < 0x11)
		{
			break;
		}
		unsigned char thisSprite = spriteset[i].spriteID;
		dst = 0x4000 + (ginfo[i].MemDst);
		if (dst >= 0x8000) {
			return 0;
		}
		if ((i != 0) && ginfo[i].MemDst == 0) continue;
		if (ginfo[i].RomPointer == 0)		continue;
		size=RD1Engine::theGame->titleInstance->GetGFX(thisSprite, &GraphicsBuffer[dst]);

		usedGfx += size;

	}
	

	tb->Load(GraphicsBuffer, 1024);
	delete[] decompbuf;
	delete[] compBuffer;
	LeakFinder::finder->PollHeap();
	return usedGfx;
}



//if (GlobalVars::gblVars->frameTables->FramesExist(currentSprite->id].front() == 0) return 0;
int cOAMManager::SetupPreview(GBAMethods* methods, int TitleChoice, Frame* targetFrame) {
	LeakFinder::finder->PollHeap();
	long GFXPnt = 0;
	long PalPnt = 0;
	unsigned char*  compBuffer = new unsigned char[32688];
	long addybuf = 0;
	long size = 0;
	SpriteObject*currentSprite = targetFrame->theSprite;
	int i = 0;
	int ii = 0;
	char blah[256] = { 0 };
	unsigned char *decompbuf = new unsigned char[64692];
	memset(decompbuf, 0, 64691);
	memset(compBuffer, 0, 32687);
	//	currentSprite->PreviewSprite.ClearLayers();

	
	

	GFXPnt = GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value;
	GFXPnt += (currentSprite->id - 0x10) * 4;
	PalPnt = GameConfiguration::mainCFG->GetDataContainer("SpritePal")->Value + (currentSprite->id - 0x10) * 4;
	currentSprite->palsize = RD1Engine::theGame->GetPalSize(currentSprite->id);

	if (!targetFrame->theSprite->selfInitPal)
	{
		targetFrame->theSprite->selfInitPal = true;

		unsigned short transferpal[256] = { 0 };

		int  X = 0;
		long off = 0;
		int x = 0;
		memset(GBAGraphics::VRAM->SprPal, 0, sizeof(GBAGraphics::VRAM->SprPal));


		MemFile::currentFile->seek(PalPnt);
		MemFile::currentFile->fread(&addybuf, 4, 1);
		MemFile::currentFile->seek(addybuf - 0x8000000);
	
		memset(GBAGraphics::VRAM->GBASprPal, 0, 0x200);
		MemFile::currentFile->fread(&transferpal, 1, currentSprite->palsize * 2);
		//	if(paltransfer[x][1] == 0) continue;
		memcpy(&GBAGraphics::VRAM->GBASprPal[128], &transferpal, currentSprite->palsize * 2);

		methods->DecodePal(GBAGraphics::VRAM->GBASprPal, currentSprite->PreviewPal, 16, 0);

	}

	if (!targetFrame->theSprite->selfInitGFX) {

		RD1Engine::theGame->titleInstance->GetGFX(currentSprite->id, &currentSprite->PreRAM[0x4000]);

	}
	delete[] decompbuf;
	delete[] compBuffer;
	LeakFinder::finder->PollHeap();
	if (targetFrame->frameOffset < 0x8000000) return -1;

	
	LeakFinder::finder->PollHeap();
	return 0;
}



int cOAMManager::DrawOAM()
{
	char buffer[1024] = { 0 };
	vector<FrameManager*> * theSprites = &RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites;
	int frameCounter = 0;
	for each(FrameManager* thisSprite in  *theSprites)
	{

		for each(Frame* curFrame in thisSprite->theFrames) {
			frameCounter++;
		}

	}
	sprintf(buffer, "There are %d sprites and a total of %d frames \n ", theSprites->size(), frameCounter);
	Logger::log->LogIt(Logger::DEBUG, buffer);
	int sprCounter = 0;
	frameCounter = 0;
	for each(FrameManager* thisSprite in  *theSprites)
	{


		try {


			for each(Frame* curFrame in thisSprite->theFrames) {
				if (curFrame->frameOffset == 0x082f46b2)
				{
					//GlobalVars::gblVars->OAMED = true;
				}
				sprintf(buffer, "Loading %X\n ", curFrame->frameOffset);
				Logger::log->LogIt(Logger::DEBUG, buffer);
			//	throw "OamEditor destroyed";
				//GlobalVars::gblVars->OAMED = true;
				/*	curFrame->theSprite->selfInitGFX = true;
					curFrame->theSprite->selfInitPal = true;*/
					//cOAMEdit::SetupPreview(_gbaMethods->ROM, currentRomType, curFrame);
								DecodeOAM(true, curFrame->theSprite, curFrame->frameOffset - 0x8000000);

				//GlobalVars::gblVars->OAMED = false;
				DrawPSprite(curFrame->theSprite);
				//DumpLayers();
				curFrame->frameInited = true;
				curFrame->animUpdated = true;

				/*DumpLayers();*/
				frameCounter++;
			}
			sprCounter++;
		}
		catch (char* a) { throw a; }

	}
	DumpLayers();

	//int width = 0;
	//int height = 0;
	//int partCounter = 0;
	//int Tile = 0;
	//int bTile = 0;
	//int Pal = 0;
	//int fw = 0;
	//int fh = 0;
	//int ty = 0;
	//int tx = 0;
	//int sx = 0;
	//int sy = 0;
	//int xFlip = 0;
	//int yFlip = 0;
	//RECT PartSize[128] = { 0 };
	//const unsigned char objSizes[3][4][2] =
	//{
	//	{ { 8, 8 },{ 16,16 },{ 32,32 },{ 64,64 } },
	//	{ { 16, 8 },{ 32, 8 },{ 32,16 },{ 64,32 } },
	//	{ { 8,16 },{ 8,32 },{ 16,32 },{ 32,64 } },
	//};

	//Image OamBuffer;
	//SpriteImage->Load(idkVRAM->RAM, 1023);



	//sx = 0;
	//sy = 0;

	//tx = 0;
	//ty = 0;
	//OamBuffer.Create(256, 256);
	//OamBuffer.SetPalette(GBAGraphics::VRAM->SprPal);

	//OverAllOAM *obj = NULL;
	//sprite_entry* sprite_in = &BaseGame::theGame->mgrOAM->roomSpriteIds[cursprite];
	//SpriteObjectManager* sprMgr = RD1Engine::theGame->mainRoom->mgrSpriteObjects;

	return 0;
}

