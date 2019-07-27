#include "GBAMethods.h"
#include "MapManager.h"
#include "structures.h"
GBAMethods::GBAMethods()
{

}

GBAMethods::~GBAMethods()
{

}

short GBAMethods::GBAPal[1024];

char GBAMethods::FileLoc[MAX_PATH];
FILE* GBAMethods::ROM;
 FILE* GBAMethods::REDIT;
//==================================================================================
// Expand difference compression data
// ==================================================================================

//==================================================================================
// Expanding run length compreGlobalVars::gblVars->SSEd data
// ==================================================================================

//==================================================================================
// LZ77CompreGlobalVars::gblVars->SSEd data extraction
// ==================================================================================
s32             GBAMethods::LZ77UnComp(unsigned char * source, unsigned char * dest)
{
   long            size = 0;

   unsigned long             header = (source[0] | (source[1] << 8) | (source[2] << 16) | (source[3] << 24));

   // printf(Buffer,"%d",header>> 8);

   // sMessage(Buffer);
   int             i, j, in = 4, out = 0;
   int             length, offset;
   unsigned long             windowOffset;

   int             len = header >> 8;

   while (len > 0)
   {
      unsigned char              d = source[in++];

      for (i = 0; i < 8; i++)
      {
         if (d & 0x80)
         {
            unsigned short             data = ((source[in] << 8) | source[in + 1]);

            in += 2;
            length = (data >> 12) + 3;
            offset = (data & 0x0FFF);
            windowOffset = out - offset - 1;
            for (j = 0; j < length; j++)
            {
               dest[out++] = dest[windowOffset++];
               len--;
               if (len == 0)
                  return header >> 8;
            }
         } else
         {
            dest[out++] = source[in++];
            len--;
            if (len == 0)
               return header >> 8;
         }
         d <<= 1;
      }
   }
   return header >> 8;
/*
 * lz77 GL;
 * 
 * 
 * GL.source = 0;
 * memcpy(&GL.cFile, &source,32691);
 * int i=0;
 * 
 * for(i=0;i<32691;i++) GL.cFile[i]=source[i];
 * GL.BIOS_LZ77UnCompWram();
 * 
 * 
 * for(i=0;i<GL.length;i++)dest[i]=GL.cFile[i];
 * return GL.length;
 */
}


//==================================================================================
// Palette
// ==================================================================================
void           *GBAMethods::DecodePal(short *palGBA, long *palPC, int numpals, char palpos)
{

   int             R, g, b, colorCounter, palCounter, s = 0;
   long            index = 0;

   for (palCounter = 0; palCounter < numpals; palCounter++)
   {
      index = 0x10 * palCounter;

      for (colorCounter = 0; colorCounter < 16; colorCounter++)
      {

         R = ((palGBA[index + colorCounter] & 0x1F) << 3);
         g = ((palGBA[index + colorCounter] >> 5) & 0x1F) << 3;
         b = (((palGBA[index + colorCounter] >> 10) & 0x1f) << 3);
         palPC[index + colorCounter] = RGBA(R, g, b, 255);
      }
   }
   return 0;
}
void           *GBAMethods::EncodePal(short *palGBA, long *palPC, int numpals, char palpos)
{

   int             i, ii, s = 0;

   // unsigned short clr;
   unsigned long             clr = 0;;
   long            index = 0;

   for (ii = 0; ii < numpals; ii++)
   {
      index = 0x10 * ii;

      for (i = 0; i < 16; i++)
      {
         clr = palPC[index + i] >> 3;
         palGBA[index + i] = (unsigned short) ((clr & 0x1F) | (clr & 0x1F00) >> 3 | (clr & 0x1F0000) >> 6);

         // clr = (palPC[index + i]>>3);
         palGBA[index + i] = (unsigned short) (((palPC[index + i] >> 3) & 0x1F) | ((palPC[index + i] >> 3) & 0x1F00) >> 3 | ((palPC[index + i] >> 3) & 0x1F0000) >> 6);
      }
   }
   return 0;
}
/*
 * long GBAMethods::ZMUncompRle(unsigned char* cmp, unsigned char* uncmp, long size, int LoopSeq ){
 * char nm=0;
 * char buf[100];
 * long src , tgt, sze, i,j;
 * 
 * unsigned char rlebyte, data,Mode;
 * 
 * src = 0;
 * 
 * tgt = 0;
 * 
 * 
 * for( j = 0; j <  2;j++){
 * // sprintf(buf,"compreGlobalVars::gblVars->SSEd offset is: %X",src);
 * // sMessage(buf);
 * Mode = cmp[src++];
 * 
 * if (Mode == 1){
 * rlebyte = cmp[src++];
 * 
 * while (rlebyte > 0){
 * if(tgt == size) return  src - 1;
 * sze = rlebyte & 0x7F;
 * if (rlebyte & 0x80){
 * data = cmp[src++];
 * 
 * for(i = -1;i<sze;i++){
 * //if(tgt == size*2) return  src - 1;
 * uncmp[tgt] = data;
 * tgt = tgt + (LoopSeq == 1? 2:1);
 * }
 * }else{
 * for(i = 0;i<sze;i++){
 * //if(tgt == size*2) return  src - 1;
 * data = cmp[src++];
 * uncmp[tgt] = data;
 * tgt = tgt + (LoopSeq == 1? 2:1);
 * }
 * }
 * rlebyte = cmp[src++];
 * 
 * }
 * }
 * 
 * 
 * if (LoopSeq == 1) 
 * if (j == 0)  tgt =1;
 * 
 * }
 * 
 * 
 * 
 * return src - 1;
 * }
 */
long            GBAMethods::UncompRle(unsigned short BufferSizeSwitch, unsigned char * arg1, unsigned char * arg2, long* compsize)
{

   unsigned char             *src = (unsigned char *) arg1, *dst = (unsigned char *) arg2;

   unsigned long             jj, ii, nn;
   unsigned long             ctrl, type;

   type = 0;
   nn = 0x3000;

   switch (BufferSizeSwitch)
   {
   case 0:
      type = 0x0800;
      break;
   case 3:
      type = 0x2000;
      break;
   default:
      type = 0x1000;
   }

   nn = 0x2000;

   // DMAFill(3, 0, arg2, nn, 16);

   for (ii = 0; ii < 2; ii++)
   {
      dst = (unsigned char *) & arg2[ii];
      if (*src++ == 1) // 8 control field

      {
         while (1)
         {
            ctrl = *src++;
            if (ctrl == 0)
               break;

            else if (ctrl & 0x80)
            {
               nn = ctrl & 0x7F;
               if (*src)
                  for (jj = 0; jj < nn; jj++)
                     dst[2 * jj] = *src;
               src++;
            } else
            {
               nn = ctrl;
               for (jj = 0; jj < nn; jj++)
                  dst[2 * jj] = src[jj];
               src += nn;
            }

            dst += 2 * nn;
         }
      } else // RLE16

      {
         while (1)
         {
            ctrl = src[0] << 8 | src[1];
            src += 2;
            if (ctrl == 0)
               break;

            if (ctrl & 0x8000)
            {
               nn = ctrl & 0x7FFF;
               if (*src)
                  for (jj = 0; jj < nn; jj++)
                     dst[2 * jj] = *src;
               src++;
            } else
            {
               nn = ctrl;
               for (jj = 0; jj < nn; jj++)
                  dst[2 * jj] = src[jj];
               src += nn;
            }
            dst += 2 * nn;
         }
      }
   }
   *compsize = src - arg1;
   return type;
}


long            GBAMethods::compress(unsigned char cmp[], unsigned char uncmp[], unsigned long sze)
{
   // int sze = 3;

   unsigned long             cmpptr = 0;
   unsigned long             sizeptr = 0;
   unsigned long             dataptr = 0;
   unsigned long             rl = 0;

   unsigned long             samebytecount = 0;

   for (int j = 0; j < 2; j++)
   {
      cmp[cmpptr++] = 1;
      while (sze > dataptr)
      {
         rl = 2;
         while (uncmp[dataptr] == uncmp[dataptr + 2] && uncmp[dataptr + 2] == uncmp[dataptr + 4] && rl < 0x7F && sze > dataptr)
         {

            dataptr += 2;
            rl++;
         }

         if (rl > 2)
         {
            // printf("run found s=%02X\n", rl);
            cmp[cmpptr++] = (unsigned char) ((rl | 0x80) & 0xFF);
            cmp[cmpptr++] = uncmp[dataptr];
            dataptr += 4;
         }
         sizeptr = cmpptr;
         cmpptr++;

         rl = 0;
         while ((uncmp[dataptr] != uncmp[dataptr + 2] || uncmp[dataptr + 2] != uncmp[dataptr + 4]) && rl < 0x7F && sze > dataptr)
         {

            cmp[cmpptr++] = uncmp[dataptr];
            dataptr += 2;
            rl++;
         }
         // printf("norun found s=%02X\n", rl);
         if (rl != 0)
            cmp[sizeptr] = (unsigned char) rl;
         else
            cmpptr--;
      }
      cmp[cmpptr++] = 0;

      dataptr = 1;
   }

   return cmpptr;

}
/*
 * 
 * long GBAMethods::FindFreeSpace(long bNo, unsigned char bConst){//bNo is how many bytes, bConst is what
 * //value to look for 
 * 
 * // returns offset of "free space" block
 * //  or -1 if no such block found
 * 
 * FILE* file = fopen(GBA.FileLoc,"rb");
 * if(!file)
 * return -1;
 * 
 * 
 * //bNo+=0x100;//Look for an extra free 256 bytes.
 * int ch;
 * long runstart = -1;
 * long runcount = 0;
 * int flength=EOF;
 * while(1)
 * {
 * ch = fgetc(file);  // get character (byte)
 * if(ch == EOF)  // if end of file
 * {
 * runstart = -1; // eof reached before we found block
 * return  runstart ;
 * 
 * }
 * 
 * if((unsigned char)ch == bConst)
 * {
 * if(!runcount) // start of run
 * runstart = (long)ftell(file) - 1;
 * 
 * ++runcount;
 * if(runcount >= bNo)  // block found
 * break;
 * } else {
 * 
 * runcount = 0;
 * }
 * 
 * }
 * 
 * fclose(file);
 * 
 * return runstart;
 * }
 */
unsigned short cprs_gba_lz77(RECORD *dst, const RECORD *src);


unsigned long             GBAMethods::LZ77Comp(unsigned long decmpsize, unsigned char * source, int srcBuffSize, unsigned char * dest)
{

	RECORD dst;

	memset(&dst, 0, sizeof(RECORD));
	dst.data = new unsigned char[1];
	RECORD src;
	src.data = source;
	src.height = decmpsize;
	src.width = 1;

	int size = (int)lz77gba_compress(&dst, &src);



	size = size;
	memcpy(dest, dst.data, size);
	free(dst.data);

	return size;
//lz77 GL;
   // GL.BIOS_LZ77CompWram(source,decmpsize,1);
   // memcpy(dest,GL.filedata,GL.filedatapos);
   /*
    * long cmpsize=0; RECORD src= { source, decmpsize, 1 }; RECORD dst= {
    * NULL, 0, 0 }; cmpsize=cprs_gba_lz77(&dst, &src); memcpy(dest,dst.data,
    * cmpsize); free(dst.data); return cmpsize;
    */
   int             unp = 0;
   int             cop = 4;

   int             dist;
   int             maxchain;
   int             ende;
   int             chain;
   int             idpos;
   unsigned short  bits = 0;

   while (unp < decmpsize)
   {
      bits = bits >> 1;
      if (bits == 0)
      {
         dest[cop] = 0;
         idpos = cop++;
         bits = 0x80;
      }
      dist = 0;
      maxchain = 0;

      ende = min(0xFFF, unp);

      for (int i = 1; i < ende; i++)
      {
         chain = 0;
         while (source[unp + chain] == source[unp - i + chain] && chain < 0x12 && unp + chain < decmpsize
                && unp - i + chain != unp - 1)
         {
            chain++;
         }
         if (chain > maxchain)
         {
            dist = i;
            maxchain = chain;
         }
         if (chain == 0x12)
            break;
      }

      if (maxchain > 2)
      {
         dest[idpos] |= bits;
         dest[cop] = (((maxchain - 3) & 0xF) << 4) | (((dist - 1) >> 8) & 0xF);
         dest[cop + 1] = dist - 1 & 0xFF;
         cop += 2;
         unp += maxchain;
      } else
      {
         dest[cop++] = source[unp++];
      }
   }

   while (cop % 4)
   {
      dest[cop++] = 0;
   }

   unsigned long             Header = 0x10 | (decmpsize << 8);

   memcpy(dest, &Header, 4);
   return cop;

   // return GL.filedatapos;
}

void            GBAMethods::Reopen()
{

   fclose(ROM);
   ROM = fopen(FileLoc, "r+b");

}
//
//bool    GBAMethods :: AllocateSpace(int offset, int newLen)
//{
//	int num = this.FindEndOfData();
//	for (int i = 655360; i < num; i += 4)
//	{
//		if (this.data[i] == 109 && this.data[i + 1] == 97 && this.data[i + 2] == 103 && this.data[i + 3] == 101)
//		{
//			int num2 = (int)this.Read16(i + 4);
//			if (num2 >= newLen)
//			{
//				bool flag = true;
//				for (int j = 6; j < num2; j++)
//				{
//					if (this.data[i + j] != 255)
//					{
//						flag = false;
//						break;
//					}
//				}
//				if (flag)
//				{
//					int num3 = i + newLen;
//					while (num3 % 4 != 0)
//					{
//						num3++;
//					}
//					int num4 = i + num2 - num3;
//					if (num4 >= 6)
//					{
//						this.Seek(i + newLen);
//						while (this.pos % 4 != 0)
//						{
//							this.data[this.pos++] = 0;
//						}
//						this.WriteASCII("mage");
//						this.Write16((unsigned short)num4);
//					}
//					offset = i;
//					return false;
//				}
//			}
//		}
//	}
//	if (!Version.IsMF && num < Version.MetroidOffset && num + newLen > Version.MetroidOffset)
//	{
//		num = this.Capacity;
//		this.FindEndOfRun(ref num);
//	}
//	if (num + newLen > this.Capacity && this.Capacity < 33554432)
//	{
//		this.Capacity *= 2;
//		this.length = this.Capacity;
//		for (int k = this.length / 2; k < this.length; k++)
//		{
//			this.data[k] = 255;
//		}
//	}
//	offset = num;
//	return true;
//}


//New find free space
long            GBAMethods::FindFreeSpace(long searchSize,  unsigned char bConst) {

	
	long sz = MemFile::currentFile->FileSize();

	unsigned char* rawRom = MemFile::currentFile->GetFile();
	searchSize += 0x100;//Attempts to make sure we're not over writing stuff. 
	byte free = bConst;
	unsigned long startingLocation = 0x400000;
	int fileIndex = 0;
	unsigned char* searching = new unsigned char[(int)searchSize];
	for (fileIndex = 0;  fileIndex < searchSize;  fileIndex++)
	{
		searching[fileIndex] = 0xFF;
	}
	int numMatches = 0;
	int freespace = -1;
	for (fileIndex = startingLocation; fileIndex < sz; fileIndex++)
	{
		byte b = rawRom[fileIndex];
		byte c = searching[numMatches];
		if (b == c)
		{
			numMatches++;
			if (numMatches == searchSize - 1)
			{
				freespace = fileIndex+2	;
				break;
			}
		}
		else
		{
			numMatches = 0;
		}
	}
	
	delete[] searching;
	return freespace;


}



//long            GBAMethods::_oldFindFreeSpace(long bNo, unsigned char bConst)
//{ // bNo is how many bytes, bConst is what
//   // value to look for 
//
//   // returns offset of "free space" block
//   // or -1 if no such block found
//
//   FILE           *file = fopen(GBA.FileLoc, "rb");
//
//   if (!file)
//      return -1;
//
//   bNo += 0x100; // Look for an extra free 256 bytes. -- still not sure what
//                 // this does,
//   // but whatever
//
//   int             ch;
//   long            runstart = -1;
//   long            runcount = 0;
//
//   MemFile::currentFile->seek(file, DefAddress);
// checkagain:
//   while (1)
//   {
//      ch = fgetc(file); // get character (byte)
//
//      if (ch == EOF) // if end of file
//
//      {
//         runstart = -1; // eof reached before we found block
//
//         break;
//
//      }
//      if ((unsigned char) ch == bConst)
//      {
//         if (!runcount) // start of run
//
//            runstart = (long) ftell(file) - 1;
//
//         ++runcount;
//         if (runcount >= bNo) // block found
//
//            break;
//      } else
//      {
//         runstart = -1;
//         runcount = 0;
//      }
//
//   }
//   if (DefAddress > 0 && runstart == -1)
//   {
//      MemFile::currentFile->seek(file, 0);
//
//      goto checkagain;
//   }
//   if (runstart != -1)
//      while (runstart % 4)
//         runstart++;
//   return runstart;
//}



int             GBAMethods::ReturnFileName(char *Filter, char *FilePath, int sofn)
{
   return ReturnFileName(NULL, NULL, Filter, FilePath, sofn, 1);

}

/*
HRESULT BasicFileOpen()
{
	// CoCreate the File Open Dialog object.
	IFileDialog *pfd = NULL;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pfd));
	if (SUCCEEDED(hr))
	{
		// Create an event handling object, and hook it up to the dialog.
		IFileDialogEvents *pfde = NULL;
		hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
		if (SUCCEEDED(hr))
		{
			// Hook up the event handler.
			DWORD dwCookie;
			hr = pfd->Advise(pfde, &dwCookie);
			if (SUCCEEDED(hr))
			{
				// Set the options on the dialog.
				DWORD dwFlags;

				// Before setting, always get the options first in order 
				// not to override existing options.
				hr = pfd->GetOptions(&dwFlags);
				if (SUCCEEDED(hr))
				{
					// In this case, get shell items only for file system items.
					hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
					if (SUCCEEDED(hr))
					{
						// Set the file types to display only. 
						// Notice that this is a 1-based array.
						hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
						if (SUCCEEDED(hr))
						{
							// Set the selected file type index to Word Docs for this example.
							hr = pfd->SetFileTypeIndex(INDEX_WORDDOC);
							if (SUCCEEDED(hr))
							{
								// Set the default extension to be ".doc" file.
								hr = pfd->SetDefaultExtension(L"doc;docx");
								if (SUCCEEDED(hr))
								{
									// Show the dialog
									hr = pfd->Show(NULL);
									if (SUCCEEDED(hr))
									{
										// Obtain the result once the user clicks 
										// the 'Open' button.
										// The result is an IShellItem object.
										IShellItem *psiResult;
										hr = pfd->GetResult(&psiResult);
										if (SUCCEEDED(hr))
										{
											// We are just going to print out the 
											// name of the file for sample sake.
											PWSTR pszFilePath = NULL;
											hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
												&pszFilePath);
											if (SUCCEEDED(hr))
											{
												TaskDialog(NULL,
													NULL,
													L"CommonFileDialogApp",
													pszFilePath,
													NULL,
													TDCBF_OK_BUTTON,
													TD_INFORMATION_ICON,
													NULL);
												CoTaskMemFree(pszFilePath);
											}
											psiResult->Release();
										}
									}
								}
							}
						}
					}
				}
				// Unhook the event handler.
				pfd->Unadvise(dwCookie);
			}
			pfde->Release();
		}
		pfd->Release();
	}
	return hr;
}
*/
int             GBAMethods::ReturnFileName(HINSTANCE inst, HWND wnd, char *Filter, char *FilePath, int sofn, unsigned char Existance)
{
   int             Flag = 0;

   if (Existance == 1)
      Flag = OFN_FILEMUSTEXIST;
   Flag |= OFN_EXPLORER;
   OPENFILENAME    ofn;

   memset(&ofn, 0, sizeof(ofn));
   ofn.lStructSize = sizeof(OPENFILENAME);
   ofn.lpstrInitialDir = ".";
   ofn.hwndOwner = wnd;
   ofn.lpstrFile = FilePath;
   ofn.nMaxFile = sofn;
   ofn.hInstance = inst;
   ofn.lpstrFilter = Filter;
   ofn.nFilterIndex = 1;
   ofn.Flags = OFN_EXPLORER | Flag;
   int returnVal = GetOpenFileName(&ofn);
  /* char const * lFilterPatterns[1] = { "*.gba" };
   char const * fn = tinyfd_openFileDialog(
	   "Open Rom",
	   "",
	   1,
	   lFilterPatterns,
	   NULL,
	   0);*/
  // sprintf(FilePath, "%s", ofn);
	return  1;
}

int             GBAMethods::ReturnEOF(FILE * fp)
{
   // Takes a file, and finds the end of it
   int             sz = 0;

   // MemFile::currentFile->seek(0,SEEK_END);
   char            blah;

   MemFile::currentFile->seek( 0);
   while (1)
   {
      blah = fgetc(fp);
      if (feof(fp) != 0)
         break;
      sz++;
   }

   return sz;
}


void GBAMethods::DecodeSNESPal(long offset, COLOR2* dstPal, int numPals, char palpos, int size, vector<unsigned char>* buffer) {

	int             i, ii, s = 0;
	long            index = 0;
	unsigned short             palGBA[512] = { 0 };

	//populate above array very shittly
	unsigned long red = 0x1F;
	unsigned long green = 0x1F << 5;
	unsigned long blue = 0x1F << 10;


	numPals = size / 32;
	numPals -= palpos;
	//memcpy(&palGBA[palpos*0x10], buffer, size);
	
	for (i = 0; i<buffer->size() / 2; i++) {
		memcpy(&palGBA[i], &buffer->front() + (i * 2), 2);
	}


	for (ii = palpos; ii < numPals; ii++)
	{
		index = 0x10 * ii;
		
		for (i = 0; i < 16; i++)
		{

			dstPal[index + i].r = (((palGBA[index + i]) >> 0) << 3) & 0xF8;
			dstPal[index + i].g = (((palGBA[index + i]) >> 5) << 3) & 0xF8;
			dstPal[index + i].b = (((palGBA[index + i]) >> 10) << 3) & 0xF8;

			dstPal[index + i].a = 255;



		}
	}

	return;
}
