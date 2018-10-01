#include "cUndoBuffer.h"



cUndoBuffer::~cUndoBuffer() {

}
cUndoBuffer::cUndoBuffer() {
	UndoNum = 0;
}

int cUndoBuffer::Set(unsigned long size, unsigned short* Layer) {
	int i = 0;

	if (UndoNum>15) {//Moves all the buffers down
		for (i = 0; i<UndoNum - 1; i += 2) {

			UndoBuffer[i].copyAddress = UndoBuffer[i + 1].copyAddress;
			UndoBuffer[i].buffer = UndoBuffer[i + 1].buffer;


		}
		UndoNum = 15;
	}
	//sUndo[UndoNum]
	if (size != 0) {
		UndoBuffer[UndoNum].copyAddress = Layer;
		UndoBuffer[UndoNum].buffer.resize(size * 2);
		//GBA.LZ77Comp(size,(unsigned char*)&Layer,(unsigned char*)&UndoBuffer[UndoNum].buffer[0]);
		//UndoBuffer[UndoNum].buffer(Layer,Layer+size);
		//std::copy(Layer, Layer + size-1, UndoBuffer[UndoNum].buffer.begin());
		//UndoBuffer[UndoNum].buffer.(Layer,size);
		//UndoBuffer[UndoNum].buffer = &Layer;
		//std::copy(&Layer,&Layer+size,UndoBuffer[UndoNum].buffer);
		//memcpy(&UndoBuffer[UndoNum].buffer[0],&Layer,size);
		UndoNum++;

	}
	return 0;

}

int cUndoBuffer::Undo() {
	if (UndoNum>15)UndoNum = 15;

	if (UndoBuffer[UndoNum].copyAddress != 0 || UndoBuffer[UndoNum].buffer.size() == 0) {
		///GBA.LZ77UnComp((unsigned char*)&UndoBuffer[UndoNum].buffer[0],(unsigned char*)UndoBuffer[UndoNum].copyAddress);
		//memcpy(UndoBuffer[UndoNum].copyAddress,&UndoBuffer[UndoNum].buffer[0],UndoBuffer[UndoNum].buffer.size());
		std::copy(UndoBuffer[UndoNum].buffer.begin(), UndoBuffer[UndoNum].buffer.end(), UndoBuffer[UndoNum].copyAddress);
		UndoBuffer[UndoNum].buffer.resize(0);
		UndoBuffer[UndoNum].copyAddress = 0;


	}
	if (UndoNum>0)	UndoNum--;
	return 0;
}
