#include "TilesetManager.h"



TilesetManager::TilesetManager(GBAMethods* gba, int crf, TileBuffer * bg, TileBuffer* tilegfx)
{
	animTiles = NULL;
	_gbaMethods = gba;
	currentRomType = crf;
	BGImage = bg;
	TileImage = tilegfx;
}


TilesetManager::~TilesetManager()
{
	if (BGImage) 
	{
		BGImage->Destroy();
	}
}


void TilesetManager::ReadTable()
{
	DataContainer* tileset = GameConfiguration::mainCFG->GetDataContainer("Tileset");
	MemFile::currentFile->seek(tileset->Value);//Tileset
	for (int i = 0; i < tileset->MemberCount; i++) {
		gTileData tileset;
		MemFile::currentFile->fread(&tileset.gTiles, sizeof(long), 1);
		MemFile::currentFile->fread(&tileset.pTiles, sizeof(long), 1);
		MemFile::currentFile->fread(&tileset.gBackground, sizeof(long), 1);
		MemFile::currentFile->fread(&tileset.TSAMap, sizeof(long), 1);
		MemFile::currentFile->fread(&tileset.EffectSet, sizeof(long), 1);
		RoomTilesets.push_back(tileset);
	}
}

int TilesetManager::SaveTileset(unsigned char TilesetVal) {


	unsigned long myoffset = 0x833DFDC;
	//fusionMemFile::currentFile->seek(0x3BF888);//Tileset
	/*
	Get FileNo, HeaderPointers + 1, RoomOffsets
	Get FileNo, DoorPointers + 1, DoorOffsets
	Get FileNo, SpecialEffectTablePnt + 1, SpecialEffects
	Get FileNo, MiniMapBanks + 1, ZMMiniMapOffsets()
	Get FileNo, ZMScrollListBase + 1, ZMScrolls()
	*/

	MemFile::currentFile->seek(GameConfiguration::mainCFG->GetDataContainer("Tileset")->Value);//Tileset


	for (int i = 0; i < GameConfiguration::mainCFG->GetDataContainer("Tileset")->MemberCount; i++) {
		MemFile::currentFile->fwrite(&RoomTilesets[i].gTiles, 4, 1);
		MemFile::currentFile->fwrite(&RoomTilesets[i].pTiles, 4, 1);
		MemFile::currentFile->fwrite(&RoomTilesets[i].gBackground, 4, 1);
		MemFile::currentFile->fwrite(&RoomTilesets[i].TSAMap, 4, 1);
		MemFile::currentFile->fwrite(&RoomTilesets[i].EffectSet, 4, 1);
	}



	return 0;
}



int TilesetManager::SaveTilesetPal(int romSwitch, unsigned char TilesetVal) {
	//gTileData* RoomTilesets = GetTileset(romSwitch, TilesetVal);
	//MemFile::currentFile->seek(RoomTilesets->pTiles - 0x8000000);//Seek the endcoded pal
	//fwrite(&_gbaMethods->GBAPal[32], sizeof(short), 0x1E0);//write


	return 0;

}

void TilesetManager::ReadTSA(gTileData* tileData)
{
	unsigned short TileCheck = 0;
	MemFile::currentFile->seek(tileData->TSAMap - 0x8000000);
	memset(&TSA.nTSA, 0, sizeof(TSA.nTSA));
	MemFile::currentFile->fread(&TSA.ID, sizeof(short), 1);
	TSA.max = 0;
	//MemFile::currentFile->fread(&TSA.nTSA,sizeof(short),0x1080,_gbaMethods->ROM);
	for (int byteCounter = 0; byteCounter < 0x1080; byteCounter++) {
		MemFile::currentFile->fread(&TileCheck, 2, 1);
		TSA.nTSA[byteCounter] = TileCheck;
		if (TileCheck == 0x2) {
			break;

		}
		TSA.max++;
	}
	TSA.max /= 4;
}

void TilesetManager::GetPal(gTileData* tileset)
{
	MemFile::currentFile->seek(tileset->pTiles - 0x8000000);//Seek the endcoded pal
	MemFile::currentFile->fread(&_gbaMethods->GBAPal[32], sizeof(short), 0x1E0);//Read pal
	_gbaMethods->GBAPal[0] = 0;
	_gbaMethods->GBAPal[16] = 0;


	if (currentRomType == 0) {
		MemFile::currentFile->seek(0x31C72E);
		MemFile::currentFile->fread(&_gbaMethods->GBAPal[1], sizeof(short), 15);
		MemFile::currentFile->seek(0x3ED51C);
		MemFile::currentFile->fread(&_gbaMethods->GBAPal[16], sizeof(short), 16);
	}
	if (currentRomType == 0) {
	
	}
	else if (currentRomType == 1) {
		_gbaMethods->GBAPal[0] = 0;
		MemFile::currentFile->seek(0x40805E);
		MemFile::currentFile->fread(&_gbaMethods->GBAPal[1], 2, 47);
		
	}

}
//(unsigned char)comboArea.GetListIndex
void TilesetManager::GetBaseGFX(gTileData* tileset, int area)
{
	unsigned char*  compBuffer = new unsigned char[64691];

	if (currentRomType == 0) 
	{
		if ( area== 6) {
			MemFile::currentFile->seek(0x5DA40C);
			MemFile::currentFile->fread(&GBAGraphics::VRAM->fGbuf[2048], 1, 4098);
		}
		else {
			MemFile::currentFile->seek(0x5D940C);
			MemFile::currentFile->fread(&GBAGraphics::VRAM->fGbuf[0x800], 1, 4098);
		}
	}
	else if (currentRomType == 1) 
	{
		MemFile::currentFile->seek(0x3F28C8);
		MemFile::currentFile->fread(&GBAGraphics::VRAM->fGbuf[0x800], 1, 4098);
	}



	MemFile::currentFile->seek(tileset->gTiles - 0x8000000);//Seek the compreGlobalVars::gblVars->SSEd Tileset
	MemFile::currentFile->fread(compBuffer, sizeof(char), 64691);//Read compression
	unsigned char* decompbuf = new unsigned char[84100];
	memset(decompbuf, 0, 84100);
	long size = _gbaMethods->LZ77UnComp(compBuffer, decompbuf);

	//memcpy( GBAGraphics::VRAM->fGbuf[6144],decompbuf[0],size);
	for (int byteCounter = 0; byteCounter < size; byteCounter++)
	{
		GBAGraphics::VRAM->fGbuf[6144 + byteCounter] = decompbuf[byteCounter];
	}
	delete[] decompbuf;


	delete[] compBuffer;
}
void TilesetManager::GetBaseTileset(unsigned char TilesetVal, int area)
{

	long decomplength = 0;
	//long Tileset=0;
	unsigned short  TileCheck = 0;
	int  byteCounter = 0;
	memset(&GBAGraphics::VRAM->fGbuf, 0, sizeof(GBAGraphics::VRAM->fGbuf));
	memset(&GBAGraphics::VRAM->BGBuf, 0, sizeof(GBAGraphics::VRAM->BGBuf));
	memset(&GBAGraphics::VRAM->PcPalMem, 0, sizeof(GBAGraphics::VRAM->PcPalMem));
	gTileData* roomTileset = &RoomTilesets[TilesetVal];

	
	ReadTSA(roomTileset);
	GetPal(roomTileset);
	GetBaseGFX(roomTileset, area);

	_gbaMethods->DecodePal(_gbaMethods->GBAPal, GBAGraphics::VRAM->PcPalMem, 16, 0);
	//InvalidateRect(hwndLPE, 0, 1);
}

void TilesetManager::Render(Image* srcImage)
{
	TileImage->Destroy();
	BGImage->Destroy();
	animTiles->GetGFX();
	
	//LoadSpecialEffects(thisTileset->EffectSet);
	TileImage->Load(GBAGraphics::VRAM->fGbuf, 1024);

	DrawTileset(&srcImage);

	//animated
	for (int bgCounter = 0; bgCounter < 512; bgCounter++)
	{
		GBAGraphics::VRAM->BGBuf[0x7DE0 + bgCounter] = GBAGraphics::VRAM->fGbuf[0x600 + bgCounter];
	}

	BGImage->Load(GBAGraphics::VRAM->BGBuf, 1024);
}

int TilesetManager::GetBackground(gTileData* thisTileset)
{
	unsigned char* decompbuf = NULL;
	unsigned char*  compBuffer = new unsigned char[64691];
	decompbuf = new unsigned char[84100];
	memset(decompbuf, 0, 84100);
	memset(compBuffer, 0, 64691);
	MemFile::currentFile->seek(thisTileset->gBackground - 0x8000000);
	MemFile::currentFile->fread(compBuffer, sizeof(char), 64691);//Read compression
	int size = _gbaMethods->LZ77UnComp(compBuffer, decompbuf);
	memcpy(&GBAGraphics::VRAM->BGBuf[0x7dE0- size], decompbuf, size);
	delete[] decompbuf;
	delete[] compBuffer;
	return 0;
}

int TilesetManager::GetTileset(Image* dst, int area, unsigned char TilesetVal, int bg3)
{
	gTileData* thisTileset = &RoomTilesets.at(TilesetVal);

	GetBaseTileset(TilesetVal, area);
	if (animTiles)
	{
		delete animTiles;
		animTiles = NULL;
	}

	animTiles = new AnimatedTiles(GameConfiguration::mainCFG->RomSwitch);
	animTiles->ReadTable(thisTileset->EffectSet);
	
	
	if (bg3 == 0x40) 
	{
		GetBackground(thisTileset);

	}
	
	Render(dst);
	return 0;
}

int TilesetManager::DrawBlock(Image* src, short Map16n, int destX, int destY) {

	src->Draw(*TileImage, (destX), (destY), (short)TSA.nTSA[Map16n]);
	src->Draw(*TileImage, (destX)+8, (destY), (short)TSA.nTSA[(Map16n)+1]);
	src->Draw(*TileImage, (destX), (destY)+8, (short)TSA.nTSA[Map16n + 2]);
	src->Draw(*TileImage, (destX)+8, (destY)+8, (short)TSA.nTSA[Map16n + 3]);

	return 0;
}

void TilesetManager::DrawTileset(Image** imgTileset) {
	int k = 0;
	int i = 0;
	int TC = 0;
	int end = 768;

	(*imgTileset)->Clear();
	(*imgTileset)->SetPalette(GBAGraphics::VRAM->PcPalMem);

	Image* tileset = *imgTileset;

	for (i = 0; i < TSA.max; i++)
	{

		DrawBlock(tileset, i * 4, (i % 16) * 16, ((i / 16)) * 16);


	}
//	tileset->glInstance->rawImage = (unsigned long*)tileset->pixels;
	
}

int TilesetManager::LoadSpecialEffects(long EffectNum) {

	long  reference_table=0, tileset_table=0, ref_offset = 0;
	long  ani_source, gfx_destination, effectCounter, pointer = 0;


	unsigned char ref_array[0x30];
	unsigned char effectGraphic[128];



	if (currentRomType == 0) {
		reference_table = 0x35FA78;
		tileset_table = 0x35F948;
	}
	else if (currentRomType == 1) {
		reference_table = 0x3C8F90;
		tileset_table = 0x3C8D58;
	}
	///if(!PreLoad){

	MemFile::currentFile->seek(reference_table + ((EffectNum & 0xFF) * 0x30));
	MemFile::currentFile->fread(&ref_array, sizeof(unsigned char), 0x30);

	for (effectCounter = 0; effectCounter < 16; effectCounter++) {

		//if(    ref_array[i * 3] == 0) continue; 
		//if not last animation, but no animation there, exit)
		ani_source = (tileset_table + (8 * ref_array[effectCounter * 3]));
		MemFile::currentFile->seek(ani_source);
		MemFile::currentFile->fread(&animTiles->specialEffects[effectCounter].Animated, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&animTiles->specialEffects[effectCounter].Timer, sizeof(unsigned char), 1);
		MemFile::currentFile->fread(&animTiles->specialEffects[effectCounter].NumFrames, sizeof(unsigned short), 1);
		MemFile::currentFile->fread(&animTiles->specialEffects[effectCounter].GraphicPnt, sizeof(unsigned long), 1);
		gfx_destination = effectCounter * 0x80; //if 0 is bufferstart...
		if (animTiles->specialEffects[effectCounter].GraphicPnt < 0x7FFFFFF) {
			continue;
		}
	/*	if (SpecialEffects[effectCounter].GraphicEnd != 0 || SpecialEffects[effectCounter].Timer != 0) {
			continue;
		}*/
		MemFile::currentFile->seek(animTiles->specialEffects[effectCounter].GraphicPnt - 0x8000000 + 128);


		MemFile::currentFile->fread(&effectGraphic[0], sizeof(unsigned char), 128);
		for (int graphicSize = 0; graphicSize < 128; graphicSize++)
		{
			GBAGraphics::VRAM->fGbuf[gfx_destination + graphicSize] = effectGraphic[graphicSize];
		}

	}
	//Copy from Tilemem to BGMem

	return 0;
}

int TilesetManager::ZMGlobalGFX(unsigned char Area) {
	/*
	Dim g2Buf(0 To 4096) As Byte
	Dim thispal(0 To 15) As Integer
	Dim fakepal(1 To 15) As Integer
	Dim thatpal(0 To 0, 0 To 15) As Long*/


	if (Area == 6) {

		MemFile::currentFile->seek(0x5DA40C);
		MemFile::currentFile->fread(&GBAGraphics::VRAM->fGbuf[2048], 1, 4098);
		//If (header = "PBMXP01") Then Get _gbaMethods->ROM, &H5ED460 + 1, g2Buf

	}
	else {

		MemFile::currentFile->seek(0x5D940C);
		MemFile::currentFile->fread(&GBAGraphics::VRAM->fGbuf[0x800], 1, 4098);
		//    If (header = "PBMXP01") Then Get _gbaMethods->ROM, &H5EC460 + 1, g2Buf
	}

	//Get FileNo, &H3ED51C, thispal
	//Get FileNo, &H31C72E, fakepal
	//If (header = "PBMXP01") Then Get FileNo, &H3EE1A9, thispal
	//If (header = "PBMXP01") Then Get FileNo, &H31D3BB, fakepal
	//'CopyMemory GbaPalMem(1), fakepal(1), 15
	/*
	For i = 1 To 15
	GbaPalMem(i) = fakepal(i)
	GbaPalMem(i + 16) = thispal(i)
	Next i*/



	//CopyMemory  GBAGraphics::VRAM->fGbuf(2048), g2Buf(0), 4096

	return 0;
}


int TilesetManager::MFGlobalGFX() {




	_gbaMethods->GBAPal[0] = 0;
	MemFile::currentFile->seek(0x40805E);
	MemFile::currentFile->fread(&_gbaMethods->GBAPal[1], 2, 47);

	MemFile::currentFile->seek(0x3F28C8);
	MemFile::currentFile->fread(&GBAGraphics::VRAM->fGbuf[0x800], 1, 4098);

	return 0;

}




//BaseGame::theGame->mainRoom->roomHeader.lBackgroundTSA
int TilesetManager::GetCBG(unsigned long backgroundPointer) {
	new std::exception("GetCBG needs a recode entirely.");
	MemFile* rom = MemFile::currentFile;
	unsigned long palpos = 0;
	unsigned char* buffer = new unsigned char[32192];
	unsigned char*  compBuffer = new unsigned char[64691];

	memset(buffer, 0, 32192);
	unsigned long size = 0;
	int i = 0;
	long pal[256] = { 0 };
	unsigned long points[3] = { 0 };
	int decmpsize = 0;
	rom->seek(backgroundPointer - 0x8000000);
	nMapBuffer * thisbuf = NULL;// RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::BackgroundLayer);
	rom->fread(&thisbuf->size, 1, 1);
	if (thisbuf->size == 0) {
		thisbuf->X = 32;
		thisbuf->Y = 32;
	}
	else if ((thisbuf->size == 1) || (thisbuf->size >= 3)) {
		thisbuf->X = 64;
		thisbuf->Y = 32;
	}
	else if (thisbuf->size == 2) {
		thisbuf->X = 32;
		thisbuf->Y = 64;
	}
	rom->fread(&palpos, 1, 1);
	rom->seek(backgroundPointer - 0x8000000);
	rom->fread(points, 1, 12);

	//MemFile::currentFile->seek(points[0] - 0x8000000);
	//MemFile::currentFile->fread(buffer, 1, 32192);
	//size = _gbaMethods->LZ77UnComp(buffer, (unsigned char*)(_gbaMethods->GBAPal));



	memset(compBuffer, 0, 64691);

	rom->seek(points[1] - 0x8000000);
	rom->fread(compBuffer, 1, 64691);


	decmpsize = _gbaMethods->LZ77UnComp(compBuffer, buffer);
	thisbuf->oSize = decmpsize;

	memset(thisbuf->TileBuf2D, 0, 0x20000);
	memcpy(thisbuf->TileBuf2D, buffer, thisbuf->oSize);

	rom->seek(points[2] - 0x8000000);
	//Seek the compreGlobalVars::gblVars->SSEd Tileset
	rom->fread(compBuffer, sizeof(char), 32691);//Read compression
	size = _gbaMethods->LZ77UnComp(compBuffer, buffer);
	memcpy(&GBAGraphics::VRAM->BGBuf[0x7DE0 - size], buffer, size);


	BGImage->Load(GBAGraphics::VRAM->BGBuf, 1024);
	delete[] buffer;

	delete[] compBuffer;
	//BGImage->
	return 0;
}