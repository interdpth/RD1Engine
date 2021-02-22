#include "ObjectDoor.h"



ObjectDoor::ObjectDoor(sDoor* d)
{
	memcpy(&_door, d, sizeof(sDoor));
	SetYPosition(d->YEntrance);
	SetXPosition(d->XEntrance);	
}


ObjectDoor::~ObjectDoor()
{
}

int ObjectDoor::XPosition()
{
	return _door.XEntrance;
}

int ObjectDoor::YPosition()
{
	return _door.YEntrance;
}

void ObjectDoor::SetXPosition(int X)
{
	_door.XEntrance = X;
}

void ObjectDoor::SetYPosition(int Y)
{
	_door.YEntrance = Y;
}

int ObjectDoor::Width()
{
	return _door.DWidth;
}

int ObjectDoor::Height()
{
	return _door.DHeight;
}



void ObjectDoor::SetWidth(int Y)
{
	_door.DWidth = Y;
}

void ObjectDoor::SetHeight(int Y)
{
	_door.DHeight = Y;
}

sDoor* ObjectDoor::GetDoor()
{
	return &_door;
}
