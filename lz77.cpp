#include "MainHeader.h"


lz77::lz77(void)
{
}

lz77::~lz77(void)
{
}

		


unsigned long lz77::CPUReadMemory(int source)
{
	return cFile[source]+cFile[source+1]*256+cFile[source+2]*(256*256)+cFile[source+3]*(256*256*256);
//	return 0;
}

void lz77::CPUWriteMemory(int adress, unsigned long data)
{
	  cFile[adress]=(unsigned char)(data & 0x000000FF);
	cFile[adress+1]=(unsigned char)(data & 0x0000FF00)>> 8;
	cFile[adress+2]=(unsigned char)(data & 0x00FF0000)>> 16;
	cFile[adress+3]=(unsigned char)(data & 0xFF000000)>> 24;


}

unsigned char lz77::CPUReadByte(int source)
{
	return cFile[source];
}


void lz77::CPUWriteByte(unsigned long a, unsigned char b)
{
	cFile[a]=b;

}










void lz77::BIOS_LZ77UnCompWram()
{


  source=0;
  
  unsigned long dest = 170000/2;

  unsigned long header = CPUReadMemory(source);
  source += 4;

  // this is just to check if everything is alright 
 /* if(((source & 0xe000000) == 0) ||
     ((source + ((header >> 8) & 0x1fffff)) & 0xe000000) == 0)
    return;  */
  
  int len = header >> 8;
	length = len;
	
	

if(length>170000/2)
	return;

  while(len > 0) {
    unsigned char d = CPUReadByte(source++);

    if(d) {
      for(int i = 0; i < 8; i++) {
        if(d & 0x80) {
			// note this is different in the vba source but it did not work for me before
       		unsigned short data = (CPUReadByte(source)<<8);
			source++;
			data = data | CPUReadByte(source++);

		
          int length = (data >> 12) + 3;
          int offset = (data & 0x0FFF);
          unsigned long windowOffset = dest - offset - 1;
          for(int i = 0; i < length; i++) {
            CPUWriteByte(dest++, CPUReadByte(windowOffset++));
            len--;
            if(len == 0)
              return  ;
          }
        } else {
          CPUWriteByte(dest++, CPUReadByte(source++));
          len--;
          if(len == 0)
            return ;
        }
        d <<= 1;
      }
    } else {
      for(int i = 0; i < 8; i++) {
        CPUWriteByte(dest++, CPUReadByte(source++));
        len--;
        if(len == 0)
          return ;
      }
    }
  }
}

void lz77::BIOS_LZ77CompWram(unsigned char data[], int size, int compression)
{

int i=0;
/*	CFile file("0x36f250.gba", CFile::modeRead);
	file.Read(data, 8192);
	file.Close();
	size = 8192;*/
	
  // : array [0..10000] of byte;

	for( i=0; i<10000; i++)
		filedata[i]=0;

  int l; //byte
  //data : array [1..17000] of byte;
  unsigned long cmpdata[16]; // : array [2..18] of word;
  for( i=0; i<16; i++)
	cmpdata[i]=0;


  int slidingwindowsize; //: integer; //word
 // size : integer; //word
  int position;// : integer; //word
  int buffersize;// : integer; //byte
  int testpos;// : integer; //word
  int testmatch;// : integer; //byte
  int count;// : integer; //byte
  int match;// : integer; //byte
  int slidingwindowmov;// : integer; //word
  int data1; // : integer; //word
  int data2; // : integer; //word
  int tmpcmpbyte; // : integer; //byte
 // int filedatapos; // : integer; //word
  // : string;
  int fin; // : integer; //byte
//  int delta, starttime;// : integer;
  int flagbyte;// : integer; //byte

  fin = 0;

  position = 0;
  testpos = 1;
  filedatapos = 0;
  tmpcmpbyte = -1;
  flagbyte = 0;

   // write header
   filedata [0] = 0x10;
   filedata [1] = (size & 0xff);
   filedata [2] = ((size & 0xff00) >> 8);
   filedata [3] = ((size & 0xff0000) >> 16);
   filedatapos = 4;

   //TRACE("OKAY IT HAS STARTED");

   for(i=0; i<=compression; i++)
   {
	tmpcmpbyte++;
	cmpdata[2*tmpcmpbyte] = data[i];
	position++;
   }

	slidingwindowsize = position-1;
	slidingwindowmov = compression;

	while (position < size) 
	{
	//	position++;
		
		count = 0;
		if (size-position>18) 
			buffersize = 18;
		else
			buffersize = size-position;

		testmatch = 2;

		do 
		{
			match = 0;
			l=0;

			while(data[position-slidingwindowmov-1] != data[position] )
			{
				slidingwindowmov++;
				if (slidingwindowmov>slidingwindowsize-1)
					break;			
			}

			for(int j = 1; j<=buffersize; j++)
			{
				
				if(data[position+l-slidingwindowmov-1] == data[position+j-1] )
				{
					match++;

				}
				else
				{
				//	match--;
					break;

				}
				l++;
				if(l-slidingwindowmov == 1)
					l = 0;


			}



			if (match > testmatch)
			{
				testmatch = match;
				testpos = slidingwindowmov;
				// If it is the very last data to compress, set a flag to end the compression

			if (testmatch+position+1 > size) 
				fin = tmpcmpbyte+2;

			}
			if (position+2 > size) 
				fin = tmpcmpbyte+2;

			slidingwindowmov++;
	}
		while(!((slidingwindowmov>slidingwindowsize-1) || (testmatch == buffersize))); 

		tmpcmpbyte++;

		if (testmatch <3)
		{
			flagbyte = (flagbyte << 1);
			cmpdata[2*tmpcmpbyte] = data[position]; // copy a raw byte

			position++;
			slidingwindowsize++;
		}
		else // data can be compreGlobalVars::gblVars->SSEd
		{
			flagbyte = (flagbyte << 1)+1;
			cmpdata[2*tmpcmpbyte] = testmatch-3; //number of byte to copy -3
			cmpdata[2*tmpcmpbyte+1] = testpos;
			position+=testmatch;
			slidingwindowsize+=testmatch;
		}

		if (slidingwindowsize > 0xfff)
			 slidingwindowsize = 0xfff;

		slidingwindowmov = compression;


		if ((tmpcmpbyte == 7) || (fin > 0))
		{
			if (fin <= 0) 
				count=7;
			else
				count=fin-1;

			filedata[filedatapos] = flagbyte;

			

			if (fin >0)
				filedata[filedatapos] = filedata[filedatapos] << (8-fin);
            filedatapos++;


			for (int i = 0; i<=count; i++)
			{

				if ((flagbyte & (1 << (count-i))) !=0)
				{
             //   if (cmpdata[2*i+1] > $fff) then showmessage ('error! '+inttohex(filedatapos,8));
                data1 = (cmpdata[2*i+1] & 0xff);
                data1 = data1 << 4;
                data1 =  data1 + cmpdata[2*i];
                data1 = data1 << 4;
                data2 = (cmpdata[2*i+1] & 0xf00);
                data2 = data2 >> 8;
                data1 = data1+data2;
                filedata[filedatapos+1] = (data1 & 0xff00) >> 8;
                filedata[filedatapos] = (data1 & 0xff);
				
                filedatapos+=2;

				}
				else
				{
					filedata[filedatapos] = (unsigned char)cmpdata[2*i];
                    filedatapos++;


				}

			}
			tmpcmpbyte = -1;


		}
	
		
	}
	filedata[filedatapos] = 0;


	if (((filedatapos) & 3) !=0)
	{
		for (int i = 1; i<=(4-(filedatapos & 3)); i++)
			filedata[filedatapos+i] = 0;
		filedatapos = filedatapos + (4-(filedatapos & 3));

    }



}
