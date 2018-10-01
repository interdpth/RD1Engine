#pragma once
//Contains RAM components, or others
class GBABase
{
	unsigned short * Pal;
	unsigned char* vram;
public:
	GBABase();
	~GBABase();
};

