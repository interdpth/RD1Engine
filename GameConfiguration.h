#pragma once
#include <stdio.h>
#include <vector>
#include "SimpleJSON-master\src\JSON.h"
#include <cstring>
#include "Logger.h"
#include <vector>
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
	char* str;
	int RomSwitch;
	bool initedVal;
	unsigned long hash;
	bool isString;
	vector<string> stringArray;
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
		isString = false;
	}

	DataContainer(int romSwitch, char* s, char* n, unsigned long o, bool iP2P,  char** members, int len)//This needs to be read in from XML at some point. 
	{
		RomSwitch = romSwitch;
		MemberCount = len;
		DataArray = NULL;
		
		for (int strCount = 0; strCount < len; strCount++)
		{
			stringArray.push_back(members[strCount]);			
		}
		initedVal = true;
		sprintf(Name, "%s", n);
		sprintf(Section, "%s", s);
		Value = o;
		isString = true;
		isPointer2Pointer = iP2P;
	}
	DataContainer(int romSwitch, char* s, char* n, unsigned long o, bool iP2P, int members)//This needs to be read in from XML at some point. 
	{
		RomSwitch = romSwitch;
		MemberCount = members;
		DataArray = new unsigned long[MemberCount];
		memset(DataArray, 0, sizeof(unsigned long)*MemberCount);
		sprintf(Name, "%s", n);
		initedVal = false;
		sprintf(Section, "%s", s);
		Value = o;
		isPointer2Pointer = iP2P;
		isString = false;
		/*unsigned long* data2 = (unsigned long*)(new char[strlen(param)]);
		delete[] data2;*/
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

