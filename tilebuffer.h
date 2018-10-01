#pragma once
#ifndef __TILEBUFFER_H__
#define __TILEBUFFER_H__

typedef const unsigned char*		tile_t;


class TileBuffer
{
public:
					TileBuffer();
					~TileBuffer();

	void			Destroy();
	void			Load(unsigned char* dat,int tiles);
    void			Save(unsigned char* dest,int first,int count);
	tile_t			operator [] (int index);

protected:
	void			Zero();
	unsigned char*			pBuf;
	int				nBufSize;		// size in tiles
	int				nFullSize;		// in tiles (size of allocated buf)
};

#endif