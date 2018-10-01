#include "OamFrameTable.h"

void OamFrameTable::Read()
{
	FILE* fp = fopen(fileName, "r");
	char *buffer = NULL; 
	if (fp) {
		buffer = new char[1024];
		memset(buffer, 0, 1024);
		while (!feof(fp)) {
		
		
			fgets(buffer, 256, fp);
			if (memcmp(buffer, "//", 2) == 0)
				continue;

			if (memcmp(buffer, "END", 3) == 0)
				break;

			char* pch = strtok(buffer, "=");

			int id = -1;
			sscanf(pch, "%x", &id);
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
					sscanf(pch, "%x", &tmp);
					OAMFrameTable[id].push_back(tmp);
				}			
				pch = strtok(NULL, ",");
			}
			
		
		}
		delete[] buffer;
		buffer = 0;
		fclose(fp);
		fp = 0;
	}
	
}

OamFrameTable::OamFrameTable(int theTitle, char* fn)
{
	TitleType = theTitle;
	fileName = fn;

	for (int i = 0; i < 0xFF; i++)
	{
		OAMFrameTable[i].clear();
	}
	Read();

	for (int i = 0; i < 0xFF; i++)
	{
		if (OAMFrameTable[i].size()==0)
		{
			char buffer[256] = { 0 };
			sprintf(buffer, "No OAM offsets for %x", i);
			Logger::log->LogIt(Logger::ERRORZ, buffer);
		}
	}
}


OamFrameTable::~OamFrameTable()
{
}
