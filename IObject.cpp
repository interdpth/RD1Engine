#include "IObject.h"

RD1Object::RD1Object()
{
	memset(&_rect, 0, sizeof(_rect));
}

RD1Object::~RD1Object()
{
}

void RD1Object::SetXPosition(int X)
{
	_rect.left = X;
}

void RD1Object::Move(int x, int y)
{
	_rect.right -= XPosition();
	_rect.bottom -= YPosition();
	
	SetXPosition(x);//;
	SetXPosition(y);//;
	_rect.right += XPosition();
	_rect.bottom += YPosition();
}
void RD1Object::SetYPosition(int Y )
{
	_rect.top = Y;
}

void RD1Object::SetWidth(int Y)
{
	_rect.bottom = Y;
}

void RD1Object::SetHeight(int Y)
{
	_rect.right = Y;
}

int RD1Object::XPosition()
{
	return _rect.left;
}
int RD1Object::YPosition()
{
	return _rect.top;
}

int RD1Object::Width()
{
	return _rect.right;
}

int RD1Object::Height()
{
	return _rect.bottom;
}

RECT* RD1Object::GetRect()
{
	return &_rect;
}