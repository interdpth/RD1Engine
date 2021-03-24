#include "SamusPiece.h"


SamusPiece::SamusPiece(unsigned long offset, unsigned char* data)
{


}

SamusPiece::SamusPiece(unsigned char top, unsigned char bottom, unsigned char* topGFX, unsigned char* bottomGFX)
{
	Setup(top, bottom, topGFX, bottomGFX);
}
SamusPiece::~SamusPiece()
{
	delete[] bottomGFX;
	delete[] topGFX;
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
	memcpy(dat, topGFX, topLen * 32);
	dat += topLen * 32;
	memcpy(dat, bottomGFX, bottomLen * 32);
}


void SamusPiece::Read(SamusAnim* theCurrentAnim, unsigned char* data)
{
	unsigned char* half = (unsigned char*)&data[(unsigned long)theCurrentAnim->Tophalf - 0x8000000];
	topLen = *half++;
	bottomLen = *half++;
	topGFX = new unsigned char[topLen * 32];
	bottomGFX = new unsigned char[bottomLen * 32];
	memcpy(topGFX, half, topLen * 32);
	half += topLen * 32;
	memcpy(bottomGFX, half, bottomLen * 32);
}

void SamusPiece::Setup(unsigned char top, unsigned char bottom, unsigned char* topGFX, unsigned char* bottomGFX)
{

}