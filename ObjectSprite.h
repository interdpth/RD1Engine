#pragma once
#include "IObject.h"
#include "structures.h"
class MapObjectSprite :
	public RD1Object
{
public:

	int Creature;
	void Apply();
	MapObjectSprite(int Creature, int X, int Y);
	MapObjectSprite(nEnemies* enemy);
	~MapObjectSprite();
private:
	nEnemies _enemy;
};

