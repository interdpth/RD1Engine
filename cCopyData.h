#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

class cCopyData {
	//Currenly only support string to int
public:

	cCopyData();
	~cCopyData();
	int Copy(int Width, RECT data, unsigned short* pnt);
	int Paste(unsigned short *pnt);
	void Dump();
private:
	RECT  DataHold;//Configuration of data
	std::vector< unsigned short> buffer;
	int CopyWidth;
	int size;
};
