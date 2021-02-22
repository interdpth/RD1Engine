#include "clsRoomScrolls.h"


clsRoomScrolls::clsRoomScrolls()
{
	RoomScrollOffset = -1;
	Room = 0;
	pnt2ScrollPointer = -1;
	scrollpnt = -1;
	scrollInfoContainer = NULL;
}


clsRoomScrolls::~clsRoomScrolls()
{
}


void clsRoomScrolls::GetScrollArray()
{
	if (scrollInfoContainer == NULL) 
	{
		scrollInfoContainer = GameConfiguration::mainCFG->GetDataContainer("Scrolls");
	}
	MemFile::currentFile->seek(scrollInfoContainer->Value);
	MemFile::currentFile->fread(&scrollInfoContainer->DataArray[0], 4, scrollInfoContainer->MemberCount);
}



int clsRoomScrolls::GetScroll() {

	for (int scrollCounter = 0; scrollCounter < Scrolls.size(); scrollCounter++) delete Scrolls[scrollCounter];
	Scrolls.clear();
	MemFile* f = MemFile::currentFile;
	f->seek(scrollpnt);
	f->fread(&Room, 1, 1);
	if (Room == 0xFF) {
		return 0;
	}
	int max = 0;
	f->fread(&max, 1, 1);

	for (int i = 0; i < max; i++) {
		ObjectScroll*  sD = new ObjectScroll();
		sD->SetXPosition(f->fgetc());
		sD->SetWidth(f->fgetc());
		sD->SetYPosition(f->fgetc());
		sD->SetHeight(f->fgetc());

		f->fread(&sD->unkData1, 2, 1);
		f->fread(&sD->unkData2, 2, 1);
		
		Scrolls.push_back(sD);
	}
	oldScrollCount = Scrolls.size();
	return 0;
}//End Function

int  clsRoomScrolls::SaveScroll(GBAMethods* mGBA) 
{
	MemFile* thisFile = MemFile::currentFile;

	if (Scrolls.size() > oldScrollCount) {
		//We need to find space...

		//destroy the old data 
		MemFile::currentFile->seek(scrollpnt);
		unsigned char writebyte = 0xFF;

		thisFile->fputc(writebyte);
		thisFile->fputc(writebyte);
		thisFile->fwrite(&writebyte, oldScrollCount, 8);



		//write data at new address
		scrollpnt = mGBA->FindFreeSpace(1 + 1 + (Scrolls.size() * 4 * 2) + 32, 0xFF) + 0x8000000;
		thisFile->seek(pnt2ScrollPointer);
		thisFile->fwrite(&scrollpnt, 4, 1);
		scrollpnt -= 0x8000000;


	}

	thisFile->seek(scrollpnt);

	if (Room == 0xFF) {
		return 0;
	}
	thisFile->fwrite(&Room, 1, 1);
	int	Number = Scrolls.size();
	thisFile->fwrite(&Number, 1, 1);

	for (int i = 0; i < Number; i++) 
	{		
		ObjectScroll*  sD = Scrolls[i];
		
		thisFile->fputc(sD->XPosition());
		thisFile->fputc(sD->Width());
		thisFile->fputc(sD->YPosition());
		thisFile->fputc(sD->Height());
		thisFile->fwrite(&sD->unkData1, 2, 1);
		thisFile->fwrite(&sD->unkData2, 2, 1);
	}


	mGBA->Reopen();
	return 0;
}//End Function


int clsRoomScrolls::initScroll(int area, int room) {//returns the address
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
		MemFile::currentFile->fread(&ScrollOffset, sizeof(long), 1);

		//ScrollOffset = ScrollOffset

	for (counter = 0; counter < 256; counter++)
	{
		/*this too*/
		MemFile::currentFile->seek(ScrollOffset - 0x8000000 + counter * 4);
		tRoom = MemFile::currentFile->fgetc();
		//start_Check:
		if (tRoom != room) {

			/* That need to be modified*/
			if (tRoom == 0xFF) {
				scrollpnt = ScrollOffset - 0x8000000;

				RoomScrollOffset = Scroll + counter*4;

				pnt2ScrollPointer = Scroll - 0x8000000 + counter * 4;


				unsigned long offset = 0;
				char buffer[256];
				sprintf(buffer, "Loading Area: %d Room: %X, %X,%X,%X", area, room, scrollpnt, RoomScrollOffset, pnt2ScrollPointer);

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
			sprintf(buffer, "Loading Area: %d Room: %X, %X,%X,%X", area, room, scrollpnt, RoomScrollOffset, pnt2ScrollPointer);

			Logger::log->LogIt(Logger::DEBUG, buffer);


			return RoomScrollOffset;
		}//End if 
	}



	unsigned long offset = 0;
	char buffer[256];
	sprintf(buffer, "Loading Area: %d Room: %X, %X,%X,%X", area, room, scrollpnt, RoomScrollOffset, pnt2ScrollPointer);

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
	RECT* thisScroll = Scrolls[ScrollNum]->GetRect();
	/*
	* Given an X,Y
	* This function will find what rectangle you are in, then will go deeper and find
	* if said rectangle is intersecting this will take some time to code -.-

	It wasn't really that interesting.
	*/

	if ((Room != 0xFF) && (thisScroll->left != 0xFF) && (thisScroll->bottom != 0xFF)) {

		MainX = thisScroll->left;
		MainY = thisScroll->top;
		MainW = thisScroll->right;
		MainH = thisScroll->bottom;;
		if (((MainX) <= X) && ((MainW) >= X) &&
			((MainY) <= Y) && ((MainH) >= Y))
		{
			return ScrollNum;
		}


	}
	return -1; // Nothing was found

}

