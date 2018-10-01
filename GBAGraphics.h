#pragma once
class GBAGraphics
{
public:
	GBAGraphics();
	~GBAGraphics();
	unsigned char          fGbuf[0x8000];
	long        PcPalMem[0x200];
	unsigned char     BGBuf[0x8000];
    static GBAGraphics* VRAM;
	long SprPal[0x1000];
	short GBASprPal[0x500];

	
};

