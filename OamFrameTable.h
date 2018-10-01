#pragma once
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <map> 
#include "Logger.h"
class OamFrameTable
{
public:
	char* fileName;
	int TitleType; 
	void Read();
	void Save();
	HWND comboCotainer;
	std::map<int,std::vector<unsigned long>> OAMFrameTable;
	OamFrameTable(int theTitle, char* fn);
	~OamFrameTable();
};

