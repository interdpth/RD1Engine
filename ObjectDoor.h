#pragma once
#include "IObject.h"
#include "structures.h"
class ObjectDoor :
	public RD1Object
{
public:
	ObjectDoor(sDoor* d);
	~ObjectDoor();
	void SetXPosition(int X) override;

	void SetYPosition(int Y) override;

	void SetWidth(int Y) override;

	void SetHeight(int Y) override;
	int Width() override;

	int Height() override;

	int XPosition() override;

	int YPosition() override;
	sDoor* GetDoor();

private:
	sDoor _door;
};

