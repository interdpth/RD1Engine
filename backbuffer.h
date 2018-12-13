#pragma once




#include <Windows.h>
#include <stdio.h>
#include <time.h>
#define RGBA(r,g,b,a)         ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))  | a << 32)
class           BackBuffer
{
   public:
   BackBuffer();
   ~BackBuffer();
   void DestroyImage(HDC dc, HBITMAP bmpold, HBITMAP bmp);
   void            Create(int width, int height);
   void            Destroy();
   
   void            Clear(COLORREF clr = (COLORREF)0);
   int currentDCID = -1 ;
   HDC             DC()
   {
	
      return mDC;
   }
   int             GetWd()
   {
      return nWidth;
   }
   int             GetHt()
   {
	   return nHeight;
   }
	  void           *dummy;

   void            SaveToFile(FILE * file);
                   protected:
   void            Zero();
   BITMAPINFO      bi;
   HDC             mDC;
   HBITMAP         hBmp;
   HBITMAP         hBmpOld;

   int             nWidth;
   int             nHeight;
};
