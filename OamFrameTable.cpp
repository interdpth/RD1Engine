#include "OamFrameTable.h"
#include <fstream>
#include <string.h>
#include <iostream>
using namespace std;
void OamFrameTable::Read()
{
	//FILE* fp = fopen(fileName, "r");
	//char *buffer = NULL; 

	//if (fp) {
	//	std::string filesrc; 

	//	buffer = new char[1024];
	//	memset(buffer, 0, 1024);

	//	


	/*	while (!feof(fp)) {
		
		
			fgets(buffer, 256, fp);
			if (memcmp(buffer, "//", 2) == 0)
				continue;

			if (memcmp(buffer, "END", 3) == 0)
				break;

			char* pch = strtok(buffer, "=");

			int id = -1;
			int k = sscanf(pch, "%X", &id);
			if (id > 256)
			{
				id = 0;
			}
			pch = strtok(NULL, ",");
			if (id == 0x1a) {
				id = 0x1a;
			}
			while (pch != NULL)
			{
				unsigned long tmp = 0;
			
				if (id < 256)
				{
					if (pch[strlen(pch) - 1] == '\n')
					{
						pch[strlen(pch) - 1] = '\0';
					}
					int k = sscanf(pch, "%X", &tmp);
					OAMFrameTable[id].push_back(tmp);
				}			
				pch = strtok(NULL, ",");
			}
			
		
		}*/

		ifstream in(fileName);

		if (!in) {
			cout << "Cannot open input file.\n";
			
		}

		char buffer[1024];

		while (in) {
			in.getline(buffer, 512);  // delim defaults to '\n'
			if (memcmp(buffer, "//", 2) == 0)
				continue;

			if (memcmp(buffer, "END", 3) == 0)
				break;
			if (strlen(buffer) == 0) continue;
			char* pch = strtok(buffer, "=");

			int id = -1;
			int k = sscanf(pch, "%X", &id);
			if (id ==-1)
			{
				continue;
			}
			pch = strtok(NULL, ",");
			if (id == 0x1a) {
				id = 0x1a;
			}
			while (pch != NULL)
			{
				unsigned long tmp = 0;

				if (id < 256 || id > 255)
				{
					if (pch[strlen(pch) - 1] == '\n')
					{
						pch[strlen(pch) - 1] = '\0';
					}
					int k = sscanf_s(pch,"%X", &tmp);
					OAMFrameTable[id].push_back(tmp);
				}
				pch = strtok(NULL, ",");
			}

		}

		in.close();

	/*	delete[] buffer;
		buffer = 0;
		fclose(fp);*/
		//fp = 0;
	//}
	
}

bool OamFrameTable::FramesExist(int id)
{
	return OAMFrameTable.count(id) > 0  &&   OAMFrameTable[id].front() != 0;
}

//Frame table init
OamFrameTable::OamFrameTable(SupportedTitles theTitle, char* appPath)
{
	TitleType = theTitle;
	
	char * zmString = "\\SpriteList.txt";
	char* mfString = "\\MF_oam.txt";
	char* titlePointer = theTitle == SupportedTitles::titleMF ? mfString : zmString;
	std::string filePath = appPath + std::string(titlePointer);

	sprintf(fileName, "%s", filePath.c_str());
	for (int i = 0; i < OAMFrameTable.size(); i++)
	{
		OAMFrameTable[i].clear();
	}
	Read();

	for (int i = 0; i <  OAMFrameTable.size(); i++)
	{
		if (OAMFrameTable[i].size()==0)
		{
			char buffer[256] = { 0 };
			sprintf(buffer, "No OAM offsets for %X", i);
			Logger::log->LogIt(Logger::ERRORZ, buffer);
		}
	}
}


OamFrameTable::~OamFrameTable()
{
}
