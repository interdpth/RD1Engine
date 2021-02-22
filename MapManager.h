#pragma once

#include "nMapBuffer.h"
#include "GBAMethods.h"
#include "MapEditingState.h"
#include "..\Win32GBAImageClasses\backbuffer.h"
#include "..\MemFile\MemFile.h"
#include "structures.h"
class MapManager
{

private:
	nMapBuffer* TheLevelData;
	nMapBuffer* TheBackLayer;
	nMapBuffer* TheForeGround;
	nMapBuffer* TheClipData;
	nMapBuffer* TheBackground;
	nMapBuffer* ThisRoomBuff;
	MapEditingState* ThisMapState;
	
public:
	enum {
		Backlayer = 0,
		LevelData = 1,
		ForeGround = 2,
		Clipdata = 3,
		BackgroundLayer = 4
	}MAPLAYERS;
	bool created;
	
	MapEditingState* GetState() {
		return ThisMapState;
	}
	void MoveObject(LPARAM lParam, int index=-1);
	void Destroy();
	void Blank();
	static int SaveLayer(GBAMethods* GBA, unsigned char layerbyte, unsigned long* LayerPointer, nMapBuffer * layer);
	nMapBuffer* GetLayer(int thelayer)
	{

		if (thelayer == Backlayer) {
			return TheBackLayer;
		}


		if (thelayer == LevelData) {
			return TheLevelData;
		}


		if (thelayer == ForeGround) {
			return TheForeGround;
		}


		if (thelayer == Clipdata) {
			return TheClipData;
		}

		if (thelayer == BackgroundLayer) {
			return TheBackground;
		}

		return nullptr;

	}
	void Clear();
	
	void Resize(editingStates thisState, editingActions thisAction, WPARAM wParam, LPARAM lParam, MousePointer* tmp);
	MapManager();

	~MapManager();
};

