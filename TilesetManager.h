#pragma once

//#include "MainHeader.h"
//#include "GlobalVars.h"
#include "AnimatedTiles.h"
#include "GBAGraphics.h"
//#include "BaseGame.h"
#include "image.h"
#include "GBAMethods.h"
#include "BaseGame.h"
class TilesetManager
{
public:
	sTilesetBlockStructure        TSA;
	/*gTileData   RoomTilesets[0x50];
	gTileData   RoomTilesets[0x80];*/
	vector<gTileData> RoomTilesets;
	TileBuffer* TileImage;
	//gTileData* GetTileset(int TilesetNum) {
	//	
	//	return &RoomTilesets[TilesetNum];;
	//}
	AnimatedTiles* animTiles;
	GBAMethods* _gbaMethods;
	void GetBaseTileset(unsigned char TilesetVal);
	void ReadTable();
	void UpdateEffects();
	void RefreshTileset();
	void ReadTSA(gTileData* tileData);
	void GetPal(gTileData* tileset);
	void GetBaseGFX(gTileData* tileset);
	TileBuffer* BGImage;
	int currentRomType;
	//SE SpecialEffects[16];
	TilesetManager(GBAMethods* gba, int crf, TileBuffer * bg, TileBuffer* TileImage);
	~TilesetManager();
	int GetTileset(Image* dst, unsigned char TilesetVal, int bg3);
	int DrawBlock(Image* src, short Map16n, int destX, int destY);
	void DrawTileset(Image** imgTileset);
	int ZMGlobalGFX(unsigned char Area);
	int MFGlobalGFX();
	int GetCBG(unsigned long backgroundPointer);
	int SaveTileset(unsigned char TilesetVal);
	int SaveTilesetPal(int romSwitch, unsigned char TilesetVal);
	int LoadSpecialEffects(long EffectNum);
	int GetBackground(gTileData* thisTileset);
	void Render(Image* srcImage);
};

