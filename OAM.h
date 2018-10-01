#pragma once
struct  EncodedOAM {
	//OAM0 Props
	// 0-7   Y-Coordinate
	// 8     Rotation/Scaling Flag
	//When Rotation/Scaling used (Attribute 0, bit 8 set):
	//    9     Double-Size Flag     (0=Normal, 1=Double)
	//  When Rotation/Scaling not used (Attribute 0, bit 8 cleared):
	// 9         OBJ Disable(0 = Normal, 1 = Not displayed)
	//10-11 OBJ Mode  (0=Normal, 1=Semi-Transparent, 2=OBJ Window, 3=Prohibited)
	//12    OBJ Mosaic
	//13    Colors/Palettes
	//14-15 OBJ Shape

	//OAM1 Props
	//0-8   X-Coordinate           (0-511)
	//When Rotation/Scaling used (Attribute 0, bit 8 set): 9-13  Rotation/Scaling Parameter Selection (0-31)
	//When Rotation/Scaling not used (Attribute 0, bit 8 cleared):
	//9-11  Not used
	//12        Horizontal Flip(0 = Normal, 1 = Mirrored)
	//13        Vertical Flip(0 = Normal, 1 = Mirrored)
	//14-15 OBJ Size               (0..3, depends on OBJ Shape, see Attr 0)
	//          Size  Square   Horizontal  Vertical
	//          0     8x8      16x8        8x16
	//          1     16x16    32x8        8x32
	//          2     32x32    32x16       16x32
	//          3     64x64    64x32       32x64

	//OAM2 Props
	//0-9   Character Name          (0-1023=Tile Number)
	//10-11 Priority relative to BG (0-3; 0=Highest)
	//12-15 Palette Number   (0-15) (Not used in 256 color/1 palette mode)
	unsigned short OAM0;
	unsigned short OAM1;
	unsigned short OAM2;


};
#define oWidth 0
#define oHeight  1
struct DecodedOAM {
	signed char yCoord;
	unsigned char objRot;
	unsigned char DSaOD;
	unsigned char ObjMode;
	unsigned char ObjMosiac;
	unsigned char Paltype;
	unsigned short ObjShape;
	unsigned short xCoord;
	unsigned char rotation;
	unsigned char HorizontalFlip;
	unsigned char VerticalFlip;
	unsigned char ObjSize;
	unsigned short TileNumber;
	unsigned long priority;
	unsigned char Palette;
};

struct OverAllOAM {

	EncodedOAM enOAM;
	DecodedOAM deOAM;
	int resolvedX;
	int resolvedY;
};
