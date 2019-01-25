#include "MemFile.h"


MemFile* MemFile::currentFile;
MemFile::MemFile(int size)
{

}
unsigned char* MemFile::GetFile()
{
	return theFile;
}
MemFile::MemFile(char* file)
{
	FILE* FP = fopen(file, "r+b");
	fileName = file;
	fseek(FP, 0, SEEK_END);
	int len = ftell(FP);
	size = len;
	fseek(FP, 0,SEEK_SET);
	theFile = new unsigned char[len];
	memset(theFile, 0, len);
	::fread(theFile, 1, len, FP);
	fclose(FP);
	FP = NULL;
}


void MemFile::Save()
{
	Save(fileName);
}
void MemFile::Save(char* file)
{
	FILE* FP = ::fopen(file, "w+b");
	::fseek(FP, 0, SEEK_SET);
	::fwrite(theFile, size, 1,FP);
	::fclose(FP);
}
unsigned char MemFile::fgetc()
{
	return theFile[fileIndex++];
}
void MemFile::seek(unsigned long offset)
{
	if (offset >= 32000000)
	{
		char buffer[1000] = { 0 };
		sprintf(buffer, "attempted to seek out of bounds, %d (0x%X)", offset, offset);
		throw buffer;
	}
	fileIndex = offset; 
}
void MemFile::fread(void* dst, int count, int size)
{
	int readSize = count*size;
	if (readSize + fileIndex > this->size)
	{
		readSize = this->size - readSize + fileIndex;
	}
	memcpy(dst, &theFile[fileIndex], readSize);
	fileIndex += readSize;
}

void MemFile::fwrite(void* src, int count, int size)
{
	int readSize = count*size;
	if (readSize > 0x800000)
	{
		char buffer[1000] = { 0 };
		sprintf(buffer, "attempted to read %d size %d many times, write size: %d", src, count, size );
		throw buffer;
	}
	if (readSize + fileIndex > this->size)
	{
		readSize = this->size - readSize + fileIndex;
	}
	memcpy(&theFile[fileIndex],src,  readSize);
	fileIndex += readSize;	
}

unsigned char MemFile::fputc(char val)
{
	theFile[fileIndex++]=val;
	return theFile[fileIndex-1];
}

MemFile::~MemFile()
{
	delete[] theFile;
}
