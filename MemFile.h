#pragma once
#include <stdio.h>
#include <iostream>
class MemFile
{
public:

	static MemFile* currentFile;
 
	MemFile(FILE* fp);
	MemFile(int size);
	MemFile(char* file, int expectedMinSize);
	int FileSize();
	void Save();
	void Save(char* file);
	void seek(unsigned long offset);
	void fread(void* dst, int count, int size);	
	unsigned char fgetc();
	unsigned char fputc(unsigned char val);
	void fwrite(void* dst, int count, int size);
	bool ValidFileSize();
	unsigned char* GetFile();
	int ftell();
	~MemFile();
private: 
	unsigned char* _theFile; 
	long fileIndex; 
	char* fileName;
	FILE* inuseFile;
	int _expectedFileSize;
	int _fileSize;
};

