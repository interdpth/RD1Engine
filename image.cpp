
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Image.h"
#include <vector>
#include "grit.h"
#include <map>
#include "GBAMethods.h"
#include "GritInterface.h"

#include <shellapi.h>
int wasDelted = 0;
LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT *pNumArgs)
{
	int retval;
	retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, NULL, 0);
	if (!SUCCEEDED(retval))
		return NULL;

	LPWSTR lpWideCharStr = (LPWSTR)malloc(retval * sizeof(WCHAR));
	if (lpWideCharStr == NULL)
		return NULL;

	retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, lpWideCharStr, retval);
	if (!SUCCEEDED(retval))
	{
		free(lpWideCharStr);
		return NULL;
	}

	int numArgs;
	LPWSTR* args;
	args = CommandLineToArgvW(lpWideCharStr, &numArgs);
	free(lpWideCharStr);
	if (args == NULL)
		return NULL;

	int storage = numArgs * sizeof(LPSTR);
	for (int i = 0; i < numArgs; ++i)
	{
		BOOL lpUsedDefaultChar = FALSE;
		retval = WideCharToMultiByte(CP_ACP, 0, args[i], -1, NULL, 0, NULL, &lpUsedDefaultChar);
		if (!SUCCEEDED(retval))
		{
			LocalFree(args);
			return NULL;
		}

		storage += retval;
	}

	LPSTR* result = (LPSTR*)LocalAlloc(LMEM_FIXED, storage);
	if (result == NULL)
	{
		LocalFree(args);
		return NULL;
	}

	int bufLen = storage - numArgs * sizeof(LPSTR);
	LPSTR buffer = ((LPSTR)result) + numArgs * sizeof(LPSTR);
	for (int i = 0; i < numArgs; ++i)
	{
		_ASSERT(bufLen > 0);
		BOOL lpUsedDefaultChar = FALSE;
		retval = WideCharToMultiByte(CP_ACP, 0, args[i], -1, buffer, bufLen, NULL, &lpUsedDefaultChar);
		if (!SUCCEEDED(retval))
		{
			LocalFree(result);
			LocalFree(args);
			return NULL;
		}

		result[i] = buffer;
		buffer += retval;
		bufLen -= retval;
	}

	LocalFree(args);

	*pNumArgs = numArgs;
	return result;
}
Image::Image(int width, int height)
{
	_isGL = false;
	glInstance = NULL;
	pixels = NULL;
	Zero();
	Create(width, height);
}
Image* Image::Import(char* apppath, char* filename)
{

	char param[1024];
	char heyman[2014] = { 0 };
	GetCurrentDirectory(1023, heyman);
	sprintf(param, "\"%s\" \"%s\\%s\" -gt -gu 8 -gB4 -mLs -MRp -mRtpf -ftm", heyman, heyman, filename);
	int numargs = 7;
	char** params = CommandLineToArgvA(param, &numargs);
	//char radiostring[3][32] = { "-Ah 32 -Aw 32","-Ah 64 -Aw 32","-Ah 32 -Aw 64" };

	GritInterface * newInterface = new GritInterface(NULL);



	GritRec * rec = newInterface->run_main(numargs, params);
	LocalFree(params);

	int width = rec->areaRight - rec->areaLeft;
	int height = rec->areaBottom - rec->areaTop;

	short rawPal[512] = { 0 };

	memcpy(rawPal, (short*)rec->_palRec.data, rec->_palRec.width*rec->_palRec.height);
	long pcPal[512] = { 0 };
	(new GBAMethods())->DecodePal(rawPal, pcPal, rec->_palRec.width, 0);
	Image* tmpImage = new Image();;
	tmpImage->Create(width, height);
	tmpImage->SetPalette(pcPal);
	TileBuffer tb;
	tb.Load((unsigned char*)rec->_gfxRec.data, (rec->_gfxRec.height* rec->_gfxRec.width) / 32);
	unsigned short* tileMap = (unsigned short*)rec->_mapRec.data;
	for (int thisX = 0; thisX < (width / 8); thisX++) {// from here if something is enabled then draw it 

		for (int thisY = 0; thisY < (height / 8); thisY++) {

			unsigned short TILE = tileMap[thisX + (thisY * (width / 8))];

			tmpImage->Draw(tb, thisX * 8, thisY * 8, TILE);
		}
	}


	return tmpImage;
}
Image::Image(HWND targetWindow, int width, int height)
{
	_isGL = true;
	glInstance = NULL;
	pixels = NULL;
	Zero();

	pixels = new unsigned char[width*height * 4];
	memset(pixels, 0, width*height * 4);
	this->Width = width;
	Height = height;
	nHeight = height;
	nWidth = width;
	nPitch = ((width + 3) / 4) * 4;
	//Create(width, height);
	glInstance = new COpenGL(targetWindow, width, height, pixels);
}

Image::Image()
{
	_isGL = false;
	glInstance = NULL;
	Zero();
}

Image::~Image()
{
	Destroy();
	Zero();

	if (maskpixels != NULL)
	{
		delete[] maskpixels;
	}
	if (maskpixels != NULL)
	{
		delete[] maskpixels;
	}
	if (pixels != NULL)
	{
		delete[] pixels;
	}
	if (pixels != NULL)
	{
		delete[] pixels;
	}
	wasDelted++;
}

int wasdelted = 0;

void Image::DestroyImage(HDC dc, HBITMAP bmpold, HBITMAP bmp)
{
	if (dc)
	{
		DeleteDC(dc);
		SelectObject(dc, bmpold);
		DeleteDC(dc);
		DeleteObject(bmp);
		DeleteObject(bmpold);
		DeleteDC(dc);
		DeleteObject(bmpold);

		DeleteDC(dc);
	}
}

void Image::Destroy()
{	
	if (!_isGL) {
		DestroyImage(mDC, hBmpOld, hBmp);
		DestroyImage(mMaskDC, hMaskBmpOld, hMaskBmp);

		mDC = 0;
		hBmpOld = 0;
		hBmp = 0;
		pixels = NULL;

		mMaskDC = 0;
		hMaskBmpOld = 0;
		hMaskBmp = 0;
		maskpixels = NULL;
	}
	else
	{
		delete[] pixels;
	}
}
void Image::Zero()
{

	hBmp = NULL;
	hBmpOld = NULL;


	mDC = NULL;

	pixels = NULL;
	nWidth = NULL;
	nHeight = NULL;

	mMaskDC = (HDC)NULL;
	hMaskBmp = NULL;
	hMaskBmpOld = NULL;
	maskpixels = NULL;
	nMaskPitch = NULL;

	bMaskValid = NULL;
}

void Image::Clear()
{
	Fill(0, 0, nWidth, nHeight, 0);
}

void Image::Clear(int x, int y, int w, int h)
{
	Fill(x, y, w, h, 0);
}


void Image::SetPalette(long* pal, int first /* = 0 */, int count /* = 256 */)
{
	if (!mDC)
		return;

	RGBQUAD* qd = new RGBQUAD[count];
	int i;
	for (i = 0; i < count; ++i)
	{
		qd[i].rgbBlue = (pal[i] >> 16) & 0xFF;
		qd[i].rgbGreen = (pal[i] >> 8) & 0xFF;
		qd[i].rgbRed = (pal[i]) & 0xFF;
	}

	SetDIBColorTable(mDC, first, count, qd);
	delete[] qd;
}

void Image::SetPaletteRaw(const BYTE* pal, int first /* = 0 */, int count /* = 256 */)
{
	if (!mDC)
		return;

	RGBQUAD* qd = new RGBQUAD[count];
	WORD tmp;
	int i;
	for (i = 0; i < count; ++i)
	{
		tmp = pal[0] | (pal[1] << 8);
		qd[i].rgbBlue = ((tmp >> 10) & 0x1F) * 0xFF / 0x1F;
		qd[i].rgbGreen = ((tmp >> 5) & 0x1F) * 0xFF / 0x1F;
		qd[i].rgbRed = ((tmp) & 0x1F) * 0xFF / 0x1F;

		pal += 2;
	}

	SetDIBColorTable(mDC, first, count, qd);
	delete[] qd;
}

void Image::Fill(int x, int y, int w, int h, BYTE color)
{
	if (!mDC)
		return;

	if ((x + w) > nWidth)		w = (nWidth - x);
	if ((y + h) > nHeight)		h = (nHeight - y);

	w += x;
	h += y;//x;

	if (x < 0)		x = 0;
	if (y < 0)		y = 0;

	int y2;
	for (y2 = y; y2 < h; ++y2)
	{
		/*for (x2 = x; x2 < w; ++x2)
		{*/
		//	pixels[(y2*nPitch) + x2] = color;
		memset(&pixels[(y2*nPitch)], color, nPitch);
		//}
	}

	bMaskValid = 0;
}

int created = 0;
int deleted = 0;


void Image::Create(int width, int height)
{
	if (mDC)
	{
		printf("he;p");
	}
	Destroy();

	Zero();
	
	mDC = CreateCompatibleDC(0);
	if (!mDC)
		return;

	BITMAPINFO		bi;
	memset(&bi, 0, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biHeight = -height;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biWidth = width;
	bi.bmiHeader.biBitCount = 8;			// 8 bit, sucka!
	bi.bmiHeader.biCompression = BI_RGB;
	hBmp = CreateDIBSection(mDC, &bi, DIB_RGB_COLORS, (void**)(&pixels), 0, 0);

	if (!hBmp)
	{
		Destroy();
		return;
	}

	hBmpOld = (HBITMAP)SelectObject(mDC, hBmp);
	Width = width;
	Height = height;
	nWidth = width;
	nHeight = height;
	this->nPitch = ((width + 3) / 4) * 4;

	// make the mask
	mMaskDC = CreateCompatibleDC(0);
	if (!mMaskDC)
	{
		Destroy();
		return;
	}
	bi.bmiHeader.biBitCount = 1;			// monochrome mask

	hMaskBmp = CreateDIBSection(mMaskDC, &bi, DIB_RGB_COLORS, (void**)(&maskpixels), 0, 0);
	if (!hMaskBmp)
	{
		Destroy();
		return;
	}
	hMaskBmpOld = (HBITMAP)SelectObject(mMaskDC, hMaskBmp);

	nMaskPitch = ((width + 31) / 32) * 4;

	static const RGBQUAD monopal[2] = { {0,0,0,0}, {255,255,255,0} };
	SetDIBColorTable(mMaskDC, 0, 2, monopal);

	Clear();
	created++;
}

void Image::Create(int width, int height, long* pal)
{


	mDC = CreateCompatibleDC(0);
	if (!mDC)
		return;

	BITMAPINFO		bi;
	memset(&bi, 0, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biHeight = -height;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biWidth = width;
	bi.bmiHeader.biBitCount = 24;			// 8 bit, sucka!
	bi.bmiHeader.biCompression = BI_RGB;
	Height = height;
	Width = width;
	hBmp = CreateDIBSection(mDC, &bi, DIB_RGB_COLORS, (void**)(&pixels), 0, 0);

	if (!hBmp)
	{
		Destroy();
		return;
	}

	hBmpOld = (HBITMAP)SelectObject(mDC, hBmp);

	nWidth = width;
	nHeight = height;
	nPitch = ((width + 3) / 4) * 4;

	// make the mask
	mMaskDC = CreateCompatibleDC(0);
	if (!mMaskDC)
	{
		Destroy();
		return;
	}
	bi.bmiHeader.biBitCount = 1;			// monochrome mask

	hMaskBmp = CreateDIBSection(mMaskDC, &bi, DIB_RGB_COLORS, (void**)(&maskpixels), 0, 0);
	if (!hMaskBmp)
	{
		Destroy();
		return;
	}
	hMaskBmpOld = (HBITMAP)SelectObject(mMaskDC, hMaskBmp);

	nMaskPitch = ((width + 31) / 32) * 4;

	static const RGBQUAD monopal[2] = { { 0,0,0,0 },{ 255,255,255,0 } };
	SetDIBColorTable(mMaskDC, 0, 2, monopal);

	Clear();
	SetPalette(pal);
	created++;
}

HDC   Image::DC()
{
	return mDC;
};
void Image::Draw(tile_t tile, long dstx, long dsty, int palette /* = 0 */, DRAWFLAGS flgs /* = 0 */, int forceclr)//

{

	if (!tile)
		return;
	if (!mDC && !glInstance)
		return;
	/* if(forceclr != -1)
	 {
		 Fill(dstx,dsty,1,1,(BYTE)forceclr);
		 return;
	 }*/
	int orval = 0;
	int xorx = (flgs & FLIPX) ? 7 : 0;
	int xory = (flgs & FLIPY) ? 7 : 0;

	BYTE pal = (BYTE)((palette & 0x0F) << 4);

	int ox = dstx;
	int x, y;
	int x2, y2;
	for (y = 0; (y < 8) && (dsty < nHeight - 1); y++, dsty++)
	{
		y2 = (y ^ xory) * 8;
		dstx = ox;

		for (x = 0; (x < 8) && (dstx < nWidth - 1); x++, dstx++)
		{
			x2 = (x ^ xorx) + y2;
			if (!tile[x2] && !(flgs & NOTRANS))
				continue;

			
			if ((signed)((dsty*nPitch) + dstx) >= 0) {
				if (glInstance)
				{
					char stringtext[1024];
					glInstance->rawImage[(dsty*nPitch) + dstx] = glInstance->rawPalette[pal * 16] | tile[x2];
				}
				else
				{
					pixels[(dsty*nPitch) + dstx] = pal | ((forceclr != -1 && tile[x2] > 1) ? forceclr : tile[x2]);
				}
			}
		}
	}

	bMaskValid = 0;

}

void Image::Draw(TileBuffer& tilebuf, long dstx, long dsty, int pftt, int forceclr)
{

	DRAWFLAGS flgs = 0;
	if (pftt & 0x400)		flgs |= FLIPX;
	if (pftt & 0x800)		flgs |= FLIPY;

	Draw(tilebuf[pftt & 0x3FF], dstx, dsty, pftt >> 12, flgs, forceclr);
}


int  Image::IsCreated() {
	return (mDC != 0);
}
void Image::Blit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy)
{
	if (mDC) {
		BitBlt(dc, dstx, dsty, width, height, mDC, srcx, srcy, SRCCOPY);


	}
}



void Image::StretchBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy, int srcw, int srch)
{
	if (mDC)
	{
		StretchBlt(dc, dstx, dsty, width, height, mDC, srcx, srcy, srcw, srch, SRCPAINT);
	}
	//TransparentBlt(dc,dstx,dsty,width,height,mDC,srcx,srcy,0,0,RGB(0,0,0));
}
void Image::TransBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy)
{
	if (mDC && mMaskDC)
	{
		if (!bMaskValid)
		{
			RebuildMask();
		}
		BitBlt(dc, dstx, dsty, width, height, mMaskDC, srcx, srcy, SRCAND);
		BitBlt(dc, dstx, dsty, width, height, mDC, srcx, srcy, SRCPAINT);
	}
	//TransparentBlt(dc,dstx,dsty,width,height,mDC,srcx,srcy,0,0,RGB(0,0,0));
}


void Image::RebuildMask()
{
	if (!mDC || !mMaskDC)
		return;

	// probably not the fastest way to do this -- but whatever,
	// this isn't going to be called very often, so it doesn't really matter
	int x, y;

	for (y = 0; y < nHeight; ++y)
	{
		for (x = 0; x < nWidth; ++x)
		{
			if (pixels[(y*nPitch) + x])
			{
				maskpixels[(y*nMaskPitch) + (x >> 3)] &= ~(0x80 >> (x & 7));
			}
			else
			{
				maskpixels[(y*nMaskPitch) + (x >> 3)] |= (0x80 >> (x & 7));
			}
		}
	}

	bMaskValid = 1;
}

void Image::SaveToFile(FILE* file)
{
	if (!mDC || !pixels)		// can't save if there's no image
		return;

	/*
	 *	You could probably use BITMAPFILEHEADER and BITMAPINFOHEADER structs here,
	 *    but i hate dumping structs t
	 o a file because there's no guarantee things will align
	 *    properly
	 */

	int tmp;
	short sht;

	// BITMAPFILEHEADER crap
	fputc('B', file);
	fputc('M', file);

	tmp = (nPitch * nHeight) + (256 * 4) + 0x28 + 0x0E;		// file size
	fwrite(&tmp, 1, 4, file);

	tmp = 0;		// reserved BS
	fwrite(&tmp, 1, 4, file);

	tmp = (256 * 4) + 0x28 + 0x0E;		// file size minus bitmap bits
	fwrite(&tmp, 1, 4, file);

	// BITMAPINFOHEADER crap
	tmp = 0x28;
	fwrite(&tmp, 1, 4, file);		// size of BITMAPINFOHEADER
	fwrite(&nWidth, 1, 4, file);
	tmp = -nHeight;				// negative height because we want a top-down bitmap
	fwrite(&tmp, 1, 4, file);

	sht = 1;
	fwrite(&sht, 1, 2, file);		// planes
	sht = 8;
	fwrite(&sht, 1, 2, file);		// bpp

	tmp = 0;
	fwrite(&tmp, 1, 4, file);		// stupid meaningless stuff
	fwrite(&tmp, 1, 4, file);
	fwrite(&tmp, 1, 4, file);
	fwrite(&tmp, 1, 4, file);
	fwrite(&tmp, 1, 4, file);
	fwrite(&tmp, 1, 4, file);


	// palette

	RGBQUAD pal[512] = { 0 };
	GetDIBColorTable(mDC, 0, 512, pal);
	fwrite(pal, 1, 2048, file);

	// bits
	fwrite(pixels, 1, nPitch*nHeight, file);
}


#define ALPHA(rgb) (unsigned int8_t)(rgb >> 24)
#define RED(rgb)   (unsigned int8_t)(rgb >> 16)
#define GREEN(rgb) (unsigned int8_t)(rgb >> 8)
#define BLUE(rgb)  (unsigned int8_t)(rgb)
int UNMULTIPLY(int color, int alpha) {
	return ((0xFF * (color + 1)) / alpha + 1);
}

#define BLEND(back, front, alpha) ((front * alpha) + (back * (255 - alpha))) / 255
#define ARGB(a, r, g, b) (a << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF)

void Image::AlphaBlit(HDC dc, int dstx, int dsty, int width, int height, int srcx, int srcy, int alphaHigh, int alphaLow)
{

	if (!mDC)
		return;
	//if (a <= 0)		return;		// fully transparent
	//if (a >= 0xFF)				// fully opaque
	//{
	//	TransBlit(dc, dstx, dsty, width, height, srcx, srcy);
	//	return;
	//}

	//int inva = a;


	// get dest bitmap information
	DIBSECTION dib;
	if (GetObject(GetCurrentObject(mDC, OBJ_BITMAP), sizeof(DIBSECTION), &dib) < sizeof(DIBSECTION))
		return;

	// ensure object data is compatible
	if (dib.dsBm.bmHeight < 0)		return;
	if (dib.dsBm.bmBitsPixel != 32)	return;
	if (!dib.dsBm.bmBits)			return;

	// ensure desired draw rect is within src and dst boundaries
	if ((srcx + width) > nWidth)				width = nWidth - srcx;
	if ((srcy + height) > nHeight)			height = nHeight - srcy;
	if ((dstx + width) > dib.dsBm.bmWidth)	width = dib.dsBm.bmWidth - dstx;
	if ((dsty + height) > dib.dsBm.bmHeight)	height = dib.dsBm.bmHeight - dsty;

	// ensure rect is legit
	if (width <= 0)		return;
	if (height <= 0)		return;


	// get source palette
	RGBQUAD pal[256];
	GetDIBColorTable(dc, 0, 256, pal);

	// begin blending!
	BYTE* src = pixels;
	BYTE* dst = (BYTE*)dib.dsBm.bmBits;

	src += (srcy * nPitch) + srcx;
	dst += (dib.dsBm.bmHeight * dib.dsBm.bmWidthBytes) - ((dsty + 1) * dib.dsBm.bmWidthBytes) + (dstx * 4);

	int src_pitchadd = nPitch - width;
	int dst_pitchadd = -dib.dsBm.bmWidthBytes - (width * 4);

	int x, y;
	for (y = 0; y < height; ++y)
	{
		for (x = 0; x < width; ++x)
		{

			if (src[0])
			{


				
				unsigned long val;
				memcpy(&val, src, 4);
				unsigned long val2;
				memcpy(&val2, dst, 4);

				unsigned short srcPal = (unsigned short)(((val >> 3) & 0x1F) | ((val >> 3) & 0x1F00) >> 3 | ((val >> 3) & 0x1F0000) >> 6);

				unsigned short dstPal = (unsigned short)(((val2 >> 3) & 0x1F) | ((val2 >> 3) & 0x1F00) >> 3 | ((val2 >> 3) & 0x1F0000) >> 6);

				int num9 = min(31744, ((srcPal & 31744) * (unsigned short)alphaLow >> 4 & 31744) + ((dstPal & 31744) * (unsigned short)alphaHigh >> 4 & 31744));
				int num10 = min(992, ((srcPal & 992) * (unsigned short)alphaLow >> 4 & 992) + ((dstPal & 992) * (unsigned short)alphaHigh >> 4 & 992));
				int num11 = min(31, ((srcPal & 31) * (unsigned short)alphaLow >> 4) + ((dstPal & 31) * (unsigned short)alphaHigh >> 4));
				unsigned short m = (unsigned short)(num9 | num10 | num11);


				int R = ((m & 0x1F) << 3);
				int g = ((m >> 5) & 0x1F) << 3;
				int b = (((m >> 10) & 0x1f) << 3);
				unsigned long dst2 = RGB(R, g, b);

				memcpy(dst, &dst2, 4);
				src += 4;
				dst += 4;}

		}
		src += src_pitchadd;
		dst += dst_pitchadd;
	}
}

