#include "GameConfiguration.h"


GameConfiguration* GameConfiguration::mainCFG;

bool GameConfiguration::LiteralCompare(char* a, char* b)
{
	int len = strlen(a);
	int i = 0;
	for (i = 0; i < len; i++)
	{
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

void GameConfiguration::DefaultLoad(int romSwitch)
{
	Containers.clear();
	float zoomTypes[3] = { 1,1.5,2 };
	Containers.push_back(new DataContainer(-1, "Framework", "ZoomStates", 0, false, (unsigned long*)zoomTypes, 8));
	Containers.push_back(new DataContainer(-1, "Framework", "UIState", 2, false, 0));
	Containers.push_back(new DataContainer(1, "Framework", "UIState", 2, false, 0));
	Containers.push_back(new DataContainer(0, "Framework", "UIState", 2, false, 0));
	if (RomSwitch == 0 || RomSwitch==1) {
		Containers.push_back(new DataContainer(0, "Framework", "Doors", 0x75FAA8, false, 8));
		Containers.push_back(new DataContainer(0, "Framework", "Scrolls", 0x75FD28, false, 0xB));
		Containers.push_back(new DataContainer(0, "Framework", "Connections", 0x05EEB8, false, 1));
		Containers.push_back(new DataContainer(0, "Framework", "Tileset", 0x33DFDC, false, 0x50));
		Containers.push_back(new DataContainer(0, "Framework", "Areas", 0x75FAC4, false, 0x7));
		Containers.push_back(new DataContainer(0, "Framework", "ZoomStates", 0, false, (unsigned long*)zoomTypes, 8));
		Containers.push_back(new DataContainer(0, "Framework", "AnimReference", 0x35FA78, false, 0x30));
		Containers.push_back(new DataContainer(0, "Framework", "AnimTileset", 0x35F948, false, 0x0));

		Containers.push_back(new DataContainer(0, "Framework", "TextGFX", 0x415460, false, 0x8000));
		Containers.push_back(new DataContainer(0, "Framework", "TextPal", 0x375738, false, 0x20));

		Containers.push_back(new DataContainer(0, "Framework", "SpriteGFX", 0x75EBF8, false, 0));
		Containers.push_back(new DataContainer(0, "Framework", "SpritePal", 0x75EEF0, false, 0x0));
		Containers.push_back(new DataContainer(0, "Framework", "SpritePalSizes", 0x75EBF8, false, 0x0));

		unsigned long zmRoomsPerArea[8] = { 0x2A, 0x2A, 0x39, 0x21, 0x14, 0x16, 0x73 };
		Containers.push_back(new DataContainer(0, "Framework", "RoomsPerArea", 0, false, zmRoomsPerArea, 8));	



		Containers.push_back(new DataContainer(1, "Framework", "Doors", 0x79B894, false, 10));
		Containers.push_back(new DataContainer(1, "Framework", "Scrolls", 0x79BB08, false, 0xC));
		Containers.push_back(new DataContainer(1, "Framework", "Connections", 0x6945c, false, 1));
		Containers.push_back(new DataContainer(1, "Framework", "Tileset", 0x3BF888, false, 0x80));
		Containers.push_back(new DataContainer(1, "Framework", "Areas", 0x79B8BC, false, 0xA));
		Containers.push_back(new DataContainer(1, "Framework", "SpriteSizes", 0x2E4A50, false, 0xC0));
		Containers.push_back(new DataContainer(1, "Framework", "AnimReference", 0x3C8F90, false, 0x30));
		Containers.push_back(new DataContainer(1, "Framework", "AnimTileset", 0x3C8D58, false, 0x0));
		Containers.push_back(new DataContainer(1, "Framework", "TextGFX", 0x682FAC, false, 0x8000));
		Containers.push_back(new DataContainer(1, "Framework", "TextPal", 0x598d0c, false, 0x20));
		Containers.push_back(new DataContainer(1, "Framework", "ZoomStates", 0, false, (unsigned long*)zoomTypes, 8));
		Containers.push_back(new DataContainer(1, "Framework", "SpriteGFX", 0x79A5D8, false, 0));
		Containers.push_back(new DataContainer(1, "Framework", "SpritePal", 0x79A8D4, false, 0x0));

		unsigned long mfRoomsPerArea[10] = { 0x57,0x36,0x3D,0x27,0x30,0x34,0x29,0x4,0x4,0x4 };
		Containers.push_back(new DataContainer(1, "Framework", "RoomsPerArea", 0, false, mfRoomsPerArea, 10));
	}
}

void GameConfiguration::Load()
{
	FILE* fp = fopen(GameConfiguration::FileName, "r");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp) + 1;

		if (size < 10)
		{
			DefaultLoad(RomSwitch);
			fclose(fp);
			return;
		}

		char* buffer = new char[size];
		memset(buffer, 0, size);
		fseek(fp, 0, SEEK_SET);
		fread(buffer, 1, size - 1,fp);


		JSONValue* jsonObj = JSON::Parse((const char*)buffer);
		JSONArray j = jsonObj->AsArray();
		for each(JSONValue* o in j)
		{
			std::string Name;
			unsigned long Value;
			bool isPointer2Pointer;
			unsigned long* DataArray;
			int MemberCount;
			std::string Section;
			int RomSwitch;
		
			bool initedVal;
			// Fetch the keys and print them out.
			/*std::vector<std::wstring> keys = o->ObjectKeys();

			std::vector<std::wstring>::iterator iter = keys.begin();
			while (iter != keys.end())
			{*/
			string strValue;
			RomSwitch = o->Child(L"RomSwitch")->AsNumber();
			wstring data = o->Child(L"Name")->AsString();
			strValue.assign(data.begin(), data.end());

			Name = strValue;

			data = o->Child(L"Section")->AsString();
			strValue.assign(data.begin(), data.end());
			Section = strValue;

			swscanf(o->Child(L"Value")->AsString().c_str(),L"%X", &Value);
				//swprintf(buffer, L"0x%X", o->Child(L"Section")->);

			isPointer2Pointer = o->Child(L"isPointer2Pointer")->AsNumber();
			MemberCount  = o->Child(L"MemberCount")->AsNumber();
			DataArray = NULL;
			if (MemberCount)
			{
				JSONArray data2 = o->Child(L"DataArray")->AsArray();
				
				if (data2.size() > MemberCount)
				{
					MemberCount = data2.size();
					DataArray = new unsigned long[data2.size()];
					memset(DataArray, 0, data2.size() * 4);
					for (int dataCounter = 0; dataCounter < data2.size(); dataCounter++)
					{
						DataArray[dataCounter] = data2[dataCounter]->AsNumber();
					}
				}
				else if (MemberCount > data2.size())
				{
					DataArray = new unsigned long[MemberCount];
					memset(DataArray, 0, MemberCount * 4);
				}
				else
				{
					DataArray = new unsigned long[data2.size()];
					memset(DataArray, 0, data2.size() * 4);
					for (int dataCounter = 0; dataCounter < data2.size(); dataCounter++)
					{
						DataArray[dataCounter] = (unsigned long)data2[dataCounter]->AsNumber();
					}
				}
				
			}
			DataContainer* dc;
			if (MemberCount)
			{
				dc = new DataContainer(RomSwitch, (char*)Section.c_str(), (char*)Name.c_str(), Value, isPointer2Pointer,DataArray, MemberCount);
			}
			else
			{
				dc = new DataContainer(RomSwitch, (char*)Section.c_str(), (char*)Name.c_str(), Value, isPointer2Pointer, MemberCount);
			}
			if (dc)
			{
				Containers.push_back(dc);
			}
		}
		if(fp)
		{
			fclose(fp);
		}
		delete jsonObj;
	}
}


void GameConfiguration::Save()
{
	JSONObject root;
	JSONArray arr;
	if (Containers.size() < 2)
	{
		return;
	}
	for each(DataContainer* entry in Containers)
	{

		JSONObject thisEntry;
		thisEntry[L"ObjectName"] = new JSONValue(L"DataContainer");
		thisEntry[L"RomSwitch"] = new JSONValue(entry->RomSwitch);
		thisEntry[L"Name"] = new JSONValue(wstring(&entry->Name[0], &entry->Name[strlen(entry->Name)]));
		thisEntry[L"Section"] = new JSONValue(wstring(&entry->Section[0], &entry->Section[strlen(entry->Section)]));
		wchar_t buffer[64];
		swprintf(buffer, 64,L"0x%X", entry->Value);
		thisEntry[L"Value"] = new JSONValue(buffer);
		thisEntry[L"isPointer2Pointer"] = new JSONValue((int)entry->isPointer2Pointer);
		thisEntry[L"MemberCount"] = new JSONValue((int)entry->MemberCount);
		JSONArray data;
		//If this is an array the gets inited upon reading the file
		if (entry->initedVal)
		{
			for (int dataCounter = 0; dataCounter < entry->MemberCount; dataCounter++)
			{
				data.push_back(new JSONValue((int)entry->DataArray[dataCounter]));
			}
		}
		thisEntry[L"DataArray"] = new JSONValue(data);

		arr.push_back(new JSONValue(thisEntry));
	}
	root[L"Data"] = new JSONValue(arr);


	FILE* fp = fopen(GameConfiguration::FileName, "w+");
	if (fp)
	{
		wstring data = JSON::Stringify(root[L"Data"]);
		string strValue;
		strValue.assign(data.begin(), data.end());  // convert wstring to string
		fputs(strValue.c_str(), fp);
		fclose(fp);
	}
}

GameConfiguration::GameConfiguration(int romSwitch)
{
	RomSwitch = romSwitch;
	
	/*if (romSwitch != -1)
	{
		Load();
	}
	else
	{*/
		DefaultLoad(romSwitch);
	/*}*/
	return;
}

GameConfiguration::~GameConfiguration()
{
	if (RomSwitch != -1)
	{
		Save();
	}
}


DataContainer* GameConfiguration::GetDataContainer(char *ContainerName)
{
	int i = 0;
	for (i = 0; i < this->Containers.size(); i++) {

		if (Containers[i]->RomSwitch != this->RomSwitch)
		{
			continue;
		}
		if (Containers[i] == NULL)
		{
			continue;
		}
		if (LiteralCompare(ContainerName, Containers[i]->Name) == true) {
			return Containers[i];
		}
	}
	return NULL;
}


int GameConfiguration::SetDataContainer(DataContainer *ContainerName)
{

	return NULL;
}