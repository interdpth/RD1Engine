#include "MemFile.h"

//Static object 
MemFile* MemFile::currentFile;


MemFile::MemFile(int size)
{

}
unsigned char* MemFile::GetFile()
{
	return _theFile;
}
int MemFile::FileSize()
{
	return _fileSize;

}
bool MemFile::ValidFileSize()
{
	return _expectedFileSize <= _fileSize; 
}


MemFile::MemFile(char* file, int expectedMinSize)
{
	_expectedFileSize = expectedMinSize;
	
	FILE* FP = fopen(file, "r+b");
	fileName = file;
	//get file size
	fseek(FP, 0, SEEK_END);
	_fileSize = ftell(FP);


	//read full size
	fseek(FP, 0,SEEK_SET);
	_theFile = new unsigned char[_fileSize];
	memset(_theFile, 0, _fileSize);
	::fread(_theFile, 1, _fileSize , FP);
	fclose(FP);
	FP = NULL;
}


void MemFile::Save()
{
	Save(fileName);
}
void MemFile::Save(char* file)
{
	//std functions
	FILE* FP = ::fopen(file, "w+b");
	::fseek(FP, 0, SEEK_SET);
	::fwrite(_theFile, _fileSize, 1,FP);
	::fclose(FP);
}
unsigned char MemFile::fgetc()
{
	return _theFile[fileIndex++];
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
	if (readSize + fileIndex > _fileSize)
	{
		readSize = this->_fileSize - readSize + fileIndex;
	}
	memcpy(dst, &_theFile[fileIndex], readSize);
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
	if (readSize + fileIndex > this->_fileSize)
	{
		readSize = this->_fileSize - readSize + fileIndex;
	}
	memcpy(&_theFile[fileIndex],src,  readSize);
	fileIndex += readSize;	
}

unsigned char MemFile::fputc(char val)
{
	_theFile[fileIndex++]=val;
	return _theFile[fileIndex-1];
}

MemFile::~MemFile()
{
	delete[] _theFile;
}
