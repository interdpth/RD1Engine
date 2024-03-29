#include "SpriteObjectManager.h"

class cOAMEdit;
SpriteObjectManager::SpriteObjectManager(SpritesetData* spriteset, GBAMethods* gba, std::map<int, std::vector<unsigned long>>* OAMFrameTable, FrameManager* currentFrames, int ObjCount)
{
	_currentFrames = currentFrames;
	_OAMFrameTable = OAMFrameTable;
	_gbaMethods = gba;
	gfxpnt_dst.resize(16);
	int i = 0;
	for (i = 0; i < ObjCount; i++)
	{
		vector<MapObjectSprite*> lst;
	
		SpriteObjects.push_back(lst);
	}
}


SpriteObjectManager::~SpriteObjectManager()
{
	for each(FrameManager* mgr in RoomSprites)
	{
		delete mgr;
	}

	for each(auto o in SpriteObjects)
	{
		for each(auto s in o)
		{
			delete s;
		}
		o.clear();
	}

	RoomSprites.clear();
}

int SpriteObjectManager::ExportPal() {

	char FileName[1024] = { 0 };
	int i = 0;
	_gbaMethods->ReturnFileName(NULL, NULL, "Please select a PAL file to export too\0*.PAL", FileName, 512, 0);
	strcat(FileName, ".pal");
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "w+b");
	if (fp) {

		for (i = 0; i < _currentFrames->GetStaticFrame()->theSprite->palsize; i++)
		{
			//			fwrite(&_currentFrames->GetStaticFrame()->theSprite->PreviewPal[128 + i], 4, 1);
		}
		fclose(fp);
	}


	//size then palettes
	//FILE* fp  = fopen(
	//_currentFrames->GetStaticFrame()->theSprite->palsize
	//InvalidateRect(cOAMEdit::OamEditor->hwndPal, 0, 1);
	return 0;
}
int SpriteObjectManager::ImportGFX() {

	char FileName[512] = { 0 };
	unsigned char*  Buffer = new unsigned char[32192];
	memset(Buffer, 0, 32192);
	unsigned short i = 0;
	int sz = 0;
	delete[]  Buffer;
	return 0;
}


int SpriteObjectManager::ExportGFX() {
	//This doesn't need a size 
	char FileName[512] = { 0 };
	int i = 0;
	_gbaMethods->ReturnFileName(NULL, NULL, "Please select a GFX file to export too\0*.GFX", FileName, 512, 0);
	FILE* fp = fopen(FileName, "w+b");
	if (fp) {

		//		fwrite(&_currentFrames->GetStaticFrame()->theSprite->PreRAM[0x4000],_currentFrames->GetStaticFrame()->theSprite->graphicsize, 1);
		fclose(fp);
	}
	return 0;
}
int SpriteObjectManager::SavePal(int RomSwitch) {
	unsigned long offset = 0;
	//GFXPnt= 0x79A5D8 + (SGBSpr.id-0x10) * 4;
	//GFXPnt= 0x79A5D8 + (SGBSpr.id-0x10) * 4;



	//PalPnt= 0x79A8D4 + (SGBSpr.id-0x10) * 4;

	//SGBSpr.palsize=  ((BaseGame::theGame->mgrOAM->MFSprSize[SGBSpr.id-0x10]/2048)*16);






	//MemFile::currentFile->seek(PalPnt );
	//MemFile::currentFile->fread(&addybuf, 4, 1);
	//MemFile::currentFile->seek(addybuf-0x8000000);
	//MemFile::currentFile->fread(&transferpal, 1, (paltransfer[x].Size)*2);
	//memset( GBAGraphics::VRAM->GBASprPal,0,0x200);
	// MemFile::currentFile->fread(&transferpal, 1, SGBSpr.palsize*2);
	//	if(paltransfer[x][1] == 0) continue;
	//memcpy(& GBAGraphics::VRAM->GBASprPal[128], &transferpal, SGBSpr.palsize*2);

	_gbaMethods->EncodePal(GBAGraphics::VRAM->GBASprPal, _currentFrames->GetStaticFrame()->theSprite->PreviewPal, 16, 0);
	offset = _gbaMethods->FindFreeSpace(_currentFrames->GetStaticFrame()->theSprite->palsize * 2, 0xFF);
	MemFile::currentFile->seek(offset);
	MemFile::currentFile->fwrite(&GBAGraphics::VRAM->GBASprPal[128], 1, _currentFrames->GetStaticFrame()->theSprite->palsize * 2);
	offset += 0x8000000;

	MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("SpritePal")->Value + (_currentFrames->GetStaticFrame()->theSprite->id - 0x10) * 4);


	MemFile::currentFile->fwrite(&offset, 4, 1);
	//SGBSpr.palsize=  ((BaseGame::theGame->mgrOAM->MFSprSize[SGBSpr.id-0x10]/2048)*16);
	//BaseGame::theGame->mgrOAM->MFSprSize[(_currentFrames->GetStaticFrame()->theSprite->id-0x10)<<1]=(_currentFrames->GetStaticFrame()->theSprite->palsize*177);

	_gbaMethods->Reopen();
	return 0;
}

int SpriteObjectManager::SaveGFX(int RomSwitch) 
{
	unsigned long GFXPointer = 0;
	unsigned char*  GFXbuf = new unsigned char[32192];
	memset(GFXbuf, 0, 32192);
	unsigned long size = 0;
	int SpriteID = _currentFrames->GetStaticFrame()->theSprite->id - 0x10;
	SpriteObject* currSprite = _currentFrames->GetStaticFrame()->theSprite;
	if (RomSwitch == 0) {

		size = _gbaMethods->LZ77Comp(currSprite->graphicsize, &currSprite->PreRAM[0x4000], sizeof(currSprite->PreRAM - 0x4000), GFXbuf);
		//Just find new space 
		GFXPointer = _gbaMethods->FindFreeSpace(size, 0xFF);
		MemFile::currentFile->seek(GFXPointer);
		MemFile::currentFile->fwrite(&GFXbuf, 1, size);
		MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (SpriteID * 4));
		GFXPointer += 0x8000000;
		MemFile::currentFile->fwrite(&GFXPointer, 1, 4);
	}
	else if (RomSwitch == 1) {
		MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (SpriteID * 4));
		MemFile::currentFile->fread(&GFXPointer, 1, 4);
		GFXPointer -= 0x8000000;
		if (RD1Engine::theGame->mgrOAM->MFSprSize[(SpriteID) << 1]<currSprite->graphicsize || RD1Engine::theGame->mgrOAM->MFSprSize[(SpriteID) << 1]>currSprite->graphicsize) {
			RD1Engine::theGame->mgrOAM->MFSprSize[(SpriteID) << 1] = currSprite->graphicsize & 0xFFFF;
			MemFile::currentFile->seek(0x2E4A50);
			MemFile::currentFile->fwrite(&RD1Engine::theGame->mgrOAM->MFSprSize, 4, 0xC0);
			GFXPointer = _gbaMethods->FindFreeSpace(currSprite->graphicsize, 0xFF);

		}
		MemFile::currentFile->seek(GFXPointer);

		MemFile::currentFile->fwrite(&currSprite->PreRAM[0x4000], currSprite->graphicsize, 1);
		GFXPointer += 0x8000000;
		MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (currSprite->id - 0x10) * 4);
		MemFile::currentFile->fwrite(&GFXPointer, 1, 4);
		_gbaMethods->Reopen();
	}


	delete[] GFXbuf;


	return 0;
}


int SpriteObjectManager::SavePal(PalData* palinfo, sprite_entry* spriteset, long *Palette) {
	unsigned char max = 0;
	max = RD1Engine::theGame->mgrOAM->maxsprite;
	/*//if (GlobalVars::gblVars->SSE == true)max = SpriteSet->total;*/
	long  addybuf;


	int  x;

	SpriteSet->usedPAL = 0;
	//	memset(& GBAGraphics::VRAM->GBASprPal[128],0,sizeof( GBAGraphics::VRAM->GBASprPal)-128);
	//    memset(& GBAGraphics::VRAM->SprPal,0,sizeof( GBAGraphics::VRAM->SprPal));

	_gbaMethods->EncodePal(GBAGraphics::VRAM->GBASprPal, Palette, 16, 0);
	for (x = 0; x < max; x++) {
		if (palinfo[x].RomPointer == 0)
			continue;

		MemFile::currentFile->seek(palinfo[x].RomPointer);
		MemFile::currentFile->fread(&addybuf, 4, 1);
		MemFile::currentFile->seek(addybuf - 0x8000000);
		//	fwrite(&GBAGraphics::VRAM->GBASprPal[128 + (palinfo[x].MemDst)], 1, (palinfo[x].Size) * 2);

			//	if(paltransfer[x][1] == 0) continue;

			//	memcpy(& GBAGraphics::VRAM->GBASprPal[128+(palinfo[x].MemDst  )  ], &transferpal, (palinfo[x].Size)*2);
		SpriteSet->usedPAL += (palinfo[x].Size);
	}





	return 0;
}

void SpriteObjectManager::AddSpriteObject(int ObjectSet, int creature)
{
	nEnemies tmp;
	tmp.Creature = 0x10| (creature+1 &0xF);
	tmp.X = 5;
	tmp.Y = 5;
	SpriteObjects[ObjectSet].push_back(new MapObjectSprite(&tmp));
}

void SpriteObjectManager::DeleteSpriteObject(int ObjectSet, int ObjID)
{
	if (ObjID == -1 || SpriteObjects[ObjectSet].size() < ObjID) {//Can't delete object, doesn't exist
		return;
	}
	delete SpriteObjects[ObjectSet][ObjID];
	SpriteObjects[ObjectSet].erase(SpriteObjects[ObjectSet].begin() + ObjID);
}


int SpriteObjectManager::GetZMSetSZ(long* GFXSizes, long*PalSizes, sprite_entry* SpriteInfo) {

	long off = 0;
	int X = 0;

	//sprite_entry tempit[15]; 
	unsigned char*  comp1 = new unsigned char[64192];
	unsigned char*  comp2 = new unsigned char[64192];
	unsigned char max = 0;



	compare = 0xFF;


	for (X = 0; X < SpriteSet->spriteCount; X++) {



		if (SpriteInfo[X].sprdetail == compare)
			continue; //compare detail# to last detail#

		compare = SpriteInfo[X].sprdetail;                    //store last detail#

		if (SpriteInfo[X].sprdetail == 0x8)
			continue;      //if detail# = 8, dont load gfx & pals

						   //gfx and palette loading


		MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (SpriteInfo[X].spriteID - 0x10) * 4);
		MemFile::currentFile->fread(&off, 4, 1);
		MemFile::currentFile->seek(off - 0x8000000);
		MemFile::currentFile->fread(&PalSizes[X], 4, 1);
		PalSizes[X] = ((PalSizes[X] >> 16) & 0xF8) * 2;
		MemFile::currentFile->seek(off - 0x8000000);
		MemFile::currentFile->fread(&comp1, 1, 32191);

		GFXSizes[X] = _gbaMethods->LZ77UnComp(comp1, comp2);
		//GFXSizes[X]+=SpriteInfo[X].sprdetail*0x800;
		GFXSizes[X] = GFXSizes[X];
	}

	delete[] comp1;
	delete[] comp2;
	return 0;
}

int SpriteObjectManager::GetMFSetSZ(long* GFXSizes, long*PalSizes, sprite_entry* SpriteInfo) {

	long off = 0;
	int X = 0;

	//sprite_entry tempit[15]; 
	unsigned char prevsprdetail = 0;
	unsigned char max = 0;

	for (X = 0; X < SpriteSet->spriteCount; X++) {
		sprite_entry* sprite_in = &RD1Engine::theGame->mgrOAM->roomSpriteIds[X];

		if ((sprite_in->spriteID == 0) && (X != 0)) break;


		if ((sprite_in->sprdetail != prevsprdetail) && (sprite_in->sprdetail != 8))
		{


			GFXSizes[X] = RD1Engine::theGame->mgrOAM->MFSprSize[(SpriteInfo[X].spriteID - 0x10) << 1] + (SpriteInfo[X].sprdetail * 0x800);

			PalSizes[X] = (RD1Engine::theGame->mgrOAM->MFSprSize[(SpriteInfo[X].spriteID - 0x10) << 1] / 128) + (SpriteInfo[X].sprdetail * 16);
		}
		prevsprdetail = SpriteInfo[X].sprdetail;
	}




	return 0;
}
int SpriteObjectManager::ImportPal(int TitleChoice) {
	/*cOAMEdit* oamEditor = cOAMEdit::OamEditor;
	char FileName[1024] = { 0 };
	int i = 0;
	int howmanyexist = 0;
	int maxpals = 0;
	_gbaMethods->ReturnFileName("Please select a PAL file to export too\0*.PAL", FileName, 512);
	FILE* fp = fopen(FileName, "r+b");
	if (fp) {
		if (TitleChoice == 0)
		{
			maxpals =_currentFrames->GetStaticFrame()->theSprite->palsize;
		}
		else if (TitleChoice == 1) {
			maxpals = (RD1Engine::theGame->mgrOAM->MFSprSize[(_currentFrames->GetStaticFrame()->theSprite->id) << 1] / 2048) * 16;
		}

		MemFile::currentFile->fread(&_currentFrames->GetStaticFrame()->theSprite->PreviewPal[128], 4 * maxpals, 1);
		fclose(fp);
		RD1Engine::theGame->mgrOAM->DrawPSprite(_currentFrames->GetStaticFrame()->theSprite);
		for (i = 512; i < 1024; i++)
			oamEditor->Tiles->Draw(*_currentFrames->GetStaticFrame()->theSprite->Tiles, ((i - 512) % 32) * 8, ((i - 512) / 32) * 8, 0x8000 + i);

		InvalidateRect(oamEditor->hwndSpritePreview, 0, 1);
		InvalidateRect(oamEditor->hwndAnimationWindow, 0, 1);
		InvalidateRect(oamEditor->hwndTiles, 0, 1);
		InvalidateRect(oamEditor->hwndPal, 0, 1);

	}*/
	return 0;
}

///
///Load enemies for a room
///
int SpriteObjectManager::LoadEnemies(RHeader* roomHeader) {
	//First we dim our need varibles

	unsigned char loopcounter = 0;
	unsigned char max = 0;
	unsigned int i = 0, j = 0;
	int EnemySet = 0;
	long curroffset = 0;
	unsigned long offsetlist[3] = { roomHeader->lSpritePointer,roomHeader->lSpritePointer2,roomHeader->lSpritePointer3 };


	loopcounter = 0;
	//Code for set 1



	for (i = 0; i < SpriteObjects.size(); i++) {
		MemFile::currentFile->seek(offsetlist[i] - 0x8000000);
		for (size_t i = 0; i < RoomSprites.size(); i++)
		{
			delete RoomSprites[i];
		}
		SpriteObjects[i].clear();//Ayyyy
		max = 0;
		for (j = 0; j < 0x20; j++) {

			nEnemies tmp;

			tmp.Y = MemFile::currentFile->fgetc();
			tmp.X = MemFile::currentFile->fgetc();
			tmp.Creature = MemFile::currentFile->fgetc();



			if ((tmp.Creature == 0xFF)) {
				break;
			}
			SpriteObjects.at(i).push_back(new MapObjectSprite(&tmp));
		}

	}


	return 0;
}
void DumpLayers()
{
	return;
	vector<FrameManager*> * theSprites = &RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites;
	int sprCounter = 0;

	{
		for (unsigned int sprCounter = 0; sprCounter < theSprites->size(); sprCounter++)
		{
			FrameManager*thisSprite = theSprites->at(sprCounter);
			thisSprite->SetStaticFrame(0);
			for (unsigned int frameCounter = 0; frameCounter < thisSprite->theFrames.size(); frameCounter++)
			{
				thisSprite->SetStaticFrame(frameCounter);
				Frame* curFrame = thisSprite->GetStaticFrame();
				//				curFrame->theSprite->PreviewSprite.RefreshImage();

				char * string = new char[MAX_PATH];
				sprintf(string, "C:\\ReleaseFolder\\dumplayer%d_%d.bmp", sprCounter, frameCounter);
				DeleteFile(string);
				FILE* fp = nullptr;
					fopen_s(&fp, string, "w+b");

				//	curFrame->theSprite->PreviewSprite._fullCache->SaveToFile(fp);
				fclose(fp);
				delete[] string;


			}


		}
	}
}
//GetDlgItem(Main, chkHS)
int SpriteObjectManager::ShowSprites(bool show, unsigned char Number, BackBuffer* drawBuffer) {

	int i = 0;
	int relX = 0;
	int relY = 0;
	int tSprite = 0;
	int d = 0;
	RECT myrect;
	HBRUSH curbrush;
	if (SpriteObjects.size() == 0) return 0;
	//RECT blah = {0,0,0,0};
	vector<MapObjectSprite*>* Sprites = &SpriteObjects[Number];
	Frame* tmpFrame = NULL;

	DumpLayers();
	int m = Sprites->size();
	for (i = 0; i < m; i++) {

		//Draw a rect around the sprite for distiction
		bool badFrame = 0;
		//	if(BaseGame::theGame->mgrOAM->SpriteOAM[BaseGame::theGame->mgrOAM->sprite_in[(Sprites.Enemies[i].Creature & 0xF)-1].sprtype]==0) continue;
		tSprite = ((Sprites->at(i)->Creature) & 0xF) - 1;
		if (tSprite > RD1Engine::theGame->mainRoom->mgrSpriteObjects->RoomSprites.size())
		{
			continue;
		}
		if (tSprite < 0)
		{
			tSprite = 0;
		}
		//Draw a rectangle
		sprite_entry* sprite_in = &RD1Engine::theGame->mgrOAM->roomSpriteIds[tSprite];
		if (RoomSprites.size() == 0) { return 0; }
		int SpriteWidth;
		int SpriteHeight;
		/*badFrame = RoomSprites.size() < tSprite;
		if (badFrame)
		{*/
		if (tSprite > RoomSprites.size() - 1)
		{
			continue;
		}
		tmpFrame = RoomSprites.at(tSprite)->GetAnimatedFrame();
		SpriteWidth = ((tmpFrame->theSprite->Borders.right - tmpFrame->theSprite->Borders.left));
		SpriteHeight = ((tmpFrame->theSprite->Borders.bottom - tmpFrame->theSprite->Borders.top));

		if (SpriteWidth == 0 || SpriteHeight == 0)
		{
			return 0;
		}
		//SpriteWidth = (SpriteWidth / 16) * 16;
		//SpriteHeight = (SpriteWidth / 16) * 16;
		if (i == 1) {
			i = i - 1;
			i++;
		}
		//}
		int SpriteX = (Sprites->at(i)->XPosition()) * 16;; //((Sprites->at(i).X) - ((SpriteWidth/8)*8) / 16) * 16;
		int SpriteY = (Sprites->at(i)->YPosition()) * 16;//(Sprites->at(i).Y - ((SpriteHeight / 8) * 8) / 16) * 16;
		PosModify* modifer = &RD1Engine::theGame->poseModifier[sprite_in->spriteID];

		if (modifer)
		{
			SpriteX += modifer->x;
			SpriteY += modifer->y;
		}

		_OAMFrameTable = &RD1Engine::theGame->frameTables->OAMFrameTable;
		std::vector<unsigned long>* thisGuy = &_OAMFrameTable->at(sprite_in->spriteID);
		if (!(thisGuy->size() > 0)) {

			continue;
		}

		if (thisGuy->size() == 0)
		{
			continue;
		}
		if (thisGuy->size() && !badFrame)
		{


			int adjustedXorigin = 0;
			int adjustedYorigin = 0;
			if (tmpFrame->theSprite->Borders.left < 0)
			{
				adjustedXorigin = 0 - tmpFrame->theSprite->Borders.left;
			}

			if (tmpFrame->theSprite->Borders.top < 0)
			{
				adjustedYorigin = 0 - tmpFrame->theSprite->Borders.top;

			}

			//If sprite doesn't exist draw nothing but the S
			if (show) {
				curbrush = CreateSolidBrush(RGB(255, 0, 0));
				myrect.left = SpriteX - adjustedXorigin + 8;
				myrect.top = SpriteY - adjustedYorigin + 16;
				myrect.right = (SpriteWidth)+myrect.left;
				myrect.bottom = myrect.top + SpriteHeight;
				FrameRect(drawBuffer->DC(), &myrect, curbrush);
				DeleteObject(curbrush);
			}

			try
			{

				tmpFrame->theSprite->PreviewSprite.GetFullImage()->TransBlit(

					drawBuffer->DC(),
					SpriteX - adjustedXorigin + 8,

					SpriteY - adjustedYorigin + 16,
					256,
					256,
					0,
					0);


			}
			catch (...)
			{
				Logger::log->LogIt(Logger::DEBUG, "ay");
			}


		}
		else {
			if (show) {
				myrect.left = SpriteX;
				myrect.top = SpriteY;
				myrect.right = myrect.left + 16;
				myrect.bottom = myrect.top + 16;
				curbrush = CreateSolidBrush(RGB(255, 255, 255));
				FrameRect(drawBuffer->DC(), &myrect, curbrush);
				DeleteObject(curbrush);
				TextOut(drawBuffer->DC(), SpriteX + 4,
					SpriteY, "S", 1);
			}
		}
	}
	return 0;
}



int SpriteObjectManager::SaveSprites(RHeader* roomHeader) {
	size_t i = 0;
	MemFile* curFile = MemFile::currentFile;
	//logic
	unsigned long pnterArray[3] = { roomHeader->lSpritePointer - 0x8000000 ,  roomHeader->lSpritePointer2 - 0x8000000,   roomHeader->lSpritePointer3 - 0x8000000 };


	for (i = 0; i < SpriteObjects.size(); i++) {
		curFile->seek(pnterArray[i]);

		size_t j = 0;
		for (j = 0; j < SpriteObjects[i].size(); j++)
		{
			int y = SpriteObjects[i][j]->YPosition();
			int x = SpriteObjects[i][j]->YPosition();
			curFile->fwrite(&y, 1, 1);
			curFile->fwrite(&x, 1, 1);
			curFile->fwrite(&SpriteObjects[i][j]->Creature, 1, 1);

		}
		curFile->fputc(0xFF);
		curFile->fputc(0xFF);
		curFile->fputc(0xFF);
	}

	fclose(_gbaMethods->ROM);
	fopen_s(&_gbaMethods->ROM, _gbaMethods->FileLoc, "r+b");

	return 0;
}
