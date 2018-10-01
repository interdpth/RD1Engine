#include <Windows.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenGL window

class COpenGL
{
// Construction
public:
	COpenGL(HWND hdc, int width, int height, unsigned char* pixelBuffer);
	HWND m_hWnd;
// Attributes
public:
	HDC hDC;				/* device context */
	HGLRC hRC;				/* opengl context */
	HPALETTE hPalette = 0;			/* custom palette (if needed) */
	unsigned* image;			/* texture image */
	int iwidth;
	int iheight;
	int idepth;
	unsigned long* rawPalette;
	unsigned long* rawImage;
// Operations
public:
	void Resize(int width, int height);
	void UpdateDisplay();
	HWND CreateOpenGLWindow(HWND hWnd, int x, int y, int width, int height, BYTE type, DWORD flags);
	void  DrawTriangle();
	void  DrawPixelTexture();

// Implementation

public:
	
	virtual ~COpenGL();
	HWND mywnd;
	// Operations
public:
	//{{AFX_MSG(COpenGL)
	int OnCreate();
	void OnPaint();
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

