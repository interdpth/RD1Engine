#pragma once
#include <string>
#include <vector>
using namespace std;
class AsmII
{
public:
	AsmII(unsigned char* data);
	unsigned char ReadMove(unsigned long offset);
	unsigned short ReadBigMove(unsigned long offset);
	bool SetMove(unsigned long offset, unsigned char val);
	bool SetBigMove(unsigned short val, unsigned long offset);
	vector<int> ReadLSL(unsigned long offset);
	bool SetLSL(int shift, int val, unsigned long offset);
private:

	string GetASM(unsigned long offset);
	bool SetASM(string asmcmd);
	unsigned char* _dat;
	unsigned long  _registers[16];
};

