#pragma once

#include "Structures.h"

using namespace std;
class AnimatedTiles
{
public:
	AnimatedTiles(int crt);
	~AnimatedTiles();
	SE* specialEffects;
	void ReadTable(int effectNum);
	bool Animate();
	void GetGFX();
	int currentRomType;
	int LoadSpecialEffects(long EffectNum);
};

