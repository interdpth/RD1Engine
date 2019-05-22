#include "cEnitityManager.h"
#include "cOAMManager.h"
//#include "cSSE.h"


cEntityManager::cEntityManager(GBAMethods *methods)
{
	_gbaMethods = methods;
//	gfxinfo = new GFXData[16];
	gfxinfo.resize(16);
	palinfo = new PalData[16];
	spriteset = new sprite_entry[16];
}


cEntityManager::~cEntityManager()
{
	delete[] palinfo;
	delete[] spriteset;
}
int cEntityManager::MFLoadSet(bool ReadObjectDetailsFromROM, vector<GFXData>* spritedata, PalData*Palettedata, sprite_entry* SpriteInfo, unsigned char SpriteSetNo)
{

	int ii = 0, X = 0, i = 0;
	unsigned char enemydat[15] = { 0 };
	unsigned char destination[15] = { 0 };
	unsigned long off = 0;
	spritedata->clear();
	spritedata->resize(16);
	if (ReadObjectDetailsFromROM == true) {


		for (ii = 0; ii < 15; ii++)
		{
			enemydat[ii] = 16;
			destination[ii] = 0;
		}

		for (X = 0; X < 15; X++) {
			Palettedata[X].RomPointer = Palettedata[X].MemDst = Palettedata[X].Size = 0;
			SpriteInfo[X].spriteID = SpriteInfo[X].sprdetail = 0;
			spritedata->at(X).RomPointer = spritedata->at(X).MemDst = 0;

		}
		RD1Engine::theGame->titleInstance->SeekSpriteTable(SpriteSetNo);

		for (i = 0; i < 15; i++) {

			MemFile::currentFile->fread(&SpriteInfo[i].spriteID, 1, 1);
			MemFile::currentFile->fread(&SpriteInfo[i].sprdetail, 1, 1);
		}


	}
	//	unsigned int BaseGame::theGame->mgrOAM->sprite_in[X].sprtype= BaseGame::theGame->mgrOAM->sprite_in[ii].sprtype, sprdetail= BaseGame::theGame->mgrOAM->sprite_in[ii].sprdetail, 
	int prevsprdetail = 255;
	RD1Engine::theGame->mgrOAM->maxsprite = 0;
	////if (GlobalVars::gblVars->SSE == true)cSSE::SpriteSet->total = 0;
	for (X = 0; X < 15; X++)
	{
		sprite_entry* sprite_in = &SpriteInfo[X];

		////if (GlobalVars::gblVars->SSE == true)cSSE::SpriteSet->total++;
		if ((sprite_in->spriteID == 0) && (X != 0)) break;
		RD1Engine::theGame->mgrOAM->maxsprite++;
		enemydat[X] = sprite_in->spriteID;
		destination[X] = sprite_in->sprdetail & 8;	 // ???

		if ((sprite_in->sprdetail != prevsprdetail) && (sprite_in->sprdetail != 8))
		{

			spritedata->at(X).RomPointer = GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (SpriteInfo[X].spriteID - 0x10) * 4;
			spritedata->at(X).MemDst = (SpriteInfo[X].sprdetail * 0x800);


			Palettedata[X].RomPointer = GameConfiguration::mainCFG->GetDataContainer("SpritePal")->Value + (SpriteInfo[X].spriteID - 0x10) * 4;
			if (SpriteInfo[X].sprdetail & 7)Palettedata[X].MemDst = (SpriteInfo[X].sprdetail * 16);

			Palettedata[X].Size = (RD1Engine::theGame->mgrOAM->MFSprSize[(SpriteInfo[X].spriteID - 0x10) << 1] / 128);
		}
		prevsprdetail = SpriteInfo[X].sprdetail;
	}




	return X;
}


int cEntityManager::LoadSet(bool ReadObjectDetailsFromROM, vector<GFXData>* spritedata, PalData*Palettedata, sprite_entry* SpriteInfo, unsigned char SpriteSetNo) {

	long off = 0;
	int X, i, compare;
	CompHeader thiscompheader;
	//sprite_entry tempit[15]; 
	unsigned char enemydat[15];
	unsigned char destination[15];
	unsigned char max = 0;



	for (X = 0; X < 15; X++) {
		enemydat[X] = 0x10;
		destination[X] = 0x00;
	}

	for (X = 0; X < 15; X++) {
		Palettedata[X].RomPointer = Palettedata[X].MemDst = Palettedata[X].Size = 0;
	/*	if (ReadObjectDetailsFromROM == false)
			SpriteInfo[X].spriteID = SpriteInfo[X].sprdetail = 0;

*/
	}
	if (ReadObjectDetailsFromROM == false) {
		RD1Engine::theGame->titleInstance->SeekSpriteTable(SpriteSetNo);

		for (i = 0; i < 15; i++) {
			MemFile::currentFile->fread(&SpriteInfo[i].spriteID, 1, 1);
			MemFile::currentFile->fread(&SpriteInfo[i].sprdetail, 1, 1);
		}
	}
	compare = 0xFF;

	for (X = 0; X < 15; X++) {

		if (SpriteInfo[X].spriteID == 0) {
			max = X;
			if (X != 0) {
				break;                                  // end this if any type but the first =0
			}
			else {
				max = X;
				continue;                                  //if first type =0 then continue
			}
		}

		enemydat[X] = SpriteInfo[X].spriteID;                  //type is written to enemydata
		destination[X] = (SpriteInfo[X].sprdetail & 0x7);   //3 least significant bits to destination

		if (SpriteInfo[X].sprdetail == compare)
			continue; //compare detail# to last detail#

		compare = SpriteInfo[X].sprdetail;                    //store last detail#

		if (SpriteInfo[X].sprdetail == 0x8)
			continue;      //if detail# = 8, dont load gfx & pals

						   //gfx and palette loading
		spritedata->at(X).RomPointer = GameConfiguration::mainCFG->GetDataContainer("SpriteGFX")->Value + (SpriteInfo[X].spriteID - 0x10) * 4;
		spritedata->at(X).MemDst = (SpriteInfo[X].sprdetail * 0x800);
		MemFile::currentFile->seek(spritedata->at(X).RomPointer);
		MemFile::currentFile->fread(&off, 4, 1);
		MemFile::currentFile->seek(off - 0x8000000);
		MemFile::currentFile->fread(&thiscompheader.check_ten, 1, 1);       //Check byte should be 0x10 for lz
		MemFile::currentFile->fread(&thiscompheader.l3, 1, 1);
		MemFile::currentFile->fread(&thiscompheader.l2, 1, 1);
		MemFile::currentFile->fread(&thiscompheader.l1, 1, 1);

		Palettedata[X].RomPointer = GameConfiguration::mainCFG->GetDataContainer("SpritePal")->Value + (SpriteInfo[X].spriteID - 0x10) * 4;
		Palettedata[X].MemDst = (SpriteInfo[X].sprdetail * 16);
		Palettedata[X].Size = (thiscompheader.l2 & 0xF8) * 2;

	}
	//if (GlobalVars::gblVars->SSE) {
	//	cSSE::SpriteSet->total = max;
	//}
	//else {
	RD1Engine::theGame->mgrOAM->maxsprite = max;
	//}

	return max;
}


int cEntityManager::LoadPal(PalData* palinfo, sprite_entry* spriteset, long *Palette) {
	if (&palinfo == 0 || &spriteset == 0) return 0;
	unsigned char max = 0;
	max = RD1Engine::theGame->mgrOAM->maxsprite;
	//if (GlobalVars::gblVars->SSE == true)max = cSSE::SpriteSet->total;
	long  addybuf = 0;
	unsigned short transferpal[1024] = { 0 };
	char osh[256] = { 0 };
	int  x = 0;
	memset(Palette, 0, 512);
	//cSSE::SpriteSet->usedPAL = 0;
	//	memset(&GBAcSprPal[128],0,sizeof( GBAGraphics::VRAM->GBASprPal)-128);
	memset(&GBAGraphics::VRAM->SprPal, 0, 0x4000);


	for (x = 0; x < max; x++) {
		if (palinfo[x].RomPointer != 0) {
			unsigned char thisSprite = spriteset[x].spriteID;
			if (x != 0 && palinfo[x].RomPointer == 0) break;

			MemFile::currentFile->seek(palinfo[x].RomPointer);
			MemFile::currentFile->fread(&addybuf, 4, 1);
			MemFile::currentFile->seek(addybuf - 0x8000000);
			MemFile::currentFile->fread(&transferpal, 1, (palinfo[x].Size) * 2);

			sprintf(osh, "Dst is: %X", palinfo[x].MemDst);

			if (128 + (palinfo[x].MemDst) < 256)
			{
				memcpy(&GBAGraphics::VRAM->GBASprPal[128 + (palinfo[x].MemDst)], &transferpal, (palinfo[x].Size) * 2);
			}
			
		}

	}
	_gbaMethods->DecodePal(GBAGraphics::VRAM->GBASprPal, Palette, 16, 0);
	return 0;
}

