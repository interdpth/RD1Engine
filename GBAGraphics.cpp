#include "GBAGraphics.h"
#include <Windows.h>
GBAGraphics* GBAGraphics::VRAM;

GBAGraphics::GBAGraphics()
{
	memset(&fGbuf, 0, 0x8000);
	memset(&PcPalMem, 0, 0x200 * sizeof(long));
	memset(&BGBuf, 0, 0x8000);
	memset(&SprPal, 0, 0x1000 * sizeof(long));
	memset(&GBASprPal, 0, 0x500 * sizeof(short));
}


GBAGraphics::~GBAGraphics()
{
}
