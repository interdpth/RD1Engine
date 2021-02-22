#pragma once
#include <vector>
#include <string>
#include "Logger.h"
//The purpose of this is to add a leak whenever you make a new or create, then remove the entry when you delete
using namespace std;
class Leak
{
public:
	string LeakName;
	bool IsArray;
	Leak(char*, void * addr, bool r = true);
	~Leak();
	void* Address;
	
};
class LeakFinder
{
private:
	int _removeCount;
	int _addedCount;
	HANDLE _processHandle;
public:
	static LeakFinder* finder; 
	vector<Leak*> Leaks;
	LeakFinder();
	~LeakFinder();
	int NumActiveLeaks();
	void* New(int size, bool isarray=true);
	void Delete(void* address);
	void RemoveLeak(void * addr);
	void AddLeak(char* leakName, void * addr, bool r= true);
	void RemoveLeak(char* leakName, void * addr);
	int GetLeakIndex(void* addr);
	void LogActiveLeaks(Logger* instance);
	bool PollHeap();
};

