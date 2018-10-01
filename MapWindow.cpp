#include "MainHeader.h"
#include "MapManager.h"
#include "GlobalVars.h"
#include "DoorManager.h"
#include "MapEditingState.h"
#include "clsRoomScrolls.h"
#include "TilesetManager.h"
#include "UiState.h"
RECT toolsRect;
extern HWND hTabControl;
extern HANDLE handle_out;
void LoadScrollInfo(int s, Scroller *scroll);
nMapBuffer* GetActiveBuffer() {
	nMapBuffer* tmpBuf = NULL;
	if (GlobalVars::gblVars->checkBoxForeground.value() == 1 || GlobalVars::gblVars->chkMC[0].value() == 1)
	{
		tmpBuf = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::ForeGround);
	}
	else if (GlobalVars::gblVars->checkBoxLevel.value() == 1 || GlobalVars::gblVars->chkMC[1].value() == 1)
	{
		tmpBuf = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData);
	}
	else if (GlobalVars::gblVars->checkBoxBackground.value() == 1 || GlobalVars::gblVars->chkMC[2].value() == 1)
	{
		tmpBuf = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Backlayer);
	}
	else if (GlobalVars::gblVars->checkBoxClip.value() == 1 || GlobalVars::gblVars->chkMC[3].value() == 1)
	{
		tmpBuf = RD1Engine::theGame->mainRoom->mapMgr->GetLayer(MapManager::Clipdata);
	}

	return tmpBuf;
}

void Draw(HDC hdc)
{
	RECT mainRect;
	RECT viewRect; 
	GetWindowRect(UiState::stateManager->GetMapWindow(), &viewRect);

	//viewRect.top = 0;
	//viewRect.left = toolsRect.right;
	//viewRect.right = mainRect.right - toolsRect.right;
	//viewRect.bottom = mainRect.bottom - 16;


	MapManager* mgrMap;
	if (RD1Engine::theGame != NULL&&RD1Engine::theGame->mainRoom != NULL)
	{
		mgrMap = RD1Engine::theGame->mainRoom->mapMgr;
		nMapBuffer* c = mgrMap->GetLayer(3);
		if (c) {


			if (viewRect.right > c->X*16)
			{
				viewRect.right = c->X * 16;
			}

			if (viewRect.bottom > c->Y * 16)
			{
				viewRect.bottom = c->Y * 16;
			}
		}
	}

	

	if (viewRect.right < 64)
	{
		viewRect.right = 64;
	}
	if (viewRect.bottom < 64)
	{
		viewRect.bottom = 64;
	}

	

	BitBlt(hdc, 0, 0, viewRect.right, viewRect.bottom, GlobalVars::gblVars->ThisBackBuffer.DC(), nHScroll[sHMap] * 16, nVScroll[sVMap] * 16, SRCCOPY);
	
	if (GlobalVars::gblVars->ViewClip.value() != 1) 
	{
		DrawTileRect(hdc, mpMap, 16);
	}
}

LRESULT CALLBACK MapProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{

	char *buffer = new char[256];
	memset(buffer, 0, 256);
	MapManager* mgrMap;
	if (RD1Engine::theGame!=NULL&&RD1Engine::theGame->mainRoom!=NULL)
	{
		mgrMap = RD1Engine::theGame->mainRoom->mapMgr;
	}
	nMapBuffer* Layer;
	PAINTSTRUCT ps;
	HDC hdc;
	int poo;
	int i = 0;
	int dw, dh, x, y, nx, ny;
	dw = 0;
	dh = 0;
	x = 0;
	y = 0;
	nx = 0;
	ny = 0;
	RECT viewRect;
	RECT Drawme;
	int blitareax = 0, blitareay = 0;
	char cBuf[200];
	int td = 0;
	switch (message)
	{
	case WM_CREATE:
		GetWindowRect(GetDlgItem(hWnd, grpMap), &toolsRect);
	

		break;
	case WM_COPYDATA:
	case WM_COPY:
		td = 1;
		break;
	case WM_PASTE:
		td = 1;
		break;
	
	case WM_PAINT:
		if (RD1Engine::theGame &&  RD1Engine::theGame->mainRoom &&  mgrMap)
		{
			
				hdc = BeginPaint(hWnd, &ps);

				Draw(hdc);

				EndPaint(hWnd, &ps);
				ReleaseDC(hWnd, hdc);
			
			//DeleteDC(BaseGame::theGame->mainRoom->mapMgr->ThisBackBuffer->DC());
		}
		break;
	case WM_VSCROLL:	// exact same idea, but V scroll instead of H scrollFindMeAScroll
		UpdateScroll(hWnd, wParam, 1, vsbMap, sVMap);
		InvalidateRect(hWnd, 0, 1);
		break;
	case WM_HSCROLL:
		UpdateScroll(hWnd, wParam, NULL, hsbMap, sHMap);
		InvalidateRect(hWnd, 0, 1);
		break;
	case WM_LBUTTONDOWN:

		if (wParam & MK_LBUTTON) {
			editingStates thisState = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();//Wait what
			if (thisState == editingStates::SPRITE) {
				int spriteno = Gimmeasprite(((GetX(lParam) / 16) + nHScroll[sHMap]), ((GetY(lParam) / 16) + nVScroll[sVMap]));
				RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(spriteno);
				RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetAction(editingActions::MOVE);
			}
			else if (thisState == editingStates::DOOR)
			{
				if (RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetAction() != editingActions::MOVE)
				{
					int doornum = RD1Engine::theGame->mgrDoors->GetDoor(((GetX(lParam) / 16) + nHScroll[sHMap]), ((GetY(lParam) / 16) + nVScroll[sVMap]));
					if (doornum != 0xffffffff)
					{
						RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(RD1Engine::theGame->mgrDoors->CurrentRoomDoorIndexes[doornum]);
						RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetAction(editingActions::MOVE);

					}
				}
			}
			else if (thisState == editingStates::SCROLL)
			{
				if (RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetAction() != editingActions::MOVE)
				{
					int scrollid = RD1Engine::theGame->mgrScrolls->Findmeascroll(((GetX(lParam) / 16) + nHScroll[sHMap]), ((GetY(lParam) / 16) + nVScroll[sVMap]), cboScroll.GetListIndex());
					if (scrollid != 0xffffffff)
					{
						RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(scrollid);
						RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetAction(editingActions::MOVE);
					}
				}
			}
			else if (thisState == editingStates::MAP)
			{
				EditLayers(wParam, lParam);
			}
			RD1Engine::theGame->DrawStatus.dirty = true;
		}
		return 0;

		break;
	case WM_RBUTTONDOWN:
		if (LOWORD(wParam) == MK_RBUTTON) {
			//HandleRightClick(hWnd, message, wParam, lParam, GlobalVars::gblVars->ScrollCheck.value() == 1);
			editingStates thisState = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();//Wait what
			if ((thisState != editingStates::SPRITE  && thisState != editingStates::DOOR) || (thisState == editingStates::DOOR &&  SendMessage(GetDlgItem(hwndMain(), chkResizeDoors), BM_GETCHECK, 0, 0) == 1))
			{
				RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetAction(editingActions::RESIZE);
			}
			else
			{
				if (thisState == editingStates::DOOR)
				{
					int doornum = RD1Engine::theGame->mgrDoors->GetDoor(((GetX(lParam) / 16) + nHScroll[sHMap]), ((GetY(lParam) / 16) + nVScroll[sVMap]));
					RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(doornum);
					if (td != -1) {
						UiState::stateManager->ShowObj();
						RD1Engine::theGame->mgrDoors->LoadThisDoor(doornum);
						UiState::stateManager->ShowObj();
						return 0;
					}
				}
				else if (thisState == editingStates::SPRITE) {
					int spriteno = Gimmeasprite(((GetX(lParam) / 16) + nHScroll[sHMap]), ((GetY(lParam) / 16) + nVScroll[sVMap]));
					RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(spriteno);
					SFSS = spriteno;
					UiState::stateManager->ShowObj();
					LoadCurSprite();
				}
				else if (thisState == editingStates::SCROLL) {
					int scrollid = RD1Engine::theGame->mgrScrolls->Findmeascroll(((GetX(lParam) / 16) + nHScroll[sHMap]), ((GetY(lParam) / 16) + nVScroll[sVMap]), cboScroll.GetListIndex());
					if (scrollid != 0xffffffff)
					{
						RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(scrollid);


						UiState::stateManager->ShowObj();
						LoadScrollInfo(scrollid, RD1Engine::theGame->mgrScrolls->GetScrollInfo());
					}
				}
				InvalidateRect(hWnd, 0, 1);
			}
		}
		break;
	case WM_RBUTTONUP:

		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetAction(editingActions::PLACETILE);//Return to default action
		break;
	case WM_LBUTTONUP:
		RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetAction(editingActions::PLACETILE);

		break;
	case WM_MOUSEMOVE:
	{
		if (!RD1Engine::theGame || !RD1Engine::theGame->mainRoom ||!RD1Engine::theGame->mainRoom->mapMgr ||  !RD1Engine::theGame->mainRoom->mapMgr||!RD1Engine::theGame->mainRoom->mapMgr->created)
		{
			return 0;
		}
		editingStates thisState = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetState();
		editingActions thisAction = RD1Engine::theGame->mainRoom->mapMgr->GetState()->GetAction();
		if (thisAction == editingActions::RESIZE && LOWORD(wParam) == MK_RBUTTON) {
			if (thisState == editingStates::SCROLL) {

				int scrollid = RD1Engine::theGame->mgrScrolls->Findmeascroll(((GetX(lParam) / 16) + nHScroll[sHMap]), ((GetY(lParam) / 16) + nVScroll[sVMap]), cboScroll.GetListIndex());
				if (scrollid != 0xffffffff)
				{
					RD1Engine::theGame->mainRoom->mapMgr->GetState()->SetObjId(scrollid);
				}
				UiState::stateManager->ShowObj();
			}
			RD1Engine::theGame->mainRoom->mapMgr->Resize(thisState, thisAction, wParam, lParam);
		}
		else if (LOWORD(wParam) == MK_LBUTTON) {
			RD1Engine::theGame->mainRoom->mapMgr->MoveObject(lParam);

		}
		else
		{
			mpMap.sY = GetY(lParam) / 16;
			mpMap.sX = GetX(lParam) / 16;

		}
		InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
		//	UiState::stateManager->UpdateMapObjectWindow(); 
		if (!mgrMap->GetLayer(4)->TileBuf2D == NULL)
		{
			poo = Combos[cClip].GetListIndex();

			//Combos[cClip].SetListIndex(mgrMap->GetLayer(MapManager::Clipdata)->TileBuf2D[((GetX(lParam) / 16) + nHScroll[sHMap]) + ((GetY(lParam) / 16) + nVScroll[sVMap])* (mgrMap->GetLayer(MapManager::LevelData)->X)]);

			GetWindowText(Combos[cClip].GetHwnd(), cBuf, 200);
			SetWindowText(GetDlgItem(GlobalVars::gblVars->frameControls, lblClip), cBuf);
			Combos[cClip].SetListIndex(poo);
		
		}
		sprintf(cBuf, "X: %X", ((GetX(lParam) / 16) + nHScroll[sHMap]));
		SetWindowText(GetDlgItem(GlobalVars::gblVars->frameControls, lblX), cBuf);
		sprintf(cBuf, "Y: %X", ((GetY(lParam) / 16) + nVScroll[sVMap]));
		SetWindowText(GetDlgItem(GlobalVars::gblVars->frameControls, lblY), cBuf);
		if (wParam == MK_LBUTTON)
		{
			SendMessage(hWnd, 0x201, wParam, lParam);
		}

	}



	break;
	case WM_RBUTTONDBLCLK:
		///Reset the rects 

		mpMap.Height = 1;
		mpMap.Width = 1;
		mpTileset.Width = mpMap.Width;
		mpTileset.Height = mpMap.Height;
		InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 1);
		InvalidateRect(UiState::stateManager->GetTilesetWindow(), 0, 1);
		break;

	case WM_KEYDOWN:							// Here's what you're interested in! -- check to see if someone preGlobalVars::gblVars->SSEd a key.
												//c=67
												//ctrl=17
												//p=80

		if (wParam&VK_CONTROL) {


			switch (wParam)
			{
				/*	case 'Z':
				sprintf(buffer, "Map-Undo's left: %d", Layer->UndoBuff->UndoNum);
				SetWindowText(UiState::stateManager->GetMapWindow(), buffer);


				Layer->UndoBuff->Undo();


				Drawme.top = 0;
				Drawme.left = 0;
				Drawme.bottom = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y;
				Drawme.right = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X;
				Layer->DrawRect.top = 0;
				Layer->DrawRect.left = 0;
				Layer->DrawRect.bottom = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->Y;
				Layer->DrawRect.right = BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->X;


				return TRUE;
				*/
			case 'X':


				return TRUE;

			case 'C':
				Layer = GetActiveBuffer();
				SetWindowText(UiState::stateManager->GetMapWindow(), "Map-Ctrl + C");
				Drawme.top = mpMap.sY;
				Drawme.left = mpMap.sX;
				Drawme.bottom = Drawme.top + mpMap.Height;
				Drawme.right = Drawme.left + mpMap.Width;
				Layer->CopyData->Copy(Layer->X, Drawme, Layer->TileBuf2D);


				return TRUE;

			case 'V':
				Layer = GetActiveBuffer();
				SetWindowText(UiState::stateManager->GetMapWindow(), "Map- Ctrl + v");
				Layer->UndoBuff->Set(Layer->X*Layer->Y * 2, Layer->TileBuf2D);
				Layer->CopyData->Paste(&Layer->TileBuf2D[mpMap.sX + (mpMap.sY*Layer->X)]); //mpMap.sX+( mpMap.sY*Layer->X)]);
				
				RD1Engine::theGame->DrawRoom();
			
				InvalidateRect(UiState::stateManager->GetMapWindow(), 0, 0);
				return TRUE;

			case 'A':


				return TRUE;
			}


		}

		// set the key that person preGlobalVars::gblVars->SSEd as true.
		return 0;


		break;

	case WM_SIZE:
		//Gotta Make sure the size isn't out of bounds.
		if (lParam != 0) {
			UiState::stateManager->ResizeMap(hTabControl);
		}
		break;
	case WM_DESTROY:

		break;

	}
	delete[] buffer;
	return DefWindowProc(hWnd, message, wParam, lParam);
}
