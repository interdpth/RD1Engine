#pragma once
#include <vector>
using namespace std;
struct sUndo {
	unsigned short* copyAddress;
	std::vector< unsigned short> buffer;


};

class cUndoBuffer {//Resizing rooms will invalidate undobuffer
public:

	~cUndoBuffer();
	cUndoBuffer();
	int Set(unsigned long size, unsigned short* Layer);
	int Undo();
	sUndo UndoBuffer[16];
	int UndoNum;
	int MoveUp();

};
