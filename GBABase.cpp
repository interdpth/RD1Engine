#include "GBABase.h"



GBABase::GBABase()
{
	Pal = new unsigned short[1024];
	vram = new unsigned char[0x8000];

}


GBABase::~GBABase()
{
	delete[] Pal;
	delete[] vram;

}
