#include "cSSE.h"
#include "cOAMManager.h"
LRESULT CALLBACK cSSETileProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam);
cSSE *cSSE::SpriteSet;
cSSE::cSSE()
{
	SpritePreview = new SprGBuf();
}


cSSE::~cSSE()
{
	delete SpritePreview;
}



int cSSE::Create(HINSTANCE mainInstance) {
	char stringBuffer[256] = { 0 };
	int i = 0;
	SpriteSets.Init(GetDlgItem(me, lstSprites));
	Population.Init(GetDlgItem(me, lstPopulation));
	cboDetail.Init(GetDlgItem(me, cboSlot));
	cboDetail.Clear();
	SpriteSets.Clear();
	Population.Clear();
	for (i = 0; i < 256; i++) {
		sprintf(stringBuffer, "%X", i);
		SpriteSets.Additem(stringBuffer);
		if (16 > i)Population.Additem(stringBuffer);
		if (8 > i)cboDetail.Additem(stringBuffer);
	}
	CreateTileWindow(mainInstance, &cSSETileProc);
	CreatePalWindow(mainInstance, &cSSEPalProc);
	SpriteSets.SetListIndex(0);
	return 0;
}






int cSSE::GetSet(int TitleChoice, unsigned char SpriteSetSelection, cEntityManager* mgr) 
{//SpriteSets.GetListIndex()
	char tehbuf[256] = { 0 };
	int i = 0;
	total = 0;
	if (TitleChoice == 0) {
		GlobalVars::gblVars->SSE = true;

		mgr->LoadSet(mgr->gfxinfo, mgr->palinfo, mgr->spriteset, SpriteSetSelection);
		GlobalVars::gblVars->SSE = false;
	}
	else if (TitleChoice == 1) {
		GlobalVars::gblVars->SSE = true;
		mgr->MFLoadSet(mgr->gfxinfo, mgr->palinfo, mgr->spriteset, SpriteSetSelection);
		GlobalVars::gblVars->SSE = false;
	}
	Population.Clear();
	for (i = 0; i < cSSE::SpriteSet->total - 1; i++) {
		sprintf(tehbuf, "%X", i);

		Population.Additem(tehbuf);

	}
	return 0;
}

int cSSE::DecodeSet(bool romSwitch) {
	int i = 0;
	GlobalVars::gblVars->SSE = true;
	cEntityManager* mgr = RD1Engine::theGame->mainRoom->mgrEntities;
	memset(pal, 0, sizeof(pal));
	memset(GBAGraphics::VRAM->GBASprPal, 0, sizeof(GBAGraphics::VRAM->GBASprPal));
	mgr->LoadPal(mgr->palinfo, mgr->spriteset, pal);

	RD1Engine::theGame->mgrOAM->LoadSpriteToMem(romSwitch, mgr->gfxinfo, mgr->spriteset, graphics, &SprGraphics);
	GlobalVars::gblVars->SSE = false;
	SpriteSet->Tiles.Create(512, 512);
	SpriteSet->Tiles.SetPalette(pal);
	for (i = 512; i < 1024; i++) {
		SpriteSet->Tiles.Draw(SprGraphics, ((i - 512) % 32) * 8, ((i - 512) / 32) * 8, 0x8000 + i);
	}

	return 0;
}


int cSSE::CreatePalWindow(HINSTANCE myInstance, WNDPROC proc) {
	WNDCLASSEX garh;
	memset(&garh, 0, sizeof(garh));
	garh.cbSize = sizeof(garh);		   // size of structure 
	garh.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	garh.lpfnWndProc = proc; //&cSSEPalProc;	  // points to window procedure 
	garh.cbClsExtra = NULL;				// no extra class memory 
	garh.cbWndExtra = NULL; 			   // no extra window memory 
	garh.hInstance = myInstance;// hGlobal;		   // handle to instance 
	garh.hIcon = NULL; 			 // predefined app. icon 
	garh.hCursor = NULL;				// predefined arrow 
	garh.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	garh.lpszMenuName = NULL;    // name of menu resource 
	garh.lpszClassName = "SSEPal";  // name of window class 


									// Register the window class. 
	RegisterClassEx(&garh);


	//Create the Window



	PalView = CreateWindowEx(NULL, "SSEPal", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 256, 128, GetDlgItem(me, fraPal), 0, myInstance, 0);


	return 0;
}

int cSSE::CreateTileWindow(HINSTANCE mainInstance, WNDPROC proc) {
	WNDCLASSEX blahf;
	memset(&blahf, 0, sizeof(blahf));
	blahf.cbSize = sizeof(blahf);		   // size of structure 
	blahf.style = CS_HREDRAW |
		CS_VREDRAW; 				   // redraw if size changes 
	blahf.lpfnWndProc = proc;//&cSSESpriteProc;	  // points to window procedure 
	blahf.cbClsExtra = NULL;				// no extra class memory 
	blahf.cbWndExtra = NULL; 			   // no extra window memory 
	blahf.hInstance = mainInstance;		   // handle to instance 
	blahf.hIcon = NULL; 			 // predefined app. icon 
	blahf.hCursor = NULL;				// predefined arrow 
	blahf.hbrBackground = (HBRUSH)GetStockObject(15);				   // white background brush 
	blahf.lpszMenuName = NULL;    // name of menu resource 
	blahf.lpszClassName = "cSSEs";  // name of window class 


									// Register the window class. 
	RegisterClassEx(&blahf);


	//Create the Window



	SprTilesView = CreateWindowEx(NULL, "cSSEs", NULL, WS_VISIBLE | WS_CHILD, 5, 11, 256, 130, GetDlgItem(me, fraTiles), 0, mainInstance, 0);


	return 0;
}


int cSSE::SetInfo() {
	char buffer[2048] = { 0 };
	unsigned short math = 0;
	math = 0x4000 - usedGFX;
	sprintf(buffer, "Sprite Graphic Data:\nThis set has %X bytes free", math);
	SetWindowText(GetDlgItem(me, lblSpriteInfo), buffer);
	math = 8 - ((usedPAL / 0x10));
	sprintf(buffer, "Sprite Palette Data:\nThis set has %X palettes free", math);
	SetWindowText(GetDlgItem(me, lblPaletteInfo), buffer);

	return 0;

}


int cSSE::SetupPreview(SprGBuf* SprG, int TitleChoice) {

	if (SprG->id <= 0xf)
	{
		return -1;
	}
	//Sets up the preview control, will be called alot
	long GFXPnt = 0;
	long PalPnt = 0;

	long addybuf = 0;
	long PalPos = 0;
	long size = 0;

	int i = 0;
	int ii = 0;
	unsigned char* decompbuf = new unsigned char[32687];
	unsigned char*  compBuffer = new unsigned char[64691];


	InvalidateRect(me, 0, 1);
	if (GlobalVars::gblVars->frameTables->OAMFrameTable[SprG->id].size() == 0 && GlobalVars::gblVars->frameTables->OAMFrameTable[SprG->id].front() == 0) return 0;
	memset(&SprG->PreRAM[0x4000], 0, 0x4000);
	

	unsigned short transferpal[256] = { 0 };

	int  X = 0;
	long off = 0;
	int x = 0;
	memset(GBAGraphics::VRAM->SprPal, 0, sizeof(GBAGraphics::VRAM->SprPal));
	//	memset(& GBAGraphics::VRAM->GBASprPal[128],0,sizeof( GBAGraphics::VRAM->GBASprPal)-128);
	//    memset(& GBAGraphics::VRAM->SprPal,0,sizeof( GBAGraphics::VRAM->SprPal));
	GFXPnt = GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (SprG->id - 0x10) * 4;
	PalPnt = GameConfiguration::mainCFG->GetDataContainer("SpritePal")->Value + (SprG->id - 0x10) * 4;
	SprG->palsize = RD1Engine::theGame->GetPalSize(SprG->id);
	
	MemFile::currentFile->seek(PalPnt);
	MemFile::currentFile->fread(&addybuf, 4, 1, GBA.ROM);
	MemFile::currentFile->seek(addybuf - 0x8000000);
	MemFile::currentFile->fread(&transferpal, 1, (SprG->palsize) * 2, GBA.ROM);
	
	memcpy(&GBAGraphics::VRAM->GBASprPal[128], &transferpal, SprG->palsize * 2);

	GBA.DecodePal(GBAGraphics::VRAM->GBASprPal, SprG->PreviewPal, 16, 0);
	
	for (i = 0; i < SprG->palsize; i++)SprG->PreviewPal[128 + i] = cSSE::SpriteSet->pal[(8 + SprG->details) * 16 + i];
	//	memcpy(SprG->PreviewPal,&cSSE::SpriteSet->pal[128],128*4);
	switch (TitleChoice) {
	case 0:
		MemFile::currentFile->seek(GFXPnt);
		MemFile::currentFile->fread(&addybuf, 4, 1, GBA.ROM);
		MemFile::currentFile->seek(addybuf - 0x8000000);
		MemFile::currentFile->fread(compBuffer, 1, sizeof(compBuffer), GBA.ROM);
		size = GBA.LZ77UnComp(compBuffer, decompbuf);
		memcpy(&SprG->PreRAM[0x4000], &decompbuf, size);
		break;
	case 1:
		size = RD1Engine::theGame->mgrOAM->MFSprSize[(SprG->id - 0x10) << 1];
		MemFile::currentFile->seek(GFXPnt);
		MemFile::currentFile->fread(&addybuf, 4, 1, GBA.ROM);
		MemFile::currentFile->seek(addybuf - 0x8000000);
		MemFile::currentFile->fread(&SprG->PreRAM[0x4000], 1, size, GBA.ROM);
		break;
	}

	GlobalVars::gblVars->SSE = true;
	SprG->PreRAM = cSSE::SpriteSet->graphics;
	delete[] decompbuf;
	SprG->PreviewSprite.SetPalette(SprG->PreviewPal);
	memcpy(SprG->PreRAM, cSSE::SpriteSet->graphics, 0x8000);
	SprG->Tiles->Load(SprG->PreRAM, 1024);
	RD1Engine::theGame->mgrOAM->DecodeOAM(GBA.ROM, GlobalVars::gblVars->OAMED, SprG, GlobalVars::gblVars->frameTables->OAMFrameTable[SprG->id].front());
	RD1Engine::theGame->mgrOAM->DrawPSprite(SprG);
	GlobalVars::gblVars->SSE = false;
	InvalidateRect(me, 0, 1);
	delete[] compBuffer;

	return 0;
}