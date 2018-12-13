#pragma once
#include "tilebuffer.h"
#include <stdio.h>
#include "OpenGL2.h"
#include <gl/gl.h>
#include <gl/glu.h>

#include "FreeImage.h"
typedef	unsigned int		pal_t;

typedef int			DRAWFLAGS;
#define FLIPX		0x01
#define FLIPY		0x02
#define NOTRANS		0x04

enum RenderType
{
	GDI,
	OPENGL,
};

class Image
{
public:
	long* originalPal;
	bool _isGL;
	Image(HWND targetWindow,int width, int height);
	Image(int width, int height);
	COpenGL* glInstance;
	
	static  Image* Import(char* apppath, char* filename);
				Image();
				~Image();
    int			Height;
	int         Width;
	void		Destroy();
	void		Create(int width,int height);
	void        Create(int width, int height, long* pal);
	void		Clear();
	void		Clear(int x,int y,int w,int h);
	void		Fill(int x,int y,int w,int h,BYTE color);

	void		SetPalette(long* pal,int first = 0,int count = 256);
	void		SetPaletteRaw(const BYTE* pal,int first = 0,int count = 256);
    
	// Draw -- draws tiles to this image
	void		Draw(tile_t tile,long dstx,long dsty,int palette = 0,DRAWFLAGS flgs = 0,int forceclr=-1);
	void		Draw(TileBuffer& tilebuf,long dstx,long dsty,int pftt,int forceclr=-1);
	static  int ClosestColor(unsigned short* colors, unsigned short rgb);
	static unsigned char*  GetGfx(unsigned short* pal, unsigned short* imgData, bool trimBlankTiles, int width, int height);
	static 	unsigned short* GetImageData(RGBQUAD* baseImage, int width, int height);
	void DrawGL();
	// Blit -- Blits this image to an HDC (actually display on screen)
	void        StretchBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy, int srcw, int srch);
	void		Blit(HDC dc,int dstx,int dsty,int width,int height,int srcx,int srcy);
	void		TransBlit(HDC dc,int dstx,int dsty,int width,int height,int srcx,int srcy);
	void        AlphaBlit(HDC dc,int dstx,int dsty,int width,int height,int srcx,int srcy,int alow, int ahigh);
	HDC         DC(); 
	int  IsCreated();
	HDC			mDC;
    void SaveToFile(FILE* file);
	BYTE*		pixels;
	BYTE*		maskpixels;
	bool created;
protected:
	void		Zero();
	void		RebuildMask();
	void DestroyImage(HDC dc, HBITMAP bmpold, HBITMAP bmp);

	HBITMAP		hBmp;
	HBITMAP		hBmpOld;

	int			nWidth;
	int			nHeight;
	int			nPitch;

	HDC			mMaskDC;
	HBITMAP		hMaskBmp;
	HBITMAP		hMaskBmpOld;
	
	int			nMaskPitch;

	int			bMaskValid;
};