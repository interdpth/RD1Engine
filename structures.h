#pragma once
#include <vector>
#include <Windows.h>
using namespace std;
struct MousePointer {
	unsigned char sX;
	unsigned char sY;
	unsigned char eX;
	unsigned char eY;
	unsigned char cX;
	unsigned char cY;
	unsigned char nX;
	unsigned char nY;
	unsigned char uX;
	unsigned char uY;
	unsigned char Width;
	unsigned char Height;

};
enum SaveOptions
{
	OFFSET,
	EXTERNALFILE,
	CHEADER
};
struct sTilesetBlockStructure {
	int max;
	unsigned short ID;
	unsigned short nTSA[0x1080];
};

struct gTileData {

	long gTiles;
	long pTiles;
	long gBackground;
	long TSAMap;
	long EffectSet;

};

struct SE {
	unsigned char Animated;
	unsigned char Timer;
	unsigned short NumFrames;

	long GraphicPnt;


	int _curFrame;
	int animCounter;
};


struct RHeader {
	unsigned char bTileset;
	unsigned char bBg0;
	unsigned char bBg1;
	unsigned char bBg2;
	unsigned long lBg3;
	unsigned long lForeground;
	unsigned long lLevelData;
	unsigned long lBackLayer;
	unsigned long lClipData;
	unsigned long lBackgroundTSA;
	unsigned char bUnknown1;
	unsigned char TransForeground;//Foreground Transparency
	unsigned short iSep1;
	unsigned long lSpritePointer;
	unsigned char bSpriteIndex1;
	unsigned char bEventSwitch;
	unsigned short iSep2;
	unsigned long lSpritePointer2;
	unsigned char bSpriteIndex2;
	unsigned char bEventSwitch2;
	unsigned short iSeperator;
	unsigned long lSpritePointer3;
	unsigned char bSpriteIndex3;
	unsigned char bMiniMapRoomX;
	unsigned char bMiniMapRoomY;
	unsigned char bEffect;
	unsigned char bSceneryYPos;
	unsigned char bNothing;
	unsigned char bMusic;
	unsigned char blank;
};




struct RectBuff {
	long Y, X;
	long StartX;
	long StartY;
	long oX;
	long oY;
	long EndX;
	long EndY;
	unsigned char Started;
};


struct sDoor {
	unsigned char DoorType;
	unsigned char OwnerRoom;
	unsigned char XEntrance;
	unsigned char DWidth;
	unsigned char YEntrance;
	unsigned char DHeight;
	unsigned char DestDoor;
	unsigned char xExitDistance;
	unsigned char yExitDistance;
	unsigned char doorNum;
	unsigned char u3;
	unsigned char u4;
};













struct nEnemies {
	unsigned char Y;
	unsigned char X;
	unsigned char Creature;
};



struct  nEnemyList {
	int oldCount;
	unsigned char Max() { return (unsigned char)Enemies.size(); };
	vector<nEnemies> Enemies;// nEnemies Enemies[0x20];
};


struct CompHeader {
	unsigned char check_ten;       //Check byte should be 0x10 for lz
	unsigned char  l3;        //length = l3*0x10000 + l2*0x100 + l1
	unsigned char  l2;
	unsigned char  l1;
};

struct sprite_entry {
	unsigned char spriteID;
	unsigned char sprdetail;
};


struct sSpriteTable {
	unsigned long Pointers;
	unsigned long Frame;

};


struct sTileMacros {
	unsigned char Clipdata;
	unsigned short ForegroundTile;
	unsigned short BacklayerTile;
};








struct ZMS {


	unsigned short health;
	unsigned short Dmg;
	unsigned short resistance;
	unsigned short unknown;
	unsigned short smallHP;
	unsigned short largehp;
	unsigned short missiles;
	unsigned short supermissiles;
	unsigned short PowerBombs;

};
struct MFS {
	unsigned short Health;
	unsigned short Damage;
	unsigned short unknownDamage;
	unsigned char resistance;
	unsigned char  unknown6;
	unsigned short unknown1;
	unsigned short unknown2;
	unsigned short unknown3;
	unsigned short end;//Always 0?
};




struct sSpritev {
	unsigned char RAM[0x10000];
};



struct GFXData {
	unsigned long  RomPointer;
	unsigned long  MemDst;
};

struct PalData {
	unsigned long RomPointer;
	unsigned long MemDst;
	unsigned long Size;
};



struct SpritesetData
{
	unsigned long usedGFX;
	unsigned long usedPAL;
	unsigned char  graphics[0x8000];

	long pal[512];
	unsigned char total;
};


struct RECORD {
	unsigned char	*data;
	unsigned long	width;
	unsigned long	height;
};

class wndCPal {
	HWND me;
	long epal[256];
	int  refreshWnd(HWND hwnd);
	int  hascontrol;
	int  GiveControl();


};





