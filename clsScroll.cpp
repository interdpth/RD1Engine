#include "clsScroll.h"

clsUIScroll::clsUIScroll() {


}
clsUIScroll::~clsUIScroll() {

}
int clsUIScroll::create(HWND hwnd, int isvert, int max) {
	this->me = hwnd;
	this->vert = isvert;
	this->max = max;
	this->curpos = 0;
	ChangeScrollbars();
	return 0;
}
int clsUIScroll::UpdateScroll(WPARAM how) {

	/*
	*	Called whenever the user scrolls (called by WM_HSCROLL and WM_VSCROLL)
	*
	*    dialogwnd is the HWND to the dialog
	*    how is the WPARAM paGlobalVars::gblVars->SSEd to WM_HSCROLL or WM_VSCROLL (it contains info about the scroll)
	*    isvertical is 0 for H scrolling, 1 for V scrolling
	*/

	// I usually do tricks with pointers and LUTs and stuff here to recycle a lot of code
	//   so I don't have to have if/else chains everywhere, but that might make this confusing
	//   so I'll stick to more basic practices:


	// get cur/max scroll


	// find out how the user is scrolling
	//  admittedly, this is a royal pain in the arse
	switch (LOWORD(how))
	{
	case SB_LEFT:	this->curpos = 0;			break;	// scroll all the way to the left
	case SB_RIGHT:	this->curpos = max;	break;	// all the way to the right
	case SB_LINELEFT: this->curpos -= 1;		break;	// one click to the left
	case SB_LINERIGHT: this->curpos += 1;		break;	// one click to the right
	case SB_PAGELEFT: this->curpos -= 10;		break;	// one page to the left
	case SB_PAGERIGHT: this->curpos += 10;		break;	// one page to the right
	case SB_THUMBTRACK:this->curpos = HIWORD(how); break;	// user is dragging bar
	}

	// keep scroll within bounds
	if (this->curpos > max)	curpos = max;
	if (curpos < 0)			curpos = 0;

	// update our scroll vars, as well as the scrollbar itself
	if (vert) {
		SetScrollPos(me, SB_VERT, curpos, 1);
	}
	else {
		SetScrollPos(me, SB_CTL, curpos, 1);
	}
	return 0;

}

int clsUIScroll::ChangeScrollbars()
{

	/*
	*	This function will prep our scrollbars so that they have the desired min/max
	*    values and other crap
	*/
	//If Res == 0 horizontal if Res == 1 vertical
	/*
	*	Before we worry about the scrollbars, let's prep our own variables
	*/
	curpos = 0;
	// maximum V scroll


	/*
	*  Calculating max scroll:
	*
	*	our full image is 256 pixels wide (the width of the Image object)
	*    only nDisplayWidth pixels are displayed
	*
	*  256 - nDisplayWidth is the number of pixels that won't be displayed
	*  doing a +7 / 8 with that value rounds up to the number of *tiles* that won't be
	*  displayed.  This is the maximum scroll value.
	*
	*  If all but 1 tile is visible, then valid scroll values would be 0,1
	*  If all but 2 tiles are visible, the scroll between 0-2, etc
	*/



	SCROLLINFO		inf;
	memset(&inf, 0, sizeof(SCROLLINFO));		// zero out our SCROLLINFO struct
	inf.cbSize = sizeof(SCROLLINFO);		// cbSize must be the size of the struct (weird WinAPI nonsense)

											// now we start filling our scroll struct with relevent info.
											//   start with H Scroll whynot

											// fMask determines the properties of this scrollbar we'll be changing.
											// Combine values with OR
	inf.fMask = SIF_DISABLENOSCROLL |	// This makes it so the scrollbar is disabled when unnecesary
										// otherwise it's hidden (this is usually more applicable to scrollbars
										// that are attached to normal windows -- not so much for scrollbar
										// controls on dialogs -- but whatever)
		SIF_PAGE |				// we will be updating the 'nPage' value
		SIF_POS |				// and the 'nPos' value
		SIF_RANGE;				// and min/max values

								// I'm unsure how this works exactly.  It affects the size
								// of the scrollable part of the bar, but I'm unclear on details.
								// higher values mean fatter bar.  Let's just use 1 because it's simple

	inf.nPos = 0;		// reset position to 0

	inf.nMin = 0;		// minimum H scroll is 0



						// now that the SCROLLINFO struct is filled, set the properties of the scrollbar
	if ((!vert)) {
		// maximum H scroll

		inf.nMax = max;

		inf.nPage = 0;
		SetScrollInfo(me,		// HWND to scrollbar
			SB_CTL,					// this scrollbar is a dialog control
			&inf,					// pointer to our info struct
			1);
		// redraw flag (redraw the scrollbar after this change)



	}
	else {

		// V Scroll is the same thing.  All values in the struct remain the same except for the max

		inf.nMax = max;
		inf.nPage = 0;
		SetScrollInfo(me,
			SB_CTL,
			&inf,
			1);





		// SetScrollPos( me, SB_HORZ, 0, 1 );
	}
	return 0;
}

