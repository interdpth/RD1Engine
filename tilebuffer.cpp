
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "tilebuffer.h"
long bytesused = 0;

TileBuffer::TileBuffer()
{
	pBuf = NULL;
	Zero();
}

TileBuffer::~TileBuffer()
{
	Destroy();
}

void TileBuffer::Destroy()
{
	if(pBuf)
	{
		bytesused -= nBufSize * 8 * 8;
		delete[] pBuf;
		pBuf = 0;
	}

	Zero();
}

void TileBuffer::Zero()
{
	nBufSize = 0;
	nFullSize = 0;
	pBuf = 0;
}

void TileBuffer::Load(BYTE* dat, int tiles)
{


	// see if buffer is already big enough
	if (nFullSize * 8 * 8 < tiles * 8 * 8)
	{
	
	Destroy();	// kill existing buffer
	pBuf = new BYTE[tiles * 8 * 8];
	bytesused += tiles * 8 * 8;
	nFullSize = tiles;

	nBufSize = tiles;
	}
	// actually load the tiles
	BYTE* ptr = pBuf;
	int i, j;
	for(i = 0; i < tiles; ++i)
	{
		for(j = 0; j < 8*8; j += 2)
		{
			ptr[0] = (dat[0] & 0x0F);
			ptr[1] = (dat[0] >>   4);
			++dat;
			ptr += 2;
		}
	}
}


tile_t TileBuffer::operator [] (int index)
{
	if(index < 0)			return 0;
	if(index >= nBufSize)	return 0;

	return &pBuf[index * 8*8];
}

void TileBuffer::Save(BYTE* dest,int first,int count)
{
	if(first < 0)						return;
	if(count <= 0)						return;
	if((first+count) >= nFullSize)		return;

	BYTE* ptr = &pBuf[first * 8 * 8];
	int i, j;
	for(i = 0; i < count; ++i)
	{
		for(j = 0; j < 8*8; j += 2)
		{
			dest[0] = (ptr[0] & 0x0F) | ((ptr[1] & 0x0F) << 4);
			++dest;
			ptr += 2;
		}
	}
}
