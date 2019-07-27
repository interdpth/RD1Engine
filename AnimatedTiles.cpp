#include <vector>
#include "GameConfiguration.h"
#include "..\MemFile\MemFile.h"
#include "AnimatedTiles.h"
#include "GBAGraphics.h"

///
///
///
AnimatedTiles::AnimatedTiles(int crt)
{
	currentRomType = crt;
	specialEffects = NULL;
}


AnimatedTiles::~AnimatedTiles()
{
	delete[] specialEffects;
}


bool AnimatedTiles::Animate()
{
	if (specialEffects == NULL)
	{
		return false;
	}

	bool returnVal = false;
	for (int effectCounter = 0; effectCounter < 16; effectCounter++) {

		SE* thisEffect = &specialEffects[effectCounter];
		if (thisEffect->NumFrames == 0)
		{
			continue;
		}
		

		if (thisEffect->animCounter == thisEffect->Timer)
		{
			thisEffect->animCounter = 0;
			thisEffect->_curFrame++;
			returnVal = true;
		}
		else 
		{
			thisEffect->animCounter++;
			returnVal = false;

		}

		if (thisEffect->_curFrame >= thisEffect->NumFrames)
		{
			thisEffect->animCounter = 0;
			thisEffect->_curFrame = 0;
			returnVal = true;
		}		
	}
	return returnVal;
}

void AnimatedTiles::GetGFX()
{

	long  ref_offset = 0;
	long  gfx_destination, effectCounter, pointer = 0;


	unsigned char effectGraphic[128];

	for (effectCounter = 0; effectCounter < 16; effectCounter++) {

		//if(    ref_array[i * 3] == 0) continue; 

		gfx_destination = effectCounter * 0x80; //if 0 is bufferstart...
		if (specialEffects[effectCounter].GraphicPnt < 0x7FFFFFF) {
			continue;
		}
		/*	if (SpecialEffects[effectCounter].GraphicEnd != 0 || SpecialEffects[effectCounter].Timer != 0) {
		continue;
		}*/
		MemFile::currentFile->seek(specialEffects[effectCounter].GraphicPnt - 0x8000000 + (128 * specialEffects[effectCounter]._curFrame));


		MemFile::currentFile->fread(&effectGraphic[0], sizeof(unsigned char), 128);
		for (int graphicSize = 0; graphicSize < 128; graphicSize++)
		{
			GBAGraphics::VRAM->fGbuf[gfx_destination + graphicSize] = effectGraphic[graphicSize];
		}

	}
	//Copy from Tilemem to BGMem

}


void AnimatedTiles::ReadTable(int effectNum)
{
	MemFile* currentFile = MemFile::currentFile;
	long  reference_table, tileset_table, ref_offset = 0;
	long  ani_source, gfx_destination, effectCounter, pointer = 0;

	if (specialEffects)
	{
		delete[] specialEffects;
	}
	specialEffects = new SE[16];

	unsigned char ref_array[0x30] = { 0 };


	DataContainer* animReferenceTable = GameConfiguration::mainCFG->GetDataContainer("AnimReference");

	DataContainer* animTileset = GameConfiguration::mainCFG->GetDataContainer("AnimTileset");


	reference_table = animReferenceTable->Value;
	tileset_table = animTileset->Value;

	///if(!PreLoad){

	currentFile->seek(reference_table + ((effectNum & 0xFF) * 0x30));
	currentFile->fread(&ref_array, sizeof(unsigned char), 0x30);

	for (effectCounter = 0; effectCounter < 16; effectCounter++) {

		//if(    ref_array[i * 3] == 0) continue; 
		//if not last animation, but no animation there, exit)
		ani_source = (tileset_table + (8 * ref_array[effectCounter * 3]));
		memset(&specialEffects[effectCounter], 0, sizeof(SE));
		currentFile->seek(ani_source);
		currentFile->fread(&specialEffects[effectCounter].Animated, sizeof(unsigned char), 1);
		currentFile->fread(&specialEffects[effectCounter].Timer, sizeof(unsigned char), 1);
		currentFile->fread(&specialEffects[effectCounter].NumFrames, sizeof(unsigned short), 1);
		currentFile->fread(&specialEffects[effectCounter].GraphicPnt, sizeof(unsigned long), 1);
	}
	//Copy from Tilemem to BGMem
}

int AnimatedTiles::LoadSpecialEffects(long EffectNum) {

	GetGFX();
	
	return 0;
}
