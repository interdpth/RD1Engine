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
void RD1Engine::LoadRoomSpriteSet(int area, int room, Image* Tileset, TileBuffer* SpriteImage, int spriteindex)
{
	mgrDoors->SetupDoors(area);
	memset(&DrawStatus, 0, sizeof(drawstate));
	DrawStatus.dirty = true;
	DrawStatus.BG0 = true;
	DrawStatus.BG1 = true;
	DrawStatus.BG2 = true;
	DrawStatus.BG3 = true;
	DrawStatus.Doors = true;
	DrawStatus.Sprites = true;

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
	sprintf(buffer, "Loading Area: %d Room: %X, Offset %X", area, room, offset);
	Logger::log->LogIt(Logger::DEBUG, buffer);
	offset = (RoomOffsets[area] - 0x8000000) + (room * 0x3C);
	LoadRoom(area, room, Tileset, SpriteImage, offset);
	////Change to room header 
	mainRoom->LoadUpSprites(spriteindex, SpriteImage);

}

void RD1Engine::LoadRoom(int area, int room, Image* Tileset, TileBuffer* SpriteImage, unsigned long offset)
{
	char buffer[512];
	sprintf(buffer, "Loading Area: %d Room: %X, Offset %X", area, room, offset);
	Logger::log->LogIt(Logger::DEBUG, buffer);
	if (mainRoom)
	{
		delete mainRoom;
		mainRoom = NULL;
	}

	mainRoom = new RoomClass(currentRomType, Tileset, (SpritesetData*)NULL, _gbaMethods, (std::map<int, std::vector<unsigned long>>*)NULL, (FrameManager*)NULL, area, room, offset);
}

int RD1Engine::GetPalSize(int sprID)
{
	int palSize = 0;

	palSize = titleInstance->GetPalSize(sprID);

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

	theGame->mgrTileset->ReadTable();



	if (RD1Engine::theGame->thisTitle == SupportedTitles::titleMF)
	{
		((MetroidFusion*)(RD1Engine::theGame->titleInstance))->LoadGameData();
		MemFile::currentFile->seek(0x3e419c);
		MemFile::currentFile->fread(&idkVRAM.RAM[0x900], 1, 0x36E0);
		MemFile::currentFile->seek(0x58b466);
		MemFile::currentFile->fread(&GBAGraphics::VRAM->GBASprPal[0x40], 1, 0x14);
		MemFile::currentFile->seek(0x3E40F2);
		MemFile::currentFile->fread(&GBAGraphics::VRAM->GBASprPal[0x56], 1, 0x3E);
	}

}

int RD1Engine::LoadAreaTable()
{
	DataContainer* areas = GameConfiguration::mainCFG->GetDataContainer("Areas");
	MemFile::currentFile->seek(areas->Value);
	for (int i = 0; i < areas->MemberCount; i++)
	{
		unsigned long gamePointer = 0;
		MemFile::currentFile->fread(&gamePointer, sizeof(long), 1);
		RoomOffsets.push_back(gamePointer);
	}
	return areas->MemberCount;
}


RD1Engine::RD1Engine(SupportedTitles theTitle, OamFrameTable*  _oAMFrameTable, TileBuffer * bg, TileBuffer* TileImage, Image* ImageTileset)
{
	ThisBackBuffer.Create(1024, 1024);
	Image *tmp = new Image(1024, 1024);
	long pal[512] = { 0 };
	tmp->SetPalette(pal);
	tmp->Fill(0, 0, 1024, 1024, 0);
	tmp->Blit(ThisBackBuffer.DC(), 0, 0, 1024, 1024, 0, 0);
	delete tmp;
	_theLog = Logger::log;
	currentRomType = (int)theTitle;
	_gbaMethods = new GBAMethods();
	mgrDoors = new DoorManager(_gbaMethods);
	mgrTileset = new TilesetManager(_gbaMethods, currentRomType, bg, TileImage);
	mgrScrolls = new clsRoomScrolls();
	frameTables = _oAMFrameTable;
	mgrOAM = new cOAMManager(&frameTables->OAMFrameTable, _gbaMethods, (int)theTitle);
	_bgBuffer = bg;
	_tileset = ImageTileset;
	titleInstance = NULL;
	titleInstance = NULL;
	this->thisTitle = theTitle;
	switch (theTitle)
	{
	case SupportedTitles::titleMF:
		titleInstance = new MetroidFusion(_gbaMethods, MemFile::currentFile);

		mgrOAM->MFSprSize = ((MetroidFusion*)(titleInstance))->MFSprSize;


		break;
	case SupportedTitles::titleZM:
		titleInstance = new ZeroMission(_gbaMethods, MemFile::currentFile);

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

	if (titleInstance != NULL)
	{
		LoadModifiers((char*)titleInstance->GetPoseFile());
	}

	mainRoom = new RoomClass((int)theTitle, ImageTileset, NULL, _gbaMethods, &frameTables->OAMFrameTable, NULL);
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
	if (titleInstance)
	{
		delete titleInstance;
	}

	if (titleInstance)
	{
		delete titleInstance;
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

		if (frameTables->OAMFrameTable.at(sprite_in->spriteID).size() == 0)
		{
			continue;
		}
		if (frameTables->OAMFrameTable.at(sprite_in->spriteID).front() && badFrame == 0)
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
			if (true) {
				curbrush = CreateSolidBrush(RGB(255, 0, 0));
				myrect.left = SpriteX - adjustedXorigin - 4;

				myrect.top = SpriteY - adjustedYorigin + 4;

				myrect.right = (SpriteWidth)+myrect.left;
				myrect.bottom = myrect.top + SpriteHeight;
				FrameRect(pic->DC(), &myrect, curbrush);
				DeleteObject(curbrush);
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
		/*else {*/
		if (true) {
			myrect.left = SpriteX;
			myrect.top = SpriteY;
			myrect.right = myrect.left + 16;
			myrect.bottom = myrect.top + 16;
			curbrush = CreateSolidBrush(RGB(255, 0, 0));
			FrameRect(pic->DC(), &myrect, curbrush);
			DeleteObject(curbrush);
			TextOut(pic->DC(), SpriteX + 4,
				SpriteY, "S", 1);
		}
		//}
	}
}



//RD1Engine::theGame->DrawRoom(GlobalVars::gblVars->checkBoxViewB.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewL.value() == BST_CHECKED, GlobalVars::gblVars->checkBoxViewF.value() == BST_CHECKED);

int RD1Engine::DisplayDoors(unsigned char Room) {


	int minX;
	int maxX;
	int minY;
	int maxY;
	int curW, curH, curX, curY;
	int i = 0;
	char cBuf[512] = { 0 };


	//First we get the current display area Displays 23x 21 Tiles at a time
	//so Get scrollbar for basics then get add proper constant


	minX = mainRoom->currentHorizScroll;
	maxX = mainRoom->currentHorizScroll + 23;
	minY = mainRoom->currentVertScroll;
	maxY = mainRoom->currentVertScroll + 21;


	/*minX = 0;
	maxX = 0 + 23;
	minY = 0;
	maxY = 0 + 21;*/
	unsigned long que = 0;
	///RECT blah={0,0,0,0};
	SetBkMode(ThisBackBuffer.DC(), TRANSPARENT);
	for (i = 0; i < mgrDoors->CurrentRoomDoorIndexes.size(); i++) {
		int CurDoorIndex = mgrDoors->CurrentRoomDoorIndexes[i];
		MousePointer* thisDoor = &mgrDoors->Doors[CurDoorIndex].virtualDoor;
		if (mgrDoors->Doors[CurDoorIndex].rawDoor.OwnerRoom == Room) {
			curX = thisDoor->sX;
			curY = thisDoor->sY;
			curH = thisDoor->Height;//-Doors[CurrentRoomDoorIndexes[i]].virtualDoor.sY)+1;
			curW = thisDoor->Width;//-Doors[CurrentRoomDoorIndexes[i]].virtualDoor.sX)+1;
			//	ShowWindow((HWND)hDoors[i],SW_SHOW);
			//	MoveWindow((HWND)hDoors[i],(unsigned short)(curX-minX)*16,(unsigned short)(curY-minY)*16,(curW+1)*16,(curH-curY+1)*16,0);
			RECT blah;
			for (int d = 0; d < 2; d++) {
				blah.left = (curX) * 16 + d;
				blah.top = (curY) * 16 + d;
				blah.right = (curW + 1) * 16 + d;
				blah.bottom = (curH + 1) * 16 + d;
				HBRUSH curbrush = CreateSolidBrush(RGB(255, 255, 255));

				FrameRect(ThisBackBuffer.DC(), &blah, curbrush);
				DeleteObject(curbrush);
			}

			TextOut(ThisBackBuffer.DC(),
				((curX)+(((curW - curX) / 2)))*(16),
				((curY)+(((curH - curY) / 2)))*(16), "D", 1);
		}


	}

	return 0;
}


int RD1Engine::DrawRoom(TileBuffer* TileImage, TileBuffer* BGImage, int ScrollIndex) {

	unsigned char cSize = 0;
	unsigned char fd = 0;
	unsigned long fWidth = 0, fHeight = 0;
	int k = 0;
	MapManager* mgr = mainRoom->mapMgr;
	if (!mgr->created)
	{
		//Logger::log->LogIt(Logger::LOGTYPE::ERRORZ, "ROOM NOT LOADED");
		return -1;
	}



	bool DrawBackLayer = DrawStatus.BG2;
	bool DrawLevelLayer = DrawStatus.BG1;
	bool DrawForeground = DrawStatus.BG0;
	bool HideSprites = DrawStatus.SpriteRect;
	bool ShowScrolls = DrawStatus.Scrolls;
	bool ShowClip = DrawStatus.Clipdata;


	nMapBuffer* buffForeground = mgr->GetLayer(MapManager::ForeGround);
	nMapBuffer* buffLevelData = mgr->GetLayer(MapManager::LevelData);
	nMapBuffer* buffBackLayer = mgr->GetLayer(MapManager::Backlayer);
	nMapBuffer* buffBackground = mgr->GetLayer(MapManager::BackgroundLayer);
	unsigned short Width = buffLevelData->X * 16, Height = buffLevelData->Y * 16;


	if (!((currentRomType == 0) || (currentRomType == 1))) return 0;

	if (!DrawStatus.dirty)
	{
		Logger::log->LogIt(Logger::ERRORZ, "WASTED DRAW ROOM");
		return 0;
	}
	//Some setup
	TileImage->Load(GBAGraphics::VRAM->fGbuf, 1024);
	BGImage->Load(GBAGraphics::VRAM->BGBuf, 1024);
	ThisBackBuffer.Destroy();
	ThisBackBuffer.Create(Width, Height);
	SetBkMode(ThisBackBuffer.DC(), TRANSPARENT);
	SetTextColor(ThisBackBuffer.DC(), RGB(255, 255, 255));

	//Setup rendermap


	if (mainRoom->roomHeader.lBg3 & 0x40) {

		int mX = mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X * 8;
		int mY = mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y * 8;
		Image* bg3Img = new Image();
		bg3Img->Create(mX * 8, mY * 8);

		float newX = (float)mX * (float)Width;
		newX /= 100;
		//newX *= Width;
	///	newX = newX/Width;
		bg3Img->SetPalette(GBAGraphics::VRAM->PcPalMem);

		DrawLayer(buffBackground, bg3Img, 0x40);// BGImage);
		bg3Img->Blit(ThisBackBuffer.DC(), 0, 0, Width, Height, 0, 0);
		//StretchBlt(ThisBackBuffer.DC(), 0, 0, (int)Width, Height, bg3Img->DC(), 0, 0, mX, mY, SRCCOPY);
		delete bg3Img;
	}
	Image imgMap;

	imgMap.Create(Width, Height);

	imgMap.SetPalette(GBAGraphics::VRAM->PcPalMem);

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

		if ((signed)mainRoom->roomHeader.TransForeground > 16)
		{
			BLENDFUNCTION bfn = { 0 }; // Struct with info for AlphaBlend
			bfn.BlendOp = AC_SRC_OVER;
			bfn.BlendFlags = 0;
			bfn.SourceConstantAlpha = 150;;//Need to unhard code
			bfn.AlphaFormat = 0;// AC_SRC_ALPHA;
			AlphaBlend(ThisBackBuffer.DC(), 0, 0, Width, Height, imgMap.DC(), 0, 0, Width, Height, bfn); // Display bitmap

		}
		else {
			imgMap.TransBlit(ThisBackBuffer.DC(), 0, 0xFF - mainRoom->roomHeader.bSceneryYPos, Width, Height, 0, 0);
		}

		buffForeground->Dirty = 0;

	}



	buffForeground->Dirty = 0;

	DisplayDoors(mainRoom->Room);

	if (ShowClip)//GlobalVars::gblVars->ViewClip.value() == 1)
	{

		DrawClipIdent();
	}

	if (ShowScrolls && !ShowClip) {//(GlobalVars::gblVars->ScrollCheck.value() && GlobalVars::gblVars->ViewClip.value() == 0) {

		DrawScrolls(ScrollIndex, mgrScrolls->GetScrollInfo());
	}

	LeakFinder::finder->LogActiveLeaks(Logger::log);


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
		pic->SetPalette(GBAGraphics::VRAM->PcPalMem);
		//this type uses the background set for tiles.

		for (int ScenRep = 0; ScenRep < mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X; ScenRep++)
		{
			for (thisX = 0; thisX < X; thisX++) {
				for (thisY = 0; thisY < Y; thisY++) {

					pic->Draw(*_bgBuffer, (thisX) * 8, (thisY) * 8, TileBuf2D[(thisX)+(thisY * X)]);
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

					BitBlt(pic->DC(), (thisX) << 4, (thisY) << 4, 16, 16, _tileset->DC(), (TILE % 16) << 4, (TILE >> 4) << 4, SRCCOPY);
				}
			}
		}
	return 0;
}

//int RD1Engine::DrawBackGround(Image* img, TileBuffer* bimage) {
//
//
//	int mX = mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->X;
//	int mY = mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->Y;
//	int difference = (mY > mX ? mX : mY);
//	int HorScenRep = 0;
//	int VerScenRep = 0;
//
//
//	//nHScroll[SceneHScroll] = nVScroll[SceneVScroll] = nVScroll[STVScroll] = 0;
//	//nMaxHScroll[SceneHScroll] = 0;
//	//nMaxVScroll[SceneVScroll] = mY / 8;
//	//nMaxVScroll[STVScroll] = 2;//for now
//
//	int i, x, y, scrX, scrY;
//	i = x = y = scrX = scrY = 0;//
//	int rex = (mX == 64 ? 2 : 1);
//	int rey = (mY == 64 ? 2 : 1);
//
//
//	if (mX && mY) {
//		/*for (scrY = 0; scrY < rey; scrY++)
//		{
//			for (scrX = 0; scrX < rex; scrX++)
//			{*/
//		for (y = 0; y < 32; ++y)
//		{
//			for (x = 0; x < 32; ++x)
//			{
//				/*for (int WRep = 0; WRep <= mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X / mX + 1; WRep++) {
//					for (int HRep = 0; HRep <= mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y / mY + 1; HRep++) {
//					*/	img->Draw(*bimage, x * 8 + scrX * 256, y * 8 + scrY * 256, mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer)->TileBuf2D[i]);
//					/*	}
//					}*/
//			i++;
//			}
//		}
//		/*	}
//		}*/
//	}
//
//	return 0;
//}


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

int percentage(int val, float p)
{
	return (int)((float)(val * p));
}
void RD1Engine::DumpAreaAsImage(char* fn, Image* Tileset, TileBuffer* SpriteImage, TileBuffer* tileImage, TileBuffer* bgImage)
{
	//Font thisFont = new Font("Arial", 16);
	DataContainer* roomsPerArea = GameConfiguration::mainCFG->GetDataContainer("RoomsPerArea");
	int thisArea = 0; //Room.AreaID;
	int maxRooms = roomsPerArea->DataArray[thisArea];

	//Calculate widths
	int maxMapWidth = 0;
	int maxMapHeight = 0;
	for (int roomCounter = 0; roomCounter < maxRooms; roomCounter++)
	{
		LoadRoomSpriteSet(thisArea, roomCounter, Tileset, SpriteImage);

		RHeader* header = &theGame->mainRoom->roomHeader;
		nMapBuffer* bounds = mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
		int calcHeight = header->bMiniMapRoomY * 16 * 16 + bounds->Y;
		int calcWidth = header->bMiniMapRoomX * 16 * 16 + bounds->X;
		maxMapHeight = maxMapHeight > calcHeight ? maxMapHeight : calcHeight;
		maxMapWidth = maxMapWidth > calcWidth ? maxMapWidth : calcWidth;
	}


	BackBuffer* theBigPicture = new BackBuffer();
	theBigPicture->Create(percentage(maxMapWidth, 0.8), percentage(maxMapHeight, 0.8));

	//Image thebigImage = theBigPicture;
	std::vector<RECT> overLapList;// = new List<Rectangle>();
	HDC g = theBigPicture->DC();
	for (int roomCounter = 0; roomCounter < maxRooms; roomCounter++)
	{

		char infoString[12048] = { 0 };
		LoadRoomSpriteSet(thisArea, roomCounter, Tileset, SpriteImage);
		//theGame->LoadRoom(thisArea, roomCounter, Tileset, SpriteImage, (RoomOffsets[thisArea] - 0x8000000) + (roomCounter * 0x3C));

		DrawStatus.dirty = 1;
		theGame->DrawRoom(tileImage, bgImage, -1);
		vector<nEnemyList>* hey = &mainRoom->mgrSpriteObjects->SpriteObjects;
		RHeader* header = &theGame->mainRoom->roomHeader;
		nMapBuffer* bounds = mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
		sprintf(infoString, "%sArea %X: Room %X", infoString, thisArea, roomCounter);
		int enemyCounter = 0;
		BackBuffer room;
		room.Create(bounds->X * 16, bounds->Y * 16);

		HDC r = room.DC();
		if (hey->size() > 0) sprintf(infoString, "%s\nEnemies1: %d", infoString, (hey->at(0)).Max());
		if (hey->size() > 1) sprintf(infoString, "%s\nEnemies2: %d : Event Trigger - %X", infoString, (hey->at(1)).Max(), header->bEventSwitch);
		if (hey->size() > 2) sprintf(infoString, "%s\nEnemies3: %d : Event Trigger - %X", infoString, (hey->at(2)).Max(), header->bEventSwitch2);
		//sprintf(infoString, "%s\nHas %d  doors", infoString, theGame->mgrDoors->CurrentRoomDoorIndexes.size());

		sprintf(infoString, "%s\nRoom X:%d\nRoomY:%d\nRoom Width:%d\nRoom Height:%d\n", infoString, header->bMiniMapRoomX, header->bMiniMapRoomX, bounds->X, bounds->Y);


		int maxWidth = bounds->X * 16;
		int maxHeight = bounds->Y * 16;


		BitBlt(r,
			0,
			0,
			bounds->X * 16,
			bounds->Y * 16,
			ThisBackBuffer.DC(), 0, 0, SRCCOPY);


		DrawDoorIndicators(r);
		SetTextColor(g, RGB(255, 255, 0));

		TextOut(
			g,
			8,
			(bounds->Y / 2) * 16,
			infoString,
			strlen(infoString)
		);


	
		StretchBlt(g,
			percentage(header->bMiniMapRoomX * 16 * 16, 0.8),
			percentage(header->bMiniMapRoomY * 16 * 16, 0.8),
			percentage(bounds->X * 16, 0.8),
			percentage(bounds->Y * 16, 0.8),
			r, 0, 0, bounds->X * 16, bounds->Y * 16, SRCCOPY);

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
	FILE* fp = fopen("boom2.bmp", "w+b");
	theBigPicture->SaveToFile(fp);
	fclose(fp);
	delete theBigPicture;
}






int             RD1Engine::DrawClipIdent()
{
	int             currclip = 0;// cboClipData.GetListIndex();
	RECT            blah;
	int             thisX = 0;
	int             thisY = 0;
	int             x = 0;
	int             y = 0;

	HBRUSH          curbrush = CreateSolidBrush(RGB(255, 255, 255));

	for (thisY = 0; thisY < RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y; thisY++)
	{
		for (thisX = 0; thisX < RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X; thisX++)
		{

			if (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->TileBuf2D[thisX + (thisY * (RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata)->X))] == currclip)
			{

				blah.left = thisX * 16;
				blah.right = thisX * 16 + 16;
				blah.top = thisY * 16;
				blah.bottom = thisY * 16 + 16;

				FrameRect(RD1Engine::theGame->ThisBackBuffer.DC(), &blah, curbrush);


			}
		}
	}
	DeleteObject(curbrush);
	return 0;
}

int RD1Engine::DrawScrolls(int ScrollToDraw, Scroller *scroll) {

	RECT blah;

	MousePointer thisscroll;

	if (scroll->Room != 0xFF) {


		thisscroll = scroll->Scrolls[ScrollToDraw].rect;



		if ((thisscroll.sX != 0xFF) && (thisscroll.Height != 0xFF)) {








			HBRUSH curbrush = CreateSolidBrush(
				RGB(
					255 -
					((ScrollToDraw + 3) << 2) | 128,
					255 - ((ScrollToDraw + 2) << 4) | 128,
					255 - ((ScrollToDraw + 1) << 8) | 128
				)
			);


			for (int d = 0; d < 2; d++) {
				blah.left = (thisscroll.sX) * 16 + d;
				blah.top = (thisscroll.sY) * 16 + d;
				blah.right = (thisscroll.Width) * 16 + d;
				blah.bottom = (thisscroll.Height) * 16 + d;
				FrameRect(RD1Engine::theGame->ThisBackBuffer.DC(), &blah, curbrush);

			}




			DeleteObject(curbrush);

			TextOut(RD1Engine::theGame->ThisBackBuffer.DC(),
				((thisscroll.sX) + (((thisscroll.Width - thisscroll.sX) / 2)))*(16),
				((thisscroll.sY) + (((thisscroll.Height - thisscroll.sY) / 2)))*(16), "Sc", 1);

		}



	}
	return 0;
}//End Function

int  RD1Engine::GetRoomCount(long RoomTableOffset) {
	vector<RHeader> roomHeaders;
	for (int i = 0; i < 1000; i++)
	{
		unsigned long TheOffset = ((RoomTableOffset - 0x8000000) + i * 0x3C);
		RHeader tmpHeader;
		MemFile::currentFile->seek(TheOffset);
		MemFile::currentFile->fread(&tmpHeader.bTileset, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bBg0, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bBg1, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bBg2, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.lBg3, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.lForeground, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.lLevelData, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.lBackLayer, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.lClipData, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.lBackgroundTSA, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.bUnknown1, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.TransForeground, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.iSep1, sizeof(unsigned short), 1);
		MemFile::currentFile->fread(&tmpHeader.lSpritePointer, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.bSpriteIndex1, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bEventSwitch, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.iSep2, sizeof(unsigned short), 1);
		MemFile::currentFile->fread(&tmpHeader.lSpritePointer2, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.bSpriteIndex2, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bEventSwitch2, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.iSeperator, sizeof(unsigned short), 1);
		MemFile::currentFile->fread(&tmpHeader.lSpritePointer3, sizeof(unsigned long), 1);
		MemFile::currentFile->fread(&tmpHeader.bSpriteIndex3, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bMiniMapRoomX, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bMiniMapRoomY, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bEffect, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bSceneryYPos, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bNothing, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.bMusic, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&tmpHeader.blank, sizeof(unsigned char), 1);

		if (CheckHeader(&tmpHeader) == true) {
			roomHeaders.push_back(tmpHeader);
		}
		else {
			break;
		}
	}
	return roomHeaders.size();
}



bool  RD1Engine::IsValidCompression(unsigned char tstByte) {


	if (tstByte == 0 || tstByte == 0x10 || tstByte == 0x40 || tstByte == 0x31 || tstByte == 0x45) {

		return true;
	}
	printf("Layer had an invalid compression type");
	return false;
}

bool  RD1Engine::IsValidPointer(unsigned long pnt) {
	if (pnt == 0xffffffff || pnt == 0xffff0000 || pnt == 0x0003fffc) {
		return false;
	}
	unsigned long tstPointer = pnt;
	tstPointer = tstPointer & 0x8000000;
	return tstPointer == 0x8000000;
}

bool  RD1Engine::CheckHeader(RHeader* tstHeader)
{
	//Check layer compression
	if (IsValidCompression(tstHeader->bBg0) == false) {
		return false;
	}
	if (IsValidCompression(tstHeader->bBg1) == false) {
		return false;
	}
	if (IsValidCompression(tstHeader->bBg2) == false) {
		return false;
	}
	if (IsValidCompression(tstHeader->lBg3) == false) {
		return false;
	}

	//Check pointers 



	if (IsValidPointer(tstHeader->lForeground) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lLevelData) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lBackLayer) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lClipData) == false) {
		return false;
	}


	if (IsValidPointer(tstHeader->lBackgroundTSA) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lSpritePointer) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lSpritePointer2) == false) {
		return false;
	}

	if (IsValidPointer(tstHeader->lSpritePointer3) == false) {
		return false;
	}



	return true;

}

int             RD1Engine::Save(MemFile * fp)
{
	MapManager *mm = RD1Engine::theGame->mainRoom->mapMgr;
	long            offset = 0;

	long            tlength;

	unsigned char*  compBuffer = new unsigned char[64691];
	memset(compBuffer, 0, 64691);

	if (fp)
	{


		MapManager::SaveLayer(_gbaMethods, RD1Engine::theGame->mainRoom->roomHeader.bBg0, &RD1Engine::theGame->mainRoom->roomHeader.lForeground, mm->GetLayer(MapManager::ForeGround));
		MapManager::SaveLayer(_gbaMethods, RD1Engine::theGame->mainRoom->roomHeader.bBg1, &RD1Engine::theGame->mainRoom->roomHeader.lLevelData, mm->GetLayer(MapManager::LevelData));
		MapManager::SaveLayer(_gbaMethods, RD1Engine::theGame->mainRoom->roomHeader.bBg2, &RD1Engine::theGame->mainRoom->roomHeader.lBackLayer, mm->GetLayer(MapManager::Backlayer));

		if (mm->GetLayer(MapManager::Clipdata)->SDirty == 1)
		{
			memset(compBuffer, 0, 64691);
			tlength = _gbaMethods->compress(compBuffer, (unsigned char *)mm->GetLayer(MapManager::Clipdata)->TileBuf2D, (mm->GetLayer(MapManager::Clipdata)->X * mm->GetLayer(MapManager::Clipdata)->Y) * 2);
			offset = _gbaMethods->FindFreeSpace(tlength + 4, 0xFF);
			if (offset != 0)
			{
				RD1Engine::theGame->mainRoom->roomHeader.lClipData = 0x8000000 + offset;
				MemFile::currentFile->seek(offset);
				MemFile::currentFile->fputc(mm->GetLayer(MapManager::Clipdata)->X);
				MemFile::currentFile->fputc(mm->GetLayer(MapManager::Clipdata)->Y);
				MemFile::currentFile->fwrite(compBuffer, sizeof(char), tlength);
			}
			else
			{
				new std::exception("Could not save Clip Data.");
			}
		}
	}
	delete[] compBuffer;
	return 0;
}
int  RD1Engine::SaveLevel(unsigned long HeaderOffset) {

	MemFile* theFile = MemFile::currentFile;
	RHeader* roomHeader;
	//	GBA.REDIT = fopen(FilePath,"r+b");
	if (theFile) {
		//   while(1){ 
		Save(theFile);
		//   if(BaseGame::theGame->mainRoom->roomHeader.lBackLayer!=BaseGame::theGame->mainRoom->roomHeader.lLevelData) break;
		//	}
		RD1Engine::theGame->mainRoom->mgrSpriteObjects->SaveSprites(&RD1Engine::theGame->mainRoom->roomHeader);
		theFile->seek(HeaderOffset);
		roomHeader = &RD1Engine::theGame->mainRoom->roomHeader;


		theFile->fwrite(&roomHeader->bTileset, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bBg0, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bBg1, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bBg2, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->lBg3, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->lForeground, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->lLevelData, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->lBackLayer, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->lClipData, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->lBackgroundTSA, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->bUnknown1, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->TransForeground, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->iSep1, sizeof(unsigned short), 1);
		theFile->fwrite(&roomHeader->lSpritePointer, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->bSpriteIndex1, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bEventSwitch, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->iSep2, sizeof(unsigned short), 1);
		theFile->fwrite(&roomHeader->lSpritePointer2, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->bSpriteIndex2, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bEventSwitch2, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->iSeperator, sizeof(unsigned short), 1);
		theFile->fwrite(&roomHeader->lSpritePointer3, sizeof(unsigned long), 1);
		theFile->fwrite(&roomHeader->bSpriteIndex3, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bMiniMapRoomX, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bMiniMapRoomY, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bEffect, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bSceneryYPos, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bNothing, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->bMusic, sizeof(unsigned char), 1);
		theFile->fwrite(&roomHeader->blank, sizeof(unsigned char), 1);

		RD1Engine::theGame->mgrScrolls->SaveScroll(_gbaMethods);
		RD1Engine::theGame->mgrDoors->SaveDoors(RD1Engine::theGame->mainRoom->Area);
		RD1Engine::theGame->mainRoom->LoadHeader(HeaderOffset);
		MemFile::currentFile->Save();

	}
	//fp = NULL;
	return 0;
}

