#include "Frames.h"
unsigned short SetBitValue(unsigned short val, int bitIndex)
{
	return (val << bitIndex);
}


unsigned short FrameManager::EncodeOAM0(
	unsigned char yCoord,
	unsigned char rot,
	unsigned char objFlag,
	unsigned char objMode,
	unsigned char objMosaic,
	unsigned char palType,
	unsigned char objShape)
{
	unsigned short newOam = 0;
	newOam = yCoord;
	newOam |= SetBitValue(rot & 1, 8);
	newOam |= SetBitValue(objFlag & 1, 9);
	newOam |= SetBitValue(objMode & 2, 10);
	newOam |= SetBitValue(objMosaic & 1, 12);
	newOam |= SetBitValue(palType & 1, 13);
	newOam |= SetBitValue(objShape & 3, 14);
	return newOam;
}

void FrameManager::UnpackOAM(EncodedOAM* src, DecodedOAM* decodedOam)
{
	unsigned short OAM0 = src->OAM0;
	unsigned short OAM1 = src->OAM1;
	unsigned short OAM2 = src->OAM2;
	decodedOam->yCoord = (OAM0 & 0xFF);

	decodedOam->objRot = cOAMManager::GetBitField(OAM0, 8, 1);
	decodedOam->DSaOD = cOAMManager::GetBitField(OAM0, 9, 1);
	decodedOam->ObjMode = cOAMManager::GetBitField(OAM0, 10, 2);
	decodedOam->ObjMosiac = cOAMManager::GetBitField(OAM0, 12, 1);
	decodedOam->Paltype = cOAMManager::GetBitField(OAM0, 13, 1);
	decodedOam->ObjShape = cOAMManager::GetBitField(OAM0, 14, 2);

	//OAM1
	decodedOam->xCoord = OAM1 & 511;

	if (cOAMManager::GetBitField(OAM0, 8, 1) == 1) {
		decodedOam->rotation = cOAMManager::GetBitField(OAM1, 9, 5);
	}
	else {
		decodedOam->HorizontalFlip = (OAM1 & 0x400 ? 1 : 0);
		decodedOam->VerticalFlip = (OAM1 & 0x800 ? 1 : 0);
	}

	decodedOam->ObjSize = OAM1 >> 14;
	decodedOam->priority = ((OAM2 & 0xC00) >> 0xA) & 3; // (oamPiece >> 10) & 3;//cOAMEdit::GetBitField(oamPiece, 10, 2);
	if (decodedOam->priority > 0)
	{
		decodedOam->priority = decodedOam->priority;
	}
	decodedOam->TileNumber = OAM2 & 0x3FF;
	decodedOam->Palette = (OAM2 & 0xF000) / 0x1000;
}

unsigned short FrameManager::EncodeOAM1(bool rotFlag,
	unsigned short xCoord, unsigned char rotBlob,
	bool hFlip, bool vFlip, unsigned char objSize)
{
	unsigned short newOAM = 0;
	newOAM = xCoord & 511;
	if (rotFlag)
	{
		SetBitValue(rotBlob & 31, 9);
	}
	else
	{
		newOAM |= SetBitValue(hFlip, 12);
		newOAM |= SetBitValue(vFlip, 13);
	}

	newOAM |= SetBitValue(objSize, 14);
	return newOAM;
}

unsigned short FrameManager::EncodeOAM2(unsigned short tileNumber, unsigned char bgPriority, unsigned char palette)
{
	unsigned short newOAM = 0;
	newOAM = tileNumber;
	newOAM |= SetBitValue(bgPriority & 3, 10);
	newOAM |= SetBitValue(palette & 0xF, 12);
	return newOAM;
}
