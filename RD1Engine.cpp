#include "BaseGame.h"
 int GetBitField(long reg, long bitStart, long bitLength) {
	return	(reg / (1 << bitStart)) & ((1 << bitLength) - 1);
}
RD1Engine* RD1Engine::theGame;
int  DrawRect(HDC hdc, long theColor, RECT* mpointer, int mul)
{
	RECT            myrect;
	HBRUSH          curbrush = CreateSolidBrush(theColor);

	myrect.left = mpointer->left * mul;
	myrect.top = mpointer->top* mul;
	myrect.right = mpointer->right * mul + myrect.left;
	myrect.bottom = myrect.top + mpointer->bottom * mul;
	FrameRect(hdc, &myrect, curbrush);
	DeleteObject(curbrush);
	return 0;
}
#include "FusionSamus.h"
void RD1Engine::LoadRoom(int area, int room, int spriteindex)
{
	mgrDoors->SetupDoors(area);
	DrawStatus.dirty = true;
	DrawStatus.BG0 = true;
	DrawStatus.BG1 = true;
	DrawStatus.BG2 = true;
	DrawStatus.BG3 = true;
	DrawStatus.Doors = true;
	DrawStatus.Sprites = true;
	FusionSamus* mf = new FusionSamus();
	mf->Logic();
	mf->Load();
	if (mainRoom->mapMgr->GetLayer(MapManager::ForeGround)) {
		mainRoom->mapMgr->GetLayer(MapManager::ForeGround)->Dirty = 1;
	}
	if (mainRoom->mapMgr->GetLayer(MapManager::LevelData))
	{
		mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Dirty = 1;
	}
	if (mainRoom->mapMgr->GetLayer(MapManager::Backlayer))
	{
		mainRoom->mapMgr->GetLayer(MapManager::Backlayer)->Dirty = 1;
	}

//	LoadingLevel = 1;
	unsigned long offset = 0;
	char buffer[256];
	sprintf(buffer, "Loading Area: %d Room: %x, Offset %x", area, room, offset);
	Logger::log->LogIt(Logger::DEBUG, buffer);
	offset = (RoomOffsets[area] - 0x8000000) + (room * 0x3C);
	LoadRoom(area, room, offset,(FILE*)NULL);
	////Change to room header 
	mainRoom->LoadUpSprites(spriteindex);

}

void RD1Engine::LoadRoom(int area, int room, unsigned long offset, FILE* fp)
{
	char buffer[256];
	sprintf(buffer, "Loading Area: %d Room: %x, Offset %x", area, room, offset);
	Logger::log->LogIt(Logger::DEBUG, buffer);
	if (mainRoom)
	{
		delete mainRoom;
		mainRoom = NULL;
	}

	mainRoom = new RoomClass(currentRomType, NULL, (SpritesetData*) NULL, _gbaMethods,  (std::map<int, std::vector<unsigned long>>*)NULL, (FrameManager*) NULL,  area, room, offset, fp);
}

int RD1Engine::GetPalSize(int sprID)
{
	int palSize = 0;
	switch (thisTitle)
	{
	case titleMF:
		palSize = fusionInstance->GetPalSize(sprID);
		break;
	case titleZM:
		palSize = zmInstance->GetPalSize(sprID);
		break;
	default:

		break;
	}
	return palSize;
}

void RD1Engine::GetArrays()
{
	int i = 0;
	/*
	Get FileNo, HeaderPointers + 1, RoomOffsets
	Get FileNo, DoorPointers + 1, DoorOffsets
	Get FileNo, SpecialEffectTablePnt + 1, SpecialEffects
	Get FileNo, MiniMapBanks + 1, ZMMiniMapOffsets()
	Get FileNo, ZMScrollListBase + 1, ZMScrolls()
	MFTileSetHeaders = &H3BF888
	MFHeaderPointers = &H79B8BC
	MFLookUpTable = &H3C8F90
	ReDim DoorOffsets(0 To 9) As Long
	DoorPointers = &H79B894
	MFSpecialEffectTablePnt = &H3C8D58
	MFScrollListBase = &H79BB08
	Enableit*/

	RD1Engine::theGame->mgrTileset->ReadTable();


	//MemFile::currentFile->seek(0x79BB08);
	//for(i = 0; i < 0xB;i++) MemFile::currentFile->fread(&MFScrolls[i],4,1,_gbaMethods->ROM);

	//MemFile::currentFile->fread(BaseGame::theGame->RoomOffsets, 4, 10,(FILE*)NULL);
	if (RD1Engine::theGame->thisTitle == SupportedTitles::titleMF)
	{
		RD1Engine::theGame->fusionInstance->LoadGameData(_gbaMethods->ROM);
		MemFile::currentFile->seek(0x3e419c);
		MemFile::currentFile->fread(&idkVRAM.RAM[0x900], 1, 0x36E0,(FILE*)NULL);
		MemFile::currentFile->seek(0x58b466);
		MemFile::currentFile->fread(&GBAGraphics::VRAM->GBASprPal[0x40], 1, 0x14,(FILE*)NULL);
		MemFile::currentFile->seek(0x3E40F2);
		MemFile::currentFile->fread(&GBAGraphics::VRAM->GBASprPal[0x56], 1, 0x3E,(FILE*)NULL);
	}

}

int RD1Engine::LoadAreaTable()
{
	DataContainer* areas = GameConfiguration::mainCFG->GetDataContainer("Areas");
	MemFile::currentFile->seek(areas->Value);
	for (int i = 0; i < areas->MemberCount; i++)
	{
		unsigned long gamePointer = 0;
		MemFile::currentFile->fread(&gamePointer, sizeof(long), 1,(FILE*)NULL);
		RoomOffsets.push_back(gamePointer);
	}
	return areas->MemberCount;
}


RD1Engine::RD1Engine(SupportedTitles theTitle, std::map<int, std::vector<unsigned long>>* _oAMFrameTable, TileBuffer * bg, TileBuffer* TileImage, Image* ImageTileset)
{
	ThisBackBuffer.Create(1024, 1024);
	_theLog = Logger::log;
	_gbaMethods = new GBAMethods();
	mgrDoors = new DoorManager(_gbaMethods);
	mgrTileset = new TilesetManager(_gbaMethods, currentRomType, bg, TileImage);
	mgrScrolls = new clsRoomScrolls();
	mgrOAM = new cOAMManager(_oAMFrameTable, _gbaMethods,(int)theTitle);

	fusionInstance = NULL;
	zmInstance = NULL;
	this->thisTitle = theTitle;
	switch (theTitle)
	{
	case SupportedTitles::titleMF:
		fusionInstance = new MetroidFusion();

		mgrOAM->MFSprSize = fusionInstance->MFSprSize;
		LoadModifiers((char*)fusionInstance->PoseFile);
		_theLog->LogIt(Logger::LOGTYPE::DEBUG, "LOADED MF");
		break;
	case SupportedTitles::titleZM:
		zmInstance = new ZeroMission();
		LoadModifiers((char*)zmInstance->PoseFile);
		_theLog->LogIt(Logger::LOGTYPE::DEBUG, "LOADED ZM");
		break;
	case SupportedTitles::titleWL:
		_theLog->LogIt(Logger::LOGTYPE::DEBUG, "LOADED WARIO LAND");
		break;

	case SupportedTitles::titleUnsupport:
	default:
		_theLog->LogIt(Logger::LOGTYPE::ERRORZ, "UNSUPPORTED TITLE, THROWING ERROR");
		//throw "UNSUPPORTED TITLE";
		break;

	}

	mainRoom = new RoomClass((int)theTitle,ImageTileset,NULL,_gbaMethods,  _oAMFrameTable, NULL );
}
void RD1Engine::LoadModifiers(char* fn)
{
	FILE* fp = fopen(fn, "r");
	long eoff = 0;
	char* buf = new char[256];
	int id = 0;
	int xpos = 0;
	int ypos = 0;
	if (fp) {
		//fclose(fp);
		fseek(fp, 0, SEEK_END);
		eoff = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		while (ftell(fp) < eoff) {
			memset(buf, 0, 256);

			fgets(buf, 256, fp);
			id = 0;
			xpos = 0;
			ypos = 0;
			//int blah=strlen(buf);
			//buf[strlen(buf)-2]=0;
			//buf[strlen(buf)-1]=0;
			sscanf(buf, "%X=%d,%d", &id, &xpos, &ypos);
			//memcpy(&table[Entry], charbuf, strlen(charbuf));
			PosModify tmp;
			tmp.x = xpos;
			tmp.y = ypos;

			poseModifier[id] = tmp;
		}
	}
	delete[] buf;

}
RD1Engine::~RD1Engine()
{
	delete mgrDoors;
	delete mgrTileset;
	delete mgrOAM;
	delete mgrScrolls;
	if (fusionInstance)
	{
		delete fusionInstance;
	}

	if (zmInstance)
	{
		delete zmInstance;
	}
}

void RD1Engine::DrawSprites(Image* pic) {
	int i = 0;
	int relX = 0;
	int relY = 0;
	int tSprite = 0;
	int d = 0;
	RECT myrect;
	HBRUSH curbrush;

	//RECT blah = {0,0,0,0};
	nEnemyList* Sprites = &mainRoom->mgrSpriteObjects->SpriteObjects[0];
	Frame* tmpFrame = NULL;


	for (i = 0; i < Sprites->Max(); i++) {

		//Draw a rect around the sprite for distiction
		bool badFrame = 0;
		//	if(BaseGame::theGame->mgrOAM->SpriteOAM[BaseGame::theGame->mgrOAM->sprite_in[(Sprites.Enemies[i].Creature & 0xF)-1].sprtype]==0) continue;
		tSprite = ((Sprites->Enemies[i].Creature) & 0xF) - 1;
		if (tSprite >= mainRoom->mgrSpriteObjects->RoomSprites.size())
		{
			continue;
		}
		if (tSprite < 0)
		{
			tSprite = 0;
		}
		//Draw a rectangle
		sprite_entry* sprite_in = &mgrOAM->roomSpriteIds[tSprite];
		if (mainRoom->mgrSpriteObjects->RoomSprites.size() == 0) { return; }
		int SpriteWidth;
		int SpriteHeight;
		/*badFrame = RoomSprites.size() < tSprite;
		if (badFrame)
		{*/
		std::vector<FrameManager*> fml = mainRoom->mgrSpriteObjects->RoomSprites;
		int count = fml.size();
		if (tSprite >= count) continue;
		FrameManager * fm = fml.at(tSprite);
		tmpFrame = fm->GetAnimatedFrame();
		SpriteWidth = ((tmpFrame->theSprite->Borders.right - tmpFrame->theSprite->Borders.left));
		SpriteHeight = ((tmpFrame->theSprite->Borders.bottom - tmpFrame->theSprite->Borders.top));

		if (SpriteWidth == 0 || SpriteHeight == 0)
		{
			return;
		}
		//SpriteWidth = (SpriteWidth / 16) * 16;
		//SpriteHeight = (SpriteWidth / 16) * 16;
		if (i == 1) {
			i = i - 1;
			i++;
		}
		//}
		int SpriteX = (Sprites->Enemies[i].X) * 16;; //((Sprites->Enemies[i].X) - ((SpriteWidth/8)*8) / 16) * 16;
		int SpriteY = (Sprites->Enemies[i].Y) * 16;//(Sprites->Enemies[i].Y - ((SpriteHeight / 8) * 8) / 16) * 16;
		PosModify* modifer = &RD1Engine::theGame->poseModifier[sprite_in->spriteID];

		if (modifer)
		{
			SpriteX += modifer->x;
			SpriteY += modifer->y;
		}

		if (frameTables->OAMFrameTable[sprite_in->spriteID].size() == 0)
		{
			continue;
		}
		if (frameTables->OAMFrameTable[sprite_in->spriteID].front() && !badFrame)
		{
			//If sprite doesn't exist draw nothing but the S
			if (true) {
				curbrush = CreateSolidBrush(RGB(255, 0, 0));
				myrect.left = SpriteX;
				myrect.top = SpriteY;
				myrect.right = (SpriteWidth)+myrect.left;
				myrect.bottom = myrect.top + SpriteHeight;
				FrameRect(pic->DC(), &myrect, curbrush);
				DeleteObject(curbrush);
			}

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
			clsPreviewSprite* curSprite = &tmpFrame->theSprite->PreviewSprite;
			if (curSprite == NULL)
			{
				Logger::log->LogIt(Logger::ERRORZ, "cur sprite is null");
				continue;
			}
			Image* targetImage = curSprite->GetFullImage();
			if (targetImage == NULL)
			{
				Logger::log->LogIt(Logger::ERRORZ, "Target image is null");
				continue;
			}
			targetImage->TransBlit(

				pic->DC(),
				SpriteX - adjustedXorigin + 8,

				SpriteY - adjustedYorigin + 16,
				256,
				256,
				0,
				0);



		}
		else {
			if (true) {
				myrect.left = SpriteX;
				myrect.top = SpriteY;
				myrect.right = myrect.left + 16;
				myrect.bottom = myrect.top + 16;
				curbrush = CreateSolidBrush(RGB(255, 255, 255));
				FrameRect(pic->DC(), &myrect, curbrush);
				DeleteObject(curbrush);
				TextOut(pic->DC(), SpriteX + 4,
					SpriteY, "S", 1);
			}
		}
	}
	return;
}



//RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED);



int RD1Engine::DrawRoom(TileBuffer* TileImage, TileBuffer* BGImage, bool DrawBackLayer, bool DrawLevelLayer, bool DrawForeground, bool HideSprites, bool ShowScrolls, bool ShowClip, int ScrollIndex) {

	unsigned char cSize = 0;
	unsigned char fd = 0;
	unsigned long fWidth = 0, fHeight = 0;
	//int Width=GlobalVars::gblVars->imgMap->Width,Height=GlobalVars::gblVars->imgMap->Height;
	int k = 0;
	MapManager* mgr = mainRoom->mapMgr;
	if (!mgr->created)
	{
		//Logger::log->LogIt(Logger::LOGTYPE::ERRORZ, "ROOM NOT LOADED");
		return -1;
	}
	nMapBuffer* buffForeground = mgr->GetLayer(MapManager::ForeGround);
	nMapBuffer* buffLevelData = mgr->GetLayer(MapManager::LevelData);
	nMapBuffer* buffBackLayer = mgr->GetLayer(MapManager::Backlayer);
	nMapBuffer* buffBackground = mgr->GetLayer(MapManager::BackgroundLayer);
	unsigned short Width = buffLevelData->X << 4, Height = buffLevelData->Y << 4;


	if (!((currentRomType == 0) || (currentRomType == 1))) return 0;

	if (!DrawStatus.dirty)
	{
		Logger::log->LogIt(Logger::ERRORZ, "WASTED DRAW ROOM");
		return 0;
	}
	//Some setup
	TileImage->Load(GBAGraphics::VRAM->fGbuf, 1024);
	BGImage->Load(GBAGraphics::VRAM->BGBuf, 1024);
	ThisBackBuffer.Clear();
	SetBkMode(ThisBackBuffer.DC(), TRANSPARENT);
	SetTextColor(ThisBackBuffer.DC(), RGB(255, 255, 255));

	//Setup rendermap
	Image imgMap;
	imgMap.Create(Width, Height);
	imgMap.SetPalette(GBAGraphics::VRAM->PcPalMem);

	if (mainRoom->roomHeader.lBg3 & 0x40) {
		DrawBackGround(&imgMap, BGImage);
		imgMap.Blit(ThisBackBuffer.DC(), 0, 0, Width, Height, 0, 0);
	}

	imgMap.Clear();

	if (DrawStatus.BG2) {

		DrawLayer(buffBackLayer, &imgMap, mainRoom->roomHeader.bBg2);//Backlayer
		imgMap.TransBlit(ThisBackBuffer.DC(), 0, 0, Width, Height, 0, 0);
		buffBackLayer->Dirty = 0;

	}

	imgMap.Clear();
	if (DrawStatus.BG1) {

		int
			alphaHigh = 16;
		int alphaLow = 0;
		DrawLayer(buffLevelData, &imgMap, mainRoom->roomHeader.bBg1);

		imgMap.TransBlit(ThisBackBuffer.DC(), 0, 0, Width, Height, 0, 0);


		buffLevelData->Dirty = 0;

	}
	imgMap.Clear();
	bool val = HideSprites;; //SendMessage(GetDlgItem(GlobalVars::gblVars->frameControls, chkHS), BM_GETCHECK, 0, 0) == 0;

	DrawSprites(&imgMap);//Foreground
	mainRoom->mgrSpriteObjects->ShowSprites(val, 0, &ThisBackBuffer);


	imgMap.Clear();

	if (DrawStatus.BG0) {

		DrawLayer(buffForeground, &imgMap, mainRoom->roomHeader.bBg0);//Foreground

		if (mainRoom->roomHeader.TransForeground > 0)
		{
			BLENDFUNCTION bfn = { 0 }; // Struct with info for AlphaBlend
			bfn.BlendOp = AC_SRC_OVER;
			bfn.BlendFlags = 0;
			bfn.SourceConstantAlpha = 150;;//Need to unhard code
			bfn.AlphaFormat = 0;// AC_SRC_ALPHA;
			AlphaBlend(ThisBackBuffer.DC(), 0, 0, Width, Height, imgMap.DC(), 0, 0, Width, Height, bfn); // Display bitmap
			//	imgMap.AlphaBlit(ThisBackBuffer.DC(), 0, 0, Width, Height, 0, 0, alphaLow, alphaHigh);
		}
		else {
			imgMap.TransBlit(ThisBackBuffer.DC(), 0, 0xFF - mainRoom->roomHeader.bSceneryYPos, Width, Height, 0, 0);
		}

		buffForeground->Dirty = 0;

	}



	buffForeground->Dirty = 0;

	mgrDoors->DisplayDoors(mainRoom->Room);

	if (ShowClip)//GlobalVars::gblVars->ViewClip.value() == 1)
	{
		//ViewClip.value(0);     
		//move to ui?
		//DrawClipIdent();
	}

	if (ShowScrolls && !ShowClip){//(GlobalVars::gblVars->ScrollCheck.value() && GlobalVars::gblVars->ViewClip.value() == 0) {
//		DrawScrolls(cboScroll.GetListIndex(), mgrScrolls->GetScrollInfo());
//move to ui?
		//DrawScrolls(ScrollIndex, mgrScrolls->GetScrollInfo());
	}

	LeakFinder::finder->LogActiveLeaks(Logger::log);

	throw new exception("Commented code needs to be sent to UI");
	/*InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);

	SendMessage(UiState::stateManager->GetMapWindow(), WM_SIZE, 1, 1);*/
	//memset(&DrawState, 0, sizeof(drawstate));
	DrawStatus.dirty = false;
	return 0;
}

int RD1Engine::DrawLayer(nMapBuffer* Map, Image* pic, unsigned char ctype) {//img is the image to blit to 
	int thisX = 0, thisY = 0, mX = 0, mY = 0;
	unsigned short TILE = 0;
	int i = 0;
	int X = Map->X;
	int Y = Map->Y;
	int ThisY = 0;
	int ThisX = 0;
	unsigned short* TileBuf2D = Map->TileBuf2D;
	if (TileBuf2D == NULL)
	{
		return -1;
	}




	if (ctype == 0x40) {
		//LZ 
						//pic->Create(BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X*16,BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y*16);
						//pic->SetPalette(GBAGraphics::VRAM->PcPalMem);
						//this type uses the background set for tiles.

		for (int ScenRep = 0; ScenRep < mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X; ScenRep++) {
			for (thisX = 0; thisX < Map->X; thisX++) {
				for (thisY = 0; thisY < Map->Y; thisY++){
					throw new exception("Drawing bg needs to be reimplemented");
				//pic->Draw(GlobalVars::gblVars->BGImage, (ScenRep * 32) + (thisX) * 8, (mainRoom->roomHeader.bSceneryYPos - 1) * 16 + (thisY) * 8, TileBuf2D[(thisX)+(thisY*X)]);
			}
			}
		}

	}
	else
		if (ctype == 0x10) {
			//	for(thisY=Map->DrawRect.top;thisY<(Map->DrawRect.bottom);thisY++){

			//		for(thisX = Map->DrawRect.left; thisX < (Map->DrawRect.right);thisX++){// from here if something is enabled then draw it 
			for (thisY = 0; thisY < (Map->Y); thisY++) {

				for (thisX = 0; thisX < (Map->X); thisX++) {// from here if something is enabled then draw it 
					TILE = (TileBuf2D[thisX + (thisY * X)]);
					throw new exception("Drawing layer needs to be reimplemented");
					//BitBlt(pic->DC(), (thisX) << 4, (thisY) << 4, 16, 16, GlobalVars::gblVars->imgTileset->DC(), (TILE % 16) << 4, (TILE >> 4) << 4, SRCCOPY);
				}
			}
		}
	return 0;
}

int RD1Engine::DrawBackGround(Image* img,TileBuffer* bimage) {


	int mX = mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X;
	int mY = mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y;
	int difference = (mY > mX ? mX : mY);
	int HorScenRep = 0;
	int VerScenRep = 0;

	throw new exception("Implemented commented code in ui");
	//nHScroll[SceneHScroll] = nVScroll[SceneVScroll] = nVScroll[STVScroll] = 0;
	//nMaxHScroll[SceneHScroll] = 0;
	//nMaxVScroll[SceneVScroll] = mY / 8;
	//nMaxVScroll[STVScroll] = 2;//for now

	int i, x, y, scrX, scrY;
	i = x = y = scrX = scrY = 0;//
	int rex = (mX == 64 ? 2 : 1);
	int rey = (mY == 64 ? 2 : 1);


	if (mX && mY) {
		for (scrY = 0; scrY < rey; scrY++)
		{
			for (scrX = 0; scrX < rex; scrX++)
			{
				for (y = 0; y < 32; ++y)
				{
					for (x = 0; x < 32; ++x)
					{
						for (int WRep = 0; WRep <= mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X / mX + 1; WRep++) {
							for (int HRep = 0; HRep <= mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y / mY + 1; HRep++) {
//								img->Draw(bimage, x * 8 + scrX * 256 + (WRep*(mX * 8)), y * 8 + scrY * 256 + (HRep*(mY * 8)), mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->TileBuf2D[i]);
							}
						}
						i++;
					}
				}
			}
		}
	}

	return 0;
}
void RD1Engine::DrawDoorIndicators(HDC g)
{
	std::vector<vDoor>* theDoors = &mgrDoors->Doors;

	for (int doorCounter = 0; doorCounter < theDoors->size(); doorCounter++)
	{
		sDoor  *thisDoor = &theDoors->at(doorCounter).rawDoor;
		//make a rect 
		int x = ((thisDoor->XEntrance + thisDoor->DWidth / 2) * 16);
		int y = ((thisDoor->YEntrance + thisDoor->DHeight / 2) * 16);

		int w = 16;
		int xDistance = thisDoor->xExitDistance;
		int xabs = abs(xDistance);

		if (thisDoor->xExitDistance < 0)
		{

			//_theLog->logit(abs);
			x -= xabs;
		}

		w += xabs;


		int h = 16;
		int yDistance = thisDoor->yExitDistance;
		int yabs = abs(yDistance);
		if (thisDoor->yExitDistance < 0)
		{

			//Logger::logit->(abs);

			y -= yabs;
		}
		h += yabs;
		printf("Drawing Rectangle at %d %d %d %d", x, y, w, h);
		RECT r;// x, y, w, h);
		r.left = x;
		r.top = y;
		r.right = w;
		r.bottom = h;
		DrawRect(g, (long)RGB(255, 255, 0), &r, 1);

	}
}

void RD1Engine::DumpAreaAsImage(char* fn)
{
	//Font thisFont = new Font("Arial", 16);
	DataContainer* roomsPerArea = GameConfiguration::mainCFG->GetDataContainer("RoomsPerArea");
	int thisArea = 0; //Room.AreaID;
	int maxRooms = roomsPerArea->DataArray[thisArea];

	//Calculate widths
	int maxMapWidth = 0;
	int maxMapHeight = 0;
	for (int roomCounter = 0; roomCounter < maxRooms / 8; roomCounter++)
	{
		LoadRoom(thisArea, roomCounter);

		RHeader* header = &theGame->mainRoom->roomHeader;
		nMapBuffer* bounds = mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
		int calcHeight = header->bMiniMapRoomY * 16 * 16 + bounds->Y;
		int calcWidth = header->bMiniMapRoomX * 16 * 16 + bounds->X;
		maxMapHeight = maxMapHeight > calcHeight ? maxMapHeight : calcHeight;
		maxMapWidth = maxMapWidth > calcWidth ? maxMapWidth : calcWidth;

	}


	Image* theBigPicture = new Image();
	theBigPicture->Create(maxMapWidth + 512, maxMapHeight + 512);
	//Image thebigImage = theBigPicture;
	std::vector<RECT> overLapList;// = new List<Rectangle>();
	HDC g = theBigPicture->DC();
	for (int roomCounter = 0; roomCounter < maxRooms; roomCounter++)
	{
		
		char infoString[2048];
		LoadRoom(thisArea, roomCounter);
		DrawStatus.dirty = 1;
		//DrawRoom();
		vector<nEnemyList>* hey = &mainRoom->mgrSpriteObjects->SpriteObjects;
		RHeader* header = &theGame->mainRoom->roomHeader;
		nMapBuffer* bounds = mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
		sprintf(infoString, "%sArea %x: Room %x", infoString, thisArea, roomCounter);
		int enemyCounter = 0;


		if (hey->size() > 0) sprintf(infoString, "%s\nEnemies1: %d", infoString, (hey->at(0)).Max());
		if (hey->size() > 1) sprintf(infoString, "%s\nEnemies2: %d : Event Trigger - %x", infoString, (hey->at(1)).Max(), header->bEventSwitch);
		if (hey->size() > 2) sprintf(infoString, "%s\nEnemies3: %d : Event Trigger - %x", infoString, (hey->at(2)).Max(), header->bEventSwitch2);
		sprintf(infoString, "%s\nHas %d  doors", infoString, theGame->mgrDoors->CurrentRoomDoorIndexes.size());

		sprintf(infoString, "%s\nRoom X:%d\nRoomY:%d\nRoom Width:%d\nRoom Height:%d\n", infoString, header->bMiniMapRoomX, header->bMiniMapRoomX, bounds->X, bounds->Y);


		int maxWidth = bounds->X * 16;
		int maxHeight = bounds->Y * 16;

		DrawDoorIndicators(g);
		SetTextColor(g, RGB(255, 255, 0));

		TextOut(
			g,
			header->bMiniMapRoomX * 16 * 16 + 16,
			header->bMiniMapRoomY * 16 * 16 + maxHeight / 2,
			infoString,
			strlen(infoString)
		);


		BitBlt(g,
			header->bMiniMapRoomX * 16 * 16,
			header->bMiniMapRoomY * 16 * 16,
			bounds->X * 16,
			bounds->Y * 16,
			ThisBackBuffer.DC(), 0, 0, SRCCOPY);
		

	}
	//Image* minimap = new Image(512, 512);
	//Image* Tileset = new Image(512, 512);
	//MiniMapClass* mmc = new MiniMapClass();
	//mmc->Create(minimap, Tileset);
	//mmc->DecompressMap(thisArea);
	//mmc->DrawMap(minimap);
	//BitBlt(minimap->DC(), maxMapWidth, maxMapHeight, 512, 512,
	//	ThisBackBuffer.DC(), 0, 0, SRCCOPY);
	//delete minimap;
	//delete Tileset;
	FILE* fp = fopen("boom.bmp", "w+b");
	theBigPicture->SaveToFile(fp);
	fclose(fp);
	delete theBigPicture;
}