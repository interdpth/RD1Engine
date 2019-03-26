#include "RoomClass.h"
int RoomClass::LoadData() {
	mapMgr->Blank();
	if (roomHeader.bBg0) GetLayerData(roomHeader.bBg0, 0, roomHeader.lForeground - 0x8000000);
	if (roomHeader.bBg1) GetLayerData(roomHeader.bBg1, 1, roomHeader.lLevelData - 0x8000000);
	if (roomHeader.bBg2) GetLayerData(roomHeader.bBg2, 2, roomHeader.lBackLayer - 0x8000000);

	if (roomHeader.lBg3 == 0x40) {

		GetLayerData(roomHeader.lBg3, 3, roomHeader.lBackgroundTSA - 0x8000000);
	}
	else if ((roomHeader.lBg3 & 0xF0) == 0xF0) {
		/*  BaseGame::theGame->mgrTileset->GetCBG();*/
	}
	GetLayerData(0x10, 4, roomHeader.lClipData - 0x8000000);
	return 0;
}


void RoomClass::LoadUpSprites(int SpriteSetIndex, TileBuffer *     SpriteImage) {
	
	FILE           *fp = NULL;
	int             sprch = 0; 
	char            cBuf[1024] = { 0 };
	int             i = 0;
	int             loadit = 0;
	sprite_entry* sprite_in = RD1Engine::theGame->mgrOAM->roomSpriteIds;
	switch (SpriteSetIndex)
	{
	case 0:
		sprch = roomHeader.bSpriteIndex1;
		if (roomHeader.bSpriteIndex1)
			loadit = 1;
		break;
	case 1:
		sprch = roomHeader.bSpriteIndex2;
		if (roomHeader.bSpriteIndex2)
			loadit = 1;
		break;
	case 2:
		sprch = roomHeader.bSpriteIndex3;
		if (roomHeader.bSpriteIndex3)
			loadit = 1;
		break;

	}
	//UiState::stateManager->UiState::stateManager->UpdateMapObjectWindow();
	if (loadit)
	{
		SpriteObjectManager* mgrSpriteObj = mgrSpriteObjects;
		cOAMManager* mgrOAM = RD1Engine::theGame->mgrOAM;
		if (currentRomType == 0)
		{
			mgrEntities->LoadSet(currentRomType,mgrSpriteObj->gfxpnt_dst, mgrSpriteObj->paltransfer, sprite_in, sprch);
		}
		else 
		if (currentRomType == 1)
		{
		/*	GlobalVars::gblVars->ReadObjectDetailsFromROM = true;
			GlobalVars::gblVars->SSE = true;*/
			mgrEntities->MFLoadSet(currentRomType,mgrSpriteObj->gfxpnt_dst, mgrSpriteObj->paltransfer, sprite_in, sprch);
		}
		mgrEntities->LoadPal(mgrSpriteObj->paltransfer, mgrOAM->roomSpriteIds, GBAGraphics::VRAM->SprPal);
		cOAMManager::LoadSpriteToMem(currentRomType,_gbaMethods, mgrSpriteObj->gfxpnt_dst, mgrOAM->roomSpriteIds, RD1Engine::theGame->idkVRAM.RAM, SpriteImage);

		RD1Engine::theGame->mgrOAM->LoadRoomOAM();

		for (i = 0; i < mgrOAM->maxsprite; i++)
		{
		RD1Engine::theGame->mgrOAM->DrawOAM();
		}
		//DumpLayers();
		
		
	}
}

int RoomClass::LoadHeader(long Offset) {//Needs to be seperate
	long blah = 0;
	int BGi = 0;
	MemFile::currentFile->seek(Offset);
	MemFile::currentFile->fread(&roomHeader.bTileset, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bBg0, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bBg1, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bBg2, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.lBg3, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.lForeground, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.lLevelData, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.lBackLayer, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.lClipData, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.lBackgroundTSA, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.bUnknown1, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.TransForeground, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.iSep1, sizeof(unsigned short), 1);
	MemFile::currentFile->fread(&roomHeader.lSpritePointer, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.bSpriteIndex1, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bEventSwitch, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.iSep2, sizeof(unsigned short), 1);
	MemFile::currentFile->fread(&roomHeader.lSpritePointer2, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.bSpriteIndex2, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bEventSwitch2, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.iSeperator, sizeof(unsigned short), 1);
	MemFile::currentFile->fread(&roomHeader.lSpritePointer3, sizeof(unsigned long), 1);
	MemFile::currentFile->fread(&roomHeader.bSpriteIndex3, sizeof(unsigned char), 1);
	//blah = ftell((FILE*)NULL) - Offset;
	MemFile::currentFile->fread(&roomHeader.bMiniMapRoomX, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bMiniMapRoomY, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bEffect, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bSceneryYPos, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bNothing, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.bMusic, sizeof(unsigned char), 1);
	MemFile::currentFile->fread(&roomHeader.blank, sizeof(unsigned char), 1);


	//	SetSceneControls();
	return 0;
}

int RoomClass::GetLayerData(unsigned char compression, unsigned char Layer, unsigned long offset) {
	unsigned char*  compBuffer = new unsigned char[64691];
	memset(compBuffer, 0, 64691);

	nMapBuffer* ELayer[5] = { mapMgr->GetLayer(MapManager::ForeGround),mapMgr->GetLayer(MapManager::LevelData),mapMgr->GetLayer(MapManager::Backlayer),mapMgr->GetLayer(MapManager::BackgroundLayer),mapMgr->GetLayer(MapManager::Clipdata) };//Edit Layer
	MemFile::currentFile->seek(offset);
	if (compression == 0x10) {
		MemFile::currentFile->fread(&ELayer[Layer]->X, sizeof(unsigned char), 1);		//BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X++;		// DISCH -- added inc
		MemFile::currentFile->fread(&ELayer[Layer]->Y, sizeof(unsigned char), 1);		//BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y++;		// DISCH -- added inc
																			// sprintf(buffer,"X is %X Y is %X",BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X,BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y);
																			// sMessage(buffer);
		MemFile::currentFile->fread(compBuffer, sizeof(unsigned char), 64691);
		ELayer[Layer]->Dirty = true;
		//_gbaMethods->ZMUncompRle(compBuffer, (unsigned char*)BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->TileBuf2D ,BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X*BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y, 1);

		if (ELayer[Layer]->BImage != NULL)
		{
			delete ELayer[Layer]->BImage;
		}

		ELayer[Layer]->BImage = new Image(ELayer[Layer]->X * 16, ELayer[Layer]->Y * 16);
		ELayer[Layer]->BImage->Create(ELayer[Layer]->X * 16, ELayer[Layer]->Y * 16);
		ELayer[Layer]->BImage->SetPalette(GBAGraphics::VRAM->PcPalMem);
		if (ELayer[Layer]->TileBuf2D)
		{
			delete ELayer[Layer]->TileBuf2D;
		}
		ELayer[Layer]->TileBuf2D = new unsigned short[ELayer[Layer]->X * ELayer[Layer]->Y];
		memset(ELayer[Layer]->TileBuf2D, 0, 2 * ELayer[Layer]->X * ELayer[Layer]->Y);
		ELayer[Layer]->oSize = _gbaMethods->UncompRle(ELayer[Layer]->X*ELayer[Layer]->Y, compBuffer, (unsigned char*)ELayer[Layer]->TileBuf2D, &ELayer[Layer]->ThisCompsize);
	}
	else if (compression == 0x40) {
		MemFile::currentFile->seek(offset);
		MemFile::currentFile->fread(&ELayer[Layer]->size, 4, 1);
		ELayer[Layer]->Dirty = true;
		if ((ELayer[Layer]->size == 1) || (ELayer[Layer]->size >= 3)) {
			ELayer[Layer]->X = 64;
			ELayer[Layer]->Y = 32;
		}
		else if (mapMgr->GetLayer(MapManager::BackgroundLayer)->size == 2) {
			ELayer[Layer]->X = 32;
			ELayer[Layer]->Y = 64;
		}
		else {

			ELayer[Layer]->X = 32;
			ELayer[Layer]->Y = 32;
		}


		MemFile::currentFile->seek(offset + 4);
		MemFile::currentFile->fread(compBuffer, sizeof(unsigned char), 32691);

		if (ELayer[Layer]->TileBuf2D)
		{
			delete ELayer[Layer]->TileBuf2D;
		}
		
		ELayer[Layer]->TileBuf2D = new unsigned short[4096];
		memset(ELayer[Layer]->TileBuf2D, 0, 2 * ELayer[Layer]->X * ELayer[Layer]->Y);

		ELayer[Layer]->oSize = _gbaMethods->LZ77UnComp(compBuffer, (unsigned char*)ELayer[Layer]->TileBuf2D);
		if (ELayer[Layer]->BImage != NULL)
		{
			delete ELayer[Layer]->BImage;
		}

		ELayer[Layer]->BImage = new Image(1024, 1024);
		ELayer[Layer]->BImage->Create(1024, 1024);
		ELayer[Layer]->BImage->SetPalette(GBAGraphics::VRAM->PcPalMem);
	}
	delete[] compBuffer;
	return 0;
}


void RoomClass::Load(Image* tilesetsrc, int area, int room, unsigned long offset )
{
	//int area = comboArea.GetListIndex;
	//int room = comboRoom.GetListIndex();

	LoadHeader(offset);
	
	int load = RD1Engine::theGame->mgrTileset->GetTileset(tilesetsrc, area, roomHeader.bTileset, roomHeader.lBg3);
	
	LoadData();
	RD1Engine::theGame->mgrDoors->LoadDoors(room);

	mgrSpriteObjects->LoadEnemies(&roomHeader);

	RD1Engine::theGame->mgrScrolls->initScroll( area, room);
	RD1Engine::theGame->mgrScrolls->GetScroll();
	LeakFinder::finder->LogActiveLeaks(Logger::log);
}


RoomClass::RoomClass(int romType, Image* tilsetsrc, SpritesetData* spriteset, GBAMethods* gba, std::map<int, std::vector<unsigned long>>* OAMFrameTable, FrameManager* currentFrames, int area, int room, unsigned long offset)
{
	currentRomType = romType;
	_gbaMethods = gba;
	mgrEntities = new cEntityManager(_gbaMethods);
	mapMgr = new MapManager();
	mgrSpriteObjects = new SpriteObjectManager(spriteset, gba, OAMFrameTable, currentFrames,1);
	memset(&roomHeader, 0, sizeof(RHeader));
	Area = area;
	Room = room;
	Load(tilsetsrc, area, room, offset);
}


RoomClass::RoomClass(int romType, Image* tilsetsrc, SpritesetData* spriteset, GBAMethods* gba, std::map<int, std::vector<unsigned long>>* OAMFrameTable, FrameManager* currentFrames)
{
	currentRomType = romType;
	_gbaMethods = gba;
	mgrEntities = new cEntityManager(_gbaMethods);
	mapMgr = new MapManager();	
	mgrSpriteObjects = new SpriteObjectManager(spriteset,gba, OAMFrameTable, currentFrames, 1);
	memset(&roomHeader, 0, sizeof(RHeader));
}


RoomClass::~RoomClass()
{
	delete mgrSpriteObjects;
	delete mapMgr;
	
	delete mgrEntities;
}
