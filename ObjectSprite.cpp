#include "ObjectSprite.h"

MapObjectSprite::MapObjectSprite(int creature, int X, int Y)
{
	_enemy.Creature = creature;
	_enemy.X = X;
	_enemy.Y = Y;
}

MapObjectSprite::MapObjectSprite(nEnemies* enemy)
{
	memcpy(&_enemy, enemy, sizeof(nEnemies)); 
	Apply();

}

MapObjectSprite::~MapObjectSprite()
{
}

void MapObjectSprite::Apply()
{
	Creature = _enemy.Creature;
	SetXPosition(_enemy.X);

	SetYPosition(_enemy.Y);
}
