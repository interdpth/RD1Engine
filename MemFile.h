#pragma once
#include <stdio.h>
#include <iostream>
class MemFile
{
public:

	static MemFile* currentFile;
	
	MemFile(FILE* fp);
	MemFile(int size);
	MemFile(char* file);
	void Save();
	void Save(char* file);
	void seek(unsigned long offset);
	void fread(void* dst, int count, int size);	
	unsigned char fgetc();
	unsigned char fputc(char val);
	void fwrite(void* dst, int count, int size);
	int size;
	unsigned char* GetFile();
	~MemFile();
private: 
	unsigned char* theFile; 
	long fileIndex; 
	char* fileName;
	FILE* inuseFile;
};

