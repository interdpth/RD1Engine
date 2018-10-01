#include "cCopyData.h"
int cCopyData::Copy(int Width, RECT data, unsigned short *pnt) {///data size is basically size of unsigned char or unsigned short ie 1 or 2

	int x = 0;  // DON'T NEED THIS
	int y = 0;
	//First kill buffer

	///Set datahold rect

	DataHold.left = 0;
	DataHold.top = 0;
	DataHold.bottom = data.bottom - data.top;
	DataHold.right = data.right - data.left;
	CopyWidth = Width;
	//resize the copy buffer
	//buffersize=(DataHold.right + (DataHold.bottom*Width))*datasize;
	//buffer = new unsigned char[buffersize];
	buffer.resize((DataHold.right * DataHold.bottom) * 2);
	std::fill(buffer.begin(), buffer.end(), 0);

	unsigned short* dst = &buffer[0];                   // pointer to start of dest
	unsigned short* src = pnt;
	for (y = 0; y < DataHold.bottom; y++) {
		for (x = 0; x< DataHold.right; x++) {

			dst[x + (y*DataHold.right)] = src[(data.left + x) + ((data.top + y)*Width)];
		}
	}
	return 0;

}
int cCopyData::Paste(unsigned short *pnt) {
	int x = 0;  // DON'T NEED THIS
	int y = 0;
	if (buffer.size() == 0) {
		MessageBox(0, "Please copy a tile first.", "None", MB_OK);
		return 0;
	}
	unsigned short* src = &buffer[0];                   // pointer to start of dest
	unsigned short* dst = pnt;  // pointer to start of source
	for (y = 0; y<DataHold.bottom; y++) {
		for (x = 0; x<DataHold.right; x++) {

			dst[x + (y)* (CopyWidth)] = src[x + (y*DataHold.right)];
			//BaseGame::theGame->mainRoom->mapMgr->GetLayer(MapManager::LevelData)->TileBuf2D[x + (y) * (CopyWidth)]=src[x+(y*DataHold.right)];
		}
	}


	return 0;

}



cCopyData::cCopyData() {
}

cCopyData::~cCopyData() {

}

void cCopyData::Dump() {/*
						FILE* fp = fopen("C:\\Users\\Matt\\Desktop\\Super Metroid (JU) [!]\\Test\\copydump.bin","w+");
						int thissize=buffer.size();
						if(fp){
						MemFile::currentFile->seek(0);
						fwrite(&buffer[0],thissize,1,fp);

						fclose(fp);
						}

						*/

}


