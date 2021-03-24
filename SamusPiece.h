#pragma once
#include "SamusBase.h"
class SamusPiece
{
public:
	SamusPiece(unsigned long offset, unsigned char* data);
	SamusPiece(unsigned char top, unsigned char bottom, unsigned char* topGFX, unsigned char* bottomGFX);
	void Write(unsigned long offset, unsigned char* data);
	void Read(SamusAnim* theCurrentAnim, unsigned char* data);
	~SamusPiece();
	int GetBufferSize();
private:
	void Setup(unsigned char top, unsigned char bottom, unsigned char* topGFX, unsigned char* bottomGFX);
	unsigned char topLen;//*32;
	unsigned char bottomLen;//*32;
	unsigned char* topGFX;
	unsigned char* bottomGFX;
};