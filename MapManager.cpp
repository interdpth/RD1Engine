#include "MapManager.h"
#include "GBAMethods.h"
#include <Windows.h>
#include "DoorManager.h"
#include "SpriteObjectManager.h"
#include "clsRoomScrolls.h"
#include "BaseGame.h"

#define GetX(lParam) LOWORD(lParam)
#define GetY(lParam) HIWORD(lParam)
void MapManager::MoveObject(LPARAM lParam, int index) {
	editingStates thisState = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();//Wait what
	int objID = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetObjId();
	if (thisState == editingStates::MAP) {
//		mpMap.sY = GetY(lParam) / 16;
	//	mpMap.sX = GetX(lParam) / 16;
	}
	else if (thisState == editingStates::DOOR) {

		//Get shit to our door
		
		if (objID != -1) 
		{			
			RD1Engine::theGame->mgrDoors->Doors[objID]->Move(GetX(lParam) / 16, GetY(lParam) / 16);
		}
	}
	else if (thisState == editingStates::SPRITE) {

		//Get shit to our door
	
		if (objID != -1) {
			switch (index) {
			case 0:				
			case 1:
			case 2:
				RD1Engine::theGame->mainRoom->mgrSpriteObjects->SpriteObjects[index][objID]->Move( GetX(lParam) / 16, ( GetY(lParam) / 16)) ;//;
				break;
			}		
		}
	}
	else if (thisState == editingStates::SCROLL) {

		//Get shit to our door
		
		RECT* thisScroll = RD1Engine::theGame->mgrScrolls->Scrolls[objID]->GetRect();


		thisScroll->right -= thisScroll->left;
		thisScroll->bottom -= thisScroll->top;


		thisScroll->left = GetX(lParam) / 16;//;
		thisScroll->top = GetY(lParam) / 16;//;
		thisScroll->right += thisScroll->left;
		thisScroll->bottom += thisScroll->top;

		
	}

}

void MapManager::Destroy()
{
	if (created)
	{
		delete TheLevelData;
		
		delete TheBackLayer;
		delete TheForeGround;
		delete TheClipData;
		delete TheBackground;
		delete ThisMapState;
		TheBackLayer = 0;
		TheLevelData = 0;
		TheForeGround = 0;
		TheBackground = 0;
		ThisMapState = 0;
		
		created = false;
	}
}
MapManager::MapManager()
{
	TheBackLayer = 0;
	TheLevelData = 0;
	TheForeGround = 0;
	TheBackground = 0;
	ThisMapState = 0;
	TheClipData = NULL;
	created = false;
}

MapManager::~MapManager()
{
	Destroy();
}
void MapManager::Blank()
{
	
	Destroy();
	//GlobalVars::gblVars->ThisBackBuffer.Clear();
	TheLevelData = new nMapBuffer();
	TheBackLayer = new nMapBuffer();
	TheForeGround = new nMapBuffer();
	TheClipData = new nMapBuffer();
	TheBackground = new nMapBuffer();
	ThisMapState = new MapEditingState();
	created = true;
}

void MapManager::Clear()
{


}
int MapManager::SaveLayer(GBAMethods* GBA, unsigned char layerbyte, unsigned long* LayerPointer, nMapBuffer * layer) {
	long tlength = 0;
	unsigned long offset = 0;

	int uncompsize = (layer->X *layer->Y) * 2;
	unsigned char*  compBuffer = new unsigned char[uncompsize];
	memset(compBuffer, 0, uncompsize);
	if (layerbyte == 16 && layer->SDirty == 1)
	{		
		tlength = GBA->compress(compBuffer, (unsigned char *)layer->TileBuf2D, uncompsize);

		if (tlength + 4 > layer->ThisCompsize + 4) {
			offset = GBA->FindFreeSpace(tlength*2 + 4, 0xFF);
		}
		else {
			offset = *LayerPointer - 0x8000000;
		}


		if (offset != 0)
		{

			*LayerPointer = offset + 0x8000000;

			MemFile::currentFile->seek(offset);
			MemFile::currentFile->fputc(layer->X);
			MemFile::currentFile->fputc(layer->Y);
			MemFile::currentFile->fwrite(compBuffer, sizeof(char), tlength);
		//	GBA->Reopen();
		}
		else
		{
			//sMessage("Could not save the layer.");
			return -1;
		}
	}
	delete[] compBuffer;
	return 0;

}
// tmp = &curGame->mgrScrolls->Scrolls[cboScroll.GetListIndex()].rect
/*int objID = curGame->mainRoom->mapMgr->GetState()->GetObjId();
		if (objID != -1) {
			tmp = &curGame->mgrDoors->Doors[objID].virtualDoor;
		}
		
		mpMap;
		*/
void MapManager::Resize(editingStates thisState, editingActions thisAction, WPARAM wParam, LPARAM lParam, MousePointer* tmp) {
	
	RD1Engine* curGame = RD1Engine::theGame;
	if (thisState == editingStates::SCROLL) {

	}
	else if (thisState == editingStates::DOOR) {
		//Not supported 
		int objID = curGame->mainRoom->mapMgr->GetState()->GetObjId();
		if (objID != -1) {
			//tmp = &curGame->mgrDoors->Doors[objID].virtualDoor;
		}
	}
	else if (thisState == editingStates::MAP) {
	
			//tmp = &mpMap;
	}



	if (wParam & (MK_RBUTTON) && tmp!=NULL) {
		tmp->nX = GetX(lParam) / 16;
		tmp->nY = GetY(lParam) / 16;
		//Determine direction for length and width;
		if (tmp->nX > tmp->cX) {//increase width;
			tmp->Width += 1;
		}
		else if (tmp->nX < tmp->cX) {
			tmp->Width -= 1;
		}
		if (tmp->nY > tmp->cY) {//increase width;
			tmp->Height += 1;
		}
		else if (tmp->nY < tmp->cY) {
			tmp->Height -= 1;
		}
		tmp->cX = GetX(lParam) / 16;
		tmp->cY = GetY(lParam) / 16;
		if (tmp->Height < 1) tmp->Height = 1;
		if (tmp->Width < 1)tmp->Width = 1;

		//	if(tmp->Width>16)tmp->Width = 16;
		//	if(tmp->Height>16)tmp->Height = 16;
		//check for out of bounds 
	/*	if (tmp->nX < 0) tmp->nX = 0;
		if (tmp->nX > GetLayer(MapManager::LevelData)->X)
			tmp->nX = GetLayer(MapManager::LevelData)->X;
		if (tmp->nY < 0) tmp->nY = 0;
		if (tmp->nY > GetLayer(MapManager::LevelData)->Y) tmp->nY = GetLayer(MapManager::LevelData)->Y;
*/
	}

}