#pragma once
struct PosModify
{
	int x;
	int y;
};

enum SupportedTitles
{
	titleZM = 0,
	titleMF,
	titleWL,
	titleUnsupport
};

struct drawstate
{
	bool dirty;
	bool BG0;
	bool BG1;
	bool BG2;
	bool BG3;
	bool Doors;
	bool Sprites;
	bool Clipdata;
	bool Scrolls;
	bool SpriteRect;
};
