#include "MainHeader.h"


BOOL CALLBACK ConnProc (HWND hwnd,unsigned int message,WPARAM wParam,LPARAM lParam)
{

	
	switch (message)
	{
		
	case WM_INITDIALOG:	// when dialog is first created
	//	DoorWin = hwnd;
		
		nMaxVScroll[sWidth] = 0xFF;
	
	case WM_COMMAND:
		if(LOWORD(wParam) == IDCANCEL) {
			ShowWindow(DoorWin,SW_HIDE);
			//EndDialog(DoorWin,0);
		}
    break;
		
	
		
	case WM_DESTROY:
		
		/// myBackBuffer.Destroy();
		break;
	}
	
	return 0;
}
