#include "SamusPiece.h"
#include <memory>

SamusPiece::SamusPiece(unsigned long offset, unsigned char* data)
{
	Init();
	Read(offset, data);
}
void SamusPiece::Init()
{
	topLen = 0;
	bottomLen = 0;
	bottomGFX = nullptr;
	topGFX = nullptr;
}
SamusPiece::SamusPiece(unsigned char top, unsigned char bottom, unsigned char* topGFX, unsigned char* bottomGFX)
{
	Setup(top, bottom, topGFX, bottomGFX);
}
SamusPiece::~SamusPiece()
{
	if(bottomGFX!=nullptr) delete[] bottomGFX;
	if (topGFX != nullptr)delete[] topGFX;
}
int SamusPiece::GetBufferSize()
{
	return 2 + topLen * 32 + bottomLen * 32;
}
void SamusPiece::Write(unsigned long offset, unsigned char* data)
{
	unsigned char* dat = data;
	*dat++ = topLen;
	*dat++ = bottomLen;
	std::memcpy(dat, topGFX, topLen * 32);
	dat += topLen * 32;
	std::memcpy(dat, bottomGFX, bottomLen * 32);
}


void SamusPiece::Read(unsigned long theCurrentAnim, unsigned char* data)
{
	unsigned char* half = (unsigned char*)&data[(unsigned long)theCurrentAnim];
	topLen = *half++;
	bottomLen = *half++;
	topGFX = new unsigned char[topLen * 32];
	bottomGFX = new unsigned char[bottomLen * 32];
	std::memcpy(topGFX, half, topLen * 32);
	half += topLen * 32;
	std::memcpy(bottomGFX, half, bottomLen * 32);
}

void SamusPiece::Setup(unsigned char top, unsigned char bottom, unsigned char* topGFX, unsigned char* bottomGFX)
{

}