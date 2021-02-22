#pragma once
#define DIFF_CODE_HEADER        (0x80)
#define RL_CODE_HEADER          (0x30)
#pragma once
#define LZ_CODE_HEADER          (0x10)
#define HUFF_CODE_HEADER        (0x20)
#define CODE_HEADER_MASK        (0xF0)
#define s32 signed long
#define s16 signed short
#define s8 signed char
#include "cprs.h"
#include "nMapBuffer.h"
#include "tinyfiledialogs.h"
struct COLOR2
{
	unsigned char      a;
	unsigned char      r;
	unsigned char      g;
	unsigned char      b;
};
class GBAMethods
{
public: 
	static FILE* ROM;
			  GBAMethods();
			 ~GBAMethods();
	    void  Reopen();
		void DecodeSNESPal(long offset, COLOR2* dstPal, int numPals, char palpos, int size, vector<unsigned char>* buffer);
		signed long   LZ77UnComp(unsigned char *source, unsigned char *dest);
	    unsigned long   LZ77Comp(unsigned long decmpsize,  unsigned char *source, int srcBuffSize, unsigned char *dest) ;
		long  UncompRle(unsigned short arg0, unsigned char *arg1, unsigned char*arg2, long* compsize);
        void* DecodePal(short *palGBA,long* palPC,int numpals, char palpos);

		int   ReturnFileName(char* Filter,char*FilePath,int sofn);
		 void* EncodePal(short *palGBA,long* palPC,int numpals, char palpos);
		
		long  compress(unsigned char cmp[], unsigned char uncmp[], unsigned long sze);
		
		long  FindFreeSpace(long bNo, unsigned char bConst);
		static int ReturnFileName(HINSTANCE inst, HWND wnd,char* Filter,char*FilePath,int sofn,unsigned char Existance);
		static int ReturnEOF(FILE* fp);

		static FILE* REDIT;
		
		static short GBAPal[1024];

		static  char FileLoc[512];


};