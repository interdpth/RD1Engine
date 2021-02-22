#include "clsPreviewSprite.h"

clsPreviewSprite::clsPreviewSprite()
{
	_fullCache.created = false;
	AddressesSet = 0;
	Height = 0;
	Width = 0;
	LeakFinder::finder->AddLeak("clsPreviewSprite()", this);
}


clsPreviewSprite::~clsPreviewSprite()
{
	_fullCache.Destroy();
	LeakFinder::finder->RemoveLeak("clsPreviewSprite()", this);
}


void clsPreviewSprite::Create(int width, int height)
{
	Width = width;
	Height = height;	
	if (!_fullCache.created) {
		_fullCache.Create(width, height);
		_fullCache.created = true;
	}
}



Image* clsPreviewSprite::GetFullImage()
{
	return &_fullCache;
}


void clsPreviewSprite::SetPalette(long* pal, int first, int count)
{

	_fullCache.SetPalette(pal, first, count);
}

void		clsPreviewSprite::SetPaletteRaw(const BYTE* pal, int first, int count)
{
	_fullCache.SetPaletteRaw(pal, first, count);
}

void  clsPreviewSprite::RefreshImage()
{
	
}



void		clsPreviewSprite::Draw(tile_t tile, long dstx, long dsty, int palette, DRAWFLAGS flgs, int forceclr)
{
	
	_fullCache.Draw(tile, dstx, dsty, palette, flgs, forceclr);
	
}
void		clsPreviewSprite::Draw(TileBuffer& tilebuf, long dstx, long dsty, int pftt, int forceclr)
{
	
	_fullCache.Draw(tilebuf, dstx, dsty, pftt, forceclr);
	
}

//Blit whole image
void        clsPreviewSprite::StretchBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy, int srcw, int srch)
{
	GetFullImage()->StretchBlit(dc, dstx, dsty, width, height, srcx, srcy, srcw, srch);
}
void		clsPreviewSprite::Blit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy)
{
	Image* thisImage = GetFullImage();
	if (thisImage)
	{
		thisImage->Blit(dc, dstx, dsty, width, height, srcx, srcy);
	}
}
void		clsPreviewSprite::TransBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy)
{
	GetFullImage()->TransBlit(dc, dstx, dsty, width, height, srcx, srcy);
}
void        clsPreviewSprite::AlphaBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy, int a)
{
//	GetFullImage()->AlphaBlit(dc, dstx, dsty, width, height, srcx, srcy, a);
}