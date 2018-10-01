#pragma once

#include <Windows.h>
#include "LeakFinder.h"
#include "image.h"
class clsPreviewSprite
{
private:
	
public:
	clsPreviewSprite();
	~clsPreviewSprite();
	
	Image _fullCache;
	//
	//
	Image* GetFullImage();
	int  IsCreated();
	void RefreshImage();
	int Width;
	int Height;


	bool AddressesSet;

//Extends image <3
public:
	void		SetPalette(long* pal, int first = 0, int count = 256);
	void		SetPaletteRaw(const BYTE* pal, int first = 0, int count = 256);
	void Create(int width, int height);

	// Draw -- draws tiles to this image
	void		Draw(tile_t tile, long dstx, long dsty, int palette = 0, DRAWFLAGS flgs = 0, int forceclr = -1);
	void		Draw(TileBuffer& tilebuf, long dstx, long dsty, int pftt, int forceclr = -1);


	// Blit -- Blits this image to an HDC (actually display on screen)
	void        StretchBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy, int srcw, int srch);
	void		Blit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy);
	void		TransBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy);
	void        AlphaBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy, int a);
	
	//debug
public:
	BYTE* layerPixelsPoint[8];

	void SetAddresses();
	void CheckAddresses();

};

