#pragma once
#include <vector>
#include "Logger.h"
//The purpose of this is to add a leak whenever you make a new or create, then remove the entry when you delete
using namespace std;
class Leak
{
public:
	char* LeakName;
	Leak(char*, void * addr);
	~Leak();
	void* Address;
};
class LeakFinder
{
	int _removeCount;
	int _addedCount;
public:
	static LeakFinder* finder; 
	vector<Leak*> Leaks;
	LeakFinder();
	~LeakFinder();
	int NumActiveLeaks();

	void RemoveLeak(void * addr);
	void AddLeak(char* leakName, void * addr);
	void RemoveLeak(char* leakName, void * addr);
	int GetLeakIndex(void* addr);
	void LogActiveLeaks(Logger* instance);
};

