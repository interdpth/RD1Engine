#include "clsRoomScrolls.h"


clsRoomScrolls::clsRoomScrolls()
{
	RoomScrollOffset = -1;

	pnt2ScrollPointer = -1;
	scrollpnt = -1;
	scrollInfoContainer = NULL;
}


clsRoomScrolls::~clsRoomScrolls()
{
}


void clsRoomScrolls::GetScrollArray(FILE* fp)
{
	if (scrollInfoContainer == NULL) 
	{
		scrollInfoContainer = GameConfiguration::mainCFG->GetDataContainer("Scrolls");
	}
	MemFile::currentFile->seek(scrollInfoContainer->Value);
	MemFile::currentFile->fread(scrollInfoContainer->DataArray, 4, scrollInfoContainer->MemberCount, fp);
}



int clsRoomScrolls::GetScroll(FILE* fp) {
	mScrollInfo.Scrolls.clear();
	mScrollInfo.Number = 0;
	MemFile::currentFile->seek(scrollpnt);
	MemFile::currentFile->fread(&mScrollInfo.Room, 1, 1, fp);
	if (mScrollInfo.Room == 0xFF) {
		return 0;
	}
	MemFile::currentFile->fread(&mScrollInfo.Number, 1, 1, fp);

	for (int i = 0; i < mScrollInfo.Number; i++) {
		scrollData sD;
		sctype tmp;
		MemFile::currentFile->fread(&tmp.xStart, 1, 1, fp);
		MemFile::currentFile->fread(&tmp.xEnd, 1, 1, fp);
		MemFile::currentFile->fread(&tmp.yStart, 1, 1, fp);
		MemFile::currentFile->fread(&tmp.yEnd, 1, 1, fp);
		memset(&sD.rect, 0, sizeof(MousePointer));
		sD.rect.sX = tmp.xStart;
		sD.rect.sY = tmp.yStart;
		sD.rect.Height = tmp.yEnd;
		sD.rect.Width = tmp.xEnd;

		MemFile::currentFile->fread(&sD.unkData, 4, 1, fp);
		if (sD.unkData != -1) {
			sD.unkData = sD.unkData;
		}
		mScrollInfo.Scrolls.push_back(sD);
	}
	mScrollInfo.oldScrollCount = mScrollInfo.Scrolls.size();
	return 0;
}//End Function



Scroller*  clsRoomScrolls::GetScrollInfo()
{

	return &mScrollInfo;
}

int  clsRoomScrolls::SaveScroll(GBAMethods* mGBA) {

	MemFile* thisFile = MemFile::currentFile;

	if (mScrollInfo.Scrolls.size() > mScrollInfo.oldScrollCount) {
		//We need to find space...

		//destroy the old data 
		MemFile::currentFile->seek(scrollpnt);
		unsigned char writebyte = 0xFF;

		thisFile->fputc(writebyte);
		thisFile->fputc(writebyte);
		thisFile->fwrite(&writebyte, mScrollInfo.oldScrollCount, 8, (FILE*)NULL);



		//write data at new address
		scrollpnt = mGBA->FindFreeSpace(1 + 1 + (mScrollInfo.Number * 4 * 2) + 32, 0xFF) + 0x8000000;
		thisFile->seek(pnt2ScrollPointer);
		thisFile->fwrite(&scrollpnt, 4, 1, (FILE*)NULL);
		scrollpnt -= 0x8000000;


	}

	thisFile->seek(scrollpnt);

	if (mScrollInfo.Room == 0xFF) {
		return 0;
	}
	thisFile->fwrite(&mScrollInfo.Room, 1, 1, (FILE*) NULL);
	mScrollInfo.Number = mScrollInfo.Scrolls.size();
	thisFile->fwrite(&mScrollInfo.Number, 1, 1, (FILE*) NULL);

	for (int i = 0; i < mScrollInfo.Number; i++) {
		scrollData sD = mScrollInfo.Scrolls[i];


		thisFile->fwrite(&sD.rect.sX, 1, 1, (FILE*)NULL);
		thisFile->fwrite(&sD.rect.Width, 1, 1, (FILE*)NULL);
		thisFile->fwrite(&sD.rect.sY, 1, 1, (FILE*)NULL);
		thisFile->fwrite(&sD.rect.Height, 1, 1, (FILE*)NULL);
		thisFile->fwrite(&sD.unkData, 4, 1, (FILE*)NULL);

	}


	mGBA->Reopen();
	return 0;
}//End Function


int clsRoomScrolls::initScroll(FILE* fp, int area, int room) {//returns the address
	unsigned char tRoom;
	unsigned long ScrollOffset;
	unsigned long counter;
	unsigned long Scroll = scrollInfoContainer->DataArray[area];
	counter = 0;
	if (room == 0x2a)
	{
		counter = 0;
	}


		MemFile::currentFile->seek(Scroll - 0x8000000 );
		MemFile::currentFile->fread(&ScrollOffset, sizeof(long), 1, fp);

		//ScrollOffset = ScrollOffset

	for (counter = 0; counter < 256; counter++)
	{
		/*this too*/
		MemFile::currentFile->seek(ScrollOffset - 0x8000000 + counter * 4);
		tRoom = fgetc(fp);
		//start_Check:
		if (tRoom != room) {

			/* That need to be modified*/
			if (tRoom == 0xFF) {
				scrollpnt = ScrollOffset - 0x8000000;

				RoomScrollOffset = Scroll + counter*4;

				pnt2ScrollPointer = Scroll - 0x8000000 + counter * 4;


				unsigned long offset = 0;
				char buffer[256];
				sprintf(buffer, "Loading Area: %d Room: %x, %x,%x,%x", area, room, scrollpnt, RoomScrollOffset, pnt2ScrollPointer);

				Logger::log->LogIt(Logger::DEBUG, buffer);



				break;
			}
		
		
		}
		else {
			scrollpnt = ScrollOffset - 0x8000000;
			RoomScrollOffset = Scroll + counter * 4;

			pnt2ScrollPointer = Scroll - 0x8000000 + counter * 4;


			unsigned long offset = 0;
			char buffer[256];
			sprintf(buffer, "Loading Area: %d Room: %x, %x,%x,%x", area, room, scrollpnt, RoomScrollOffset, pnt2ScrollPointer);

			Logger::log->LogIt(Logger::DEBUG, buffer);


			return RoomScrollOffset;
		}//End if 
	}



	unsigned long offset = 0;
	char buffer[256];
	sprintf(buffer, "Loading Area: %d Room: %x, %x,%x,%x", area, room, scrollpnt, RoomScrollOffset, pnt2ScrollPointer);

	Logger::log->LogIt(Logger::DEBUG, buffer);




	return RoomScrollOffset;
}//End Function

int clsRoomScrolls::EditScrollData(GBAMethods* mGBA) {

	////Find Enough Space
	//unsigned long blah = 0;
	//int i = 0;


	//MemFile::currentFile->seek(initScroll() - 0x8000000);
	//scrollpnt = GBA.FindFreeSpace(scroll.Number * 8 + 2 + 4, 0xFF);
	//blah = scrollpnt + 0x8000000;
	//fwrite(&blah, 4, 1, GBA.ROM);
	SaveScroll(mGBA);
	mGBA->Reopen();
	return 0;
}//End Function


int clsRoomScrolls::Findmeascroll(int X, int Y, int ScrollNum)//cboScroll.GetListIndex()
{
	int             scrollnum = 0;
	int             MainX = 0;
	int             MainY = 0;
	int             MainH = 0;
	int             MainW = 0;
	int             curX = 0;
	int             curY = 0;
	int             curH = 0;
	int             curW = 0;
	int             i = 0;
	int             j = 0;
	MousePointer thisScroll = mScrollInfo.Scrolls[ScrollNum].rect;
	/*
	* Given an X,Y
	* This function will find what rectangle you are in, then will go deeper and find
	* if said rectangle is intersecting this will take some time to code -.-

	It wasn't really that interesting.
	*/

	if ((mScrollInfo.Room != 0xFF) && (thisScroll.sX != 0xFF) && (thisScroll.Height != 0xFF)) {

		MainX = thisScroll.sX;
		MainY = thisScroll.sY;
		MainW = thisScroll.Width;
		MainH = thisScroll.Height;
		if (((MainX) <= X) && ((MainW) >= X) &&
			((MainY) <= Y) && ((MainH) >= Y))
		{

			return ScrollNum;
		}


	}
	return -1; // Nothing was found

}

