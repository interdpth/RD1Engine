#pragma once
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <map> 
#include "Logger.h"
#include "BaseTypes.h"
class OamFrameTable
{
public:
	char fileName[2048];
	SupportedTitles TitleType;
	void Read();
	void Save();
	HWND comboCotainer;
	std::map<int,std::vector<unsigned long>> OAMFrameTable;
	OamFrameTable(SupportedTitles theTitle, char* appPath);
	~OamFrameTable();
};

