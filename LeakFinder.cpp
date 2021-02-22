#include "LeakFinder.h"

LeakFinder* LeakFinder::finder;

Leak::Leak(char* name, void * addr, bool r)
{
	IsArray = r;
	LeakName= name;
	Address = addr;
}

Leak::~Leak()
{
	
}

LeakFinder::LeakFinder()
{	
	_addedCount = 0;
	_removeCount = 0;
	_processHandle = GetProcessHeap();
}

LeakFinder::~LeakFinder()
{
}

void* LeakFinder::New(int size, bool isarray)
{
	void* addr = new unsigned char[size];
	AddLeak("SelfAdded",  addr, isarray);
	return addr;
}

void LeakFinder::Delete(void* addr)
{

	int add = GetLeakIndex(addr);
	delete[] addr;

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

void LeakFinder::AddLeak(char* name, void * addr, bool r)
{
	Leaks.push_back(new Leak(name, addr, r));
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

	/*char* data = new char[2048];
	memset(data, 0, 2048);
	sprintf(data, "Showing active addreses\nPossible leaks %d\n", NumActiveLeaks());
	instance->LogIt(Logger::LOGTYPE::DEBUG, data);
	for each(auto l in Leaks)
	{
		sprintf(data, "\tLeak Name: %s, Address: %X\n",l->LeakName.c_str(), (unsigned int)l->Address);
		instance->LogIt(Logger::LOGTYPE::DEBUG, data);
	}
	sprintf(data, "End active leaks\n"); 
	instance->LogIt(Logger::LOGTYPE::DEBUG, data);
	delete[] data;*/
}

bool LeakFinder::PollHeap() 
{
	if (HeapValidate(_processHandle, 0, NULL)==0)
	{
		throw "HEAP gone my guy, debug it";
	}
	return true;
}