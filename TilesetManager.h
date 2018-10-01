#pragma once

//#include "MainHeader.h"
//#include "GlobalVars.h"
#include "AnimatedTiles.h"
#include "GBAGraphics.h"
//#include "BaseGame.h"
#include "image.h"

class TilesetManager
{
public:
	sTilesetBlockStructure        TSA;
	/*gTileData   RoomTilesets[0x50];
	gTileData   RoomTilesets[0x80];*/
	vector<gTileData> RoomTilesets;
	//gTileData* GetTileset(int TilesetNum) {
	//	
	//	return &RoomTilesets[TilesetNum];;
	//}
	AnimatedTiles* animTiles;
	void GetBaseTileset(unsigned char TilesetVal);
	void ReadTable();
	void UpdateEffects();
	void RefreshTileset();
	void ReadTSA(gTileData* tileData);
	void GetPal(gTileData* tileset);
	void GetBaseGFX(gTileData* tileset);
	//SE SpecialEffects[16];
	TilesetManager();
	~TilesetManager();
	int GetTileset(unsigned char TilesetVal, int bg3);
	int DrawBlock(Image* src, short Map16n, int destX, int destY);
	void DrawTileset();
	int ZMGlobalGFX(unsigned char Area);
	int MFGlobalGFX();
	int GetCBG(unsigned long backgroundPointer);
	int SaveTileset(unsigned char TilesetVal);
	int SaveTilesetPal(int romSwitch, unsigned char TilesetVal);
	int LoadSpecialEffects(long EffectNum);
	int GetBackground(gTileData* thisTileset);
	void Render();
};

