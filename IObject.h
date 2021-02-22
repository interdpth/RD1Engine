#pragma once
#include <Windows.h>
class RD1Object
{
public:
	virtual void SetXPosition(int X);
	virtual void SetYPosition(int Y);
	virtual void SetWidth(int X);
	virtual void SetHeight(int Y);
	//Returns left from Rect;
	virtual int XPosition();
	//Returns top from Rect;
	virtual int YPosition();
	//Returns right from Rect;
	virtual int Width();
	//Returns bottom from Rect;
	virtual int Height();
	//Returns reference to  _rect;
	RECT* GetRect();

	void Move(int x, int y);

	//Constructor
	RD1Object();

	//Deconstructor
	~RD1Object();
private:
	RECT _rect;
};

