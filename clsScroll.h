#pragma once
#include <Windows.h>
class clsUIScroll {
public:
	long curpos;
	int max;
	clsUIScroll();
	~clsUIScroll();
	HWND me;
	int vert;
	int UpdateScroll(WPARAM how);
	int create(HWND hwnd, int isvert, int max);
	int ChangeScrollbars();
};