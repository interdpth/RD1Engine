#include "LeakFinder.h"

LeakFinder* LeakFinder::finder;

Leak::Leak(char* name, void * addr)
{
	LeakName = new char[strlen(name) + 1];
	memset(LeakName, 0, strlen(name) + 1);
	sprintf(LeakName, "%s", name);
	Address = addr;
}

Leak::~Leak()
{
	if (LeakName)
	{
		delete[] LeakName;
	}
}

LeakFinder::LeakFinder()
{
}

LeakFinder::~LeakFinder()
{
}


int LeakFinder::GetLeakIndex(void* addr)
{	
	for (int returnValue = 0; returnValue < Leaks.size(); returnValue++)
	{
		if (Leaks[returnValue]->Address == addr)
		{
			return returnValue;
		}
	}
	return -1;
}

void LeakFinder::AddLeak(char* name, void * addr)
{
	Leaks.push_back(new Leak(name, addr));
	_addedCount++;
}

void LeakFinder::RemoveLeak(void * addr)
{
	int index = GetLeakIndex(addr);
	if (index != -1)
	{
		Leak* del = Leaks.at(index);
		delete (del);
		Leaks.erase(Leaks.begin() + index);
		_removeCount++;
	}
}

void LeakFinder::RemoveLeak(char* name, void * addr)
{
	int index = GetLeakIndex(addr);
	if (index != -1)
	{
		Leak* del = Leaks.at(index);
		delete del;
		Leaks.erase(Leaks.begin() + index);
		_removeCount++;
	}
}

int LeakFinder::NumActiveLeaks()
{
	return _addedCount - _removeCount;
}

void LeakFinder::LogActiveLeaks(Logger* instance)
{

	//char* data = new char[2048];
	//memset(data, 0, 2048);
	//sprintf(data, "Showing active addreses\nPossible leaks %d\n", NumActiveLeaks());
	//instance->LogIt(Logger::LOGTYPE::DEBUG, data);
	//for each(Leak* l in Leaks)
	//{
	//	sprintf(data, "\tLeak Name: %s, Address: %X\n",l->LeakName, l->Address);
	//	instance->LogIt(Logger::LOGTYPE::DEBUG, data);
	//}
	//sprintf(data, "End active leaks\n", data); 
	//instance->LogIt(Logger::LOGTYPE::DEBUG, data);
	//delete[] data;
}