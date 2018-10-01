#pragma once
#include <stdio.h>
#include <vector>
#include "SimpleJSON-master\src\JSON.h"
#include <cstring>
#include <stdio.h>
#include "Logger.h"

using namespace std;
class DataContainer
{
public:
	char Name[1024];
	unsigned long Value;
	bool isPointer2Pointer; 
	unsigned long* DataArray;
	int MemberCount;
	char Section[1024];
	int RomSwitch;
	bool initedVal;
	unsigned long hash;
	DataContainer(int romSwitch, char* s, char* n, unsigned long o, bool iP2P, unsigned long* members, int len)//This needs to be read in from XML at some point. 
	{
	
		RomSwitch = romSwitch;
		MemberCount = len+1;
		DataArray = new unsigned long[len+1];
		memset(DataArray, 0, sizeof(unsigned long)*MemberCount);
		memcpy(DataArray, members, sizeof(unsigned long)*MemberCount);
		initedVal = true;
		sprintf(Name, "%s", n);
		sprintf(Section, "%s", s);
		Value = o;
		isPointer2Pointer = iP2P;	
	}

	DataContainer(int romSwitch, char* s, char* n, unsigned long o, bool iP2P, int members)//This needs to be read in from XML at some point. 
	{
		RomSwitch = romSwitch;
		MemberCount = members+1;
		DataArray = new unsigned long[MemberCount];
		memset(DataArray, 0, sizeof(unsigned long)*MemberCount);
		sprintf(Name, "%s", n);
		initedVal = false;
		sprintf(Section, "%s", s);
		Value = o;
		isPointer2Pointer = iP2P;

	}
	~DataContainer() {
		if (DataArray) {
			delete[] this->DataArray;
		}
	}
};
class GameConfiguration
{
private:

	vector<DataContainer*> Containers; 
	const char FileName[32] = "GameConfiguration.json";
	
public:
	void DefaultLoad(int romswitch);
	int RomSwitch;
	static GameConfiguration* mainCFG; 
	GameConfiguration( int RomSwitch);
	~GameConfiguration();
	void Load();
	void Save();
	static bool LiteralCompare(char* a, char* b);
	DataContainer* GetDataContainer(char* ContainerName);
	int SetDataContainer(DataContainer *ContainerName);
};

