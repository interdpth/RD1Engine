#pragma once
#include <stdio.h>
#include "SamusAnim.h"
class SamusPiece
{
public:
	SamusPiece(unsigned long offset, unsigned char* data);
	SamusPiece(unsigned char top, unsigned char bottom, unsigned char* topGFX, unsigned char* bottomGFX);
	void Write(unsigned long offset, unsigned char* data);
	void Read(unsigned long theCurrentAnim,  unsigned char* data);
	~SamusPiece();
	int GetBufferSize();
	void Init();
	unsigned char topLen;//*32;
	unsigned char bottomLen;//*32;
	unsigned char* topGFX;
	unsigned char* bottomGFX;
private:
	void Setup(unsigned char top, unsigned char bottom, unsigned char* topGFX, unsigned char* bottomGFX);
	
};