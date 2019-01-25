#include "DoorManager.h"

vDoor::vDoor(sDoor src) {
	rawDoor = src;
	memset(&virtualDoor, 0, sizeof(virtualDoor));
	virtualDoor.sX = src.XEntrance;
	virtualDoor.sY = src.YEntrance;
	virtualDoor.Width = src.DWidth;
	virtualDoor.Height = src.DHeight; 
}



DoorManager::DoorManager(GBAMethods* gba)
{
	doorInfoContainer = NULL;
	OriginalDoorCount = 0;
	_gbaMethods = gba;
}


DoorManager::~DoorManager()
{
}


int DoorManager::AddDoor(int RoomNum)
{
	sDoor newDoor;
	memset(&newDoor, 0, sizeof(newDoor));
	newDoor.OwnerRoom = RoomNum;
	newDoor.XEntrance = 1;
	newDoor.YEntrance = 1;
	newDoor.DWidth= 1;
	newDoor.DHeight = 1;
	newDoor.DoorType = 0x14;
	//Fake setup is done, now get our vDoor
	vDoor *me = new vDoor(newDoor);
	Doors.push_back(*me);
	//Also update the currentroom index

	CurrentRoomDoorIndexes.push_back(Doors.size() - 1);
	
	return CurrentRoomDoorIndexes.size();
}

int   DoorManager::DeleteDoor(int Room, int doorNum)
{

	Doors.erase(Doors.begin()+doorNum);


	LoadDoors(Room);
	return CurrentRoomDoorIndexes.size();
}

//Combos[cRoom].GetListIndex()
int   DoorManager::GetDoor(int Room, int X, int Y)
{
	int             i = 0;
	int             door = 0;
	int             curX = 0;
	int             curY = 0;
	int             curH = 0;
	int             curW = 0;

	// Function returns what door the is touching
	for (i = 0; i < CurrentRoomDoorIndexes.size(); i++)
	{

		if (Doors[CurrentRoomDoorIndexes[i]].rawDoor.OwnerRoom == Room)
		{
			curX = Doors[CurrentRoomDoorIndexes[i]].virtualDoor.sX;
			curY = Doors[CurrentRoomDoorIndexes[i]].virtualDoor.sY;
			curH = (Doors[CurrentRoomDoorIndexes[i]].virtualDoor.Height); // -Doors[CurrentRoomDoorIndexes[i]].virtualDoor.sY)+1;

			curW = (Doors[CurrentRoomDoorIndexes[i]].virtualDoor.Width); // -Doors[CurrentRoomDoorIndexes[i]].virtualDoor.sX)+1;
														 // ShowWindow((HWND)hDoors[i],SW_SHOW);
														 // MoveWindow((HWND)hDoors[i],(unsigned short)(curX-minX)*16,(unsigned short)(curY-minY)*16,(curW+1)*16,(curH-curY+1)*16,0);

			if ((curX <= X) &&
				(curW >= X) &&
				(curY <= Y) &&
				(curH >= Y))
			{

				return i;
			}
		}
	}
	return -1;
}
int DoorManager::SaveDoorOffsets()
{
	if (doorInfoContainer == NULL) {
		doorInfoContainer = GameConfiguration::mainCFG->GetDataContainer("Doors");
	}
	MemFile::currentFile->seek(doorInfoContainer->Value);
	MemFile::currentFile->fwrite(doorInfoContainer->DataArray, 4, doorInfoContainer->MemberCount);
	return 0;
}


int DoorManager::GetDoorArray(FILE* fp)
{

	if (doorInfoContainer == NULL) {
		doorInfoContainer = GameConfiguration::mainCFG->GetDataContainer("Doors");
	}
	MemFile::currentFile->seek(doorInfoContainer->Value);
MemFile::currentFile->fread(doorInfoContainer->DataArray, 4, doorInfoContainer->MemberCount);
	return 0;



}


unsigned long DoorManager::SetAreaOffset(int Area, int newOffset)
{

	if (doorInfoContainer == NULL) {
		doorInfoContainer = GameConfiguration::mainCFG->GetDataContainer("Doors");
	}
	doorInfoContainer->DataArray[Area]= newOffset;
	return doorInfoContainer->DataArray[Area];

}

unsigned long DoorManager::GetAreaOffset(int Area) {

	if (doorInfoContainer == NULL) {
		doorInfoContainer = GameConfiguration::mainCFG->GetDataContainer("Doors");
	}
	return doorInfoContainer->DataArray[Area];
}
//Combos[cArea].GetListIndex() 
int DoorManager::SaveDoors(int area) {

	unsigned char h = 0;
	int i = 0;
	MemFile* thisFile = MemFile::currentFile;


	bool overWriteDataPointer = false;


	if (DoorNum() > OriginalDoorCount) {
		overWriteDataPointer = true;
		int newOffset = _gbaMethods->FindFreeSpace(DoorNum()*sizeof(sDoor) + sizeof(sDoor), 0xFF);
		SetAreaOffset(area, newOffset+0x8000000);
		SaveDoorOffsets();
	}


	




	thisFile->seek(GetAreaOffset(area) - 0x8000000);
	



	for (i = 0; i < DoorNum(); i++) {
		sDoor tmpDoor;
		memcpy(&tmpDoor, 			&Doors[i].rawDoor, sizeof(sDoor));
		MousePointer* actualMP = &Doors[i].virtualDoor;
		thisFile->fwrite(&tmpDoor.DoorType, sizeof(unsigned char), 1);
		thisFile->fwrite(&tmpDoor.OwnerRoom, sizeof(unsigned char), 1);
		
		thisFile->fwrite(&actualMP->sX, sizeof(unsigned char), 1);
		
		thisFile->fwrite(&actualMP->Width, sizeof(unsigned char), 1);

		thisFile->fwrite(&actualMP->sY, sizeof(unsigned char), 1);
		
		thisFile->fwrite(&actualMP->Height, sizeof(unsigned char), 1);

		thisFile->fwrite(&tmpDoor.DestDoor, sizeof(unsigned char), 1);
		thisFile->fwrite(&tmpDoor.xExitDistance, sizeof(unsigned char), 1);
		thisFile->fwrite(&tmpDoor.yExitDistance, sizeof(unsigned char), 1);
		thisFile->fwrite(&tmpDoor.doorNum, sizeof(unsigned char), 1);
		thisFile->fwrite(&tmpDoor.u3, sizeof(unsigned char), 1);
		thisFile->fwrite(&tmpDoor.u4, sizeof(unsigned char), 1);
		
	}

	unsigned long endPoint = 0;
	//Marks end of Door Reading. 
	thisFile->fwrite(&endPoint, 4, 1);
	thisFile->fwrite(&endPoint, 4, 1);
	thisFile->fwrite(&endPoint, 4, 1);
	//fclose(_gbaMethods->ROM);
	//_gbaMethods->ROM = fopen(_gbaMethods->FileLoc,"r+bw+b");

	

	return 0;
}

//please call 	
/*Combos[cD1].Clear();
Combos[cD2].Clear();
*/
int DoorManager::SetupDoors(long area) {

	int i = 0;
	
	//If File = "ZM" Then





	
	Doors.clear();
	if (_gbaMethods->ROM) {

		MemFile::currentFile->seek(GetAreaOffset(area) - 0x8000000);
	

		for (i = 0; i < 255; i++) {
			sDoor tmpDoor;
			MemFile::currentFile->fread(&tmpDoor.DoorType, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.OwnerRoom, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.XEntrance, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.DWidth, sizeof(unsigned char), 1);

			MemFile::currentFile->fread(&tmpDoor.YEntrance, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.DHeight, sizeof(unsigned char), 1);

			MemFile::currentFile->fread(&tmpDoor.DestDoor, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.xExitDistance, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.yExitDistance, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.doorNum, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.u3, sizeof(unsigned char), 1);
			MemFile::currentFile->fread(&tmpDoor.u4, sizeof(unsigned char), 1);

			bool stopReading = true;
			if (i == 98) {
				stopReading = true;
			}
			for (int j = 0; j < sizeof(sDoor)/4; j++) {
				char* firstCast = (char*)&tmpDoor;
				long blah = 0;
				memcpy(&blah, (unsigned long*)&firstCast[j * 4], 4);

				unsigned long val = (unsigned long)blah;
				if (val!= 0) {
					stopReading = false;
				}
			}
			if (stopReading == true) {
				break;
			}
			//memcpy(&Doors[i], &tmpDoor, sizeof(sDoor));
			Doors.push_back(tmpDoor);
		}

	}


	//Writing to a file;
/*
	FILE * fp = fopen("C:\\doors.txt","w");
	if(fp){
	char debugbuf[256];
	for(i=0;i<DoorNum;i++){

	sprintf(debugbuf,"*********************Door %X********************\r\n",i);
	fputs(debugbuf,fp);
	sprintf(debugbuf,"Door Type %X\r\n", Doors[i].DoorType);
	fputs(debugbuf,fp);
	sprintf(debugbuf,"Owner Room %X\r\n",Doors[i].OwnerRoom);
	fputs(debugbuf,fp);
	sprintf(debugbuf,"X Entrance %X\r\n",Doors[i].virtualDoor.sX);
	fputs(debugbuf,fp);

	sprintf(debugbuf,"Door Width %X\r\n",Doors[i].virtualDoor.Width);
	fputs(debugbuf,fp);
	sprintf(debugbuf,"Y Entrance %X\r\n",Doors[i].virtualDoor.sY);
	fputs(debugbuf,fp);

	sprintf(debugbuf,"Door Height %X\r\n",Doors[i].virtualDoor.Height);
	fputs(debugbuf,fp);



	sprintf(debugbuf,"Destination Door %X\r\n", Doors[i].DestDoor);
	fputs(debugbuf,fp);

	sprintf(debugbuf,"Exit Length %X\r\n",Doors[i].ExitDoorLength);
	fputs(debugbuf,fp);
	sprintf(debugbuf,"Door Unknowns: %X,%X,%X,%X\r\n",Doors[i].u1,Doors[i].u2,Doors[i].u3,Doors[i].u4);
	fputs(debugbuf,fp);

	sprintf(debugbuf,"************************************************\r\n");
	fputs(debugbuf,fp);
	}
	fclose(fp);
	}else{
	MessageBox(0,"Lame",NULL,MB_OK);
	}*/
	return 0;
}



unsigned short DoorManager::DoorNum()
{
	return Doors.size();
}

int DoorManager::LoadDoors(int Room) {

	int i = 0;


	CurrentRoomDoorIndexes.clear();


	for (i = 0; i<DoorNum(); i++) {
		if ((Doors[i].rawDoor.OwnerRoom == Room)) {

			CurrentRoomDoorIndexes.push_back( i);

			
		}
	}

	OriginalDoorCount = DoorNum();
	return 0;
}
