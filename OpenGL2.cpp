// OpenGL.cpp : implementation file
//

//#include "texture.h"

#include "OpenGL2.h"
#include <gl/gl.h>
#include <gl/glu.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGL

COpenGL::COpenGL(HWND src, int width , int height, unsigned char* pixelBuffer)
{
	iwidth = width;
	iheight = height;
	idepth = 4;
	m_hWnd = src;
	CreateOpenGLWindow(src,0, 0, width, height,
		PFD_TYPE_RGBA, 0);
	hDC = GetDC(src);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	mywnd = src;
}

COpenGL::~COpenGL()
{
	wglMakeCurrent(NULL, NULL);
	ReleaseDC(mywnd, hDC);
	wglDeleteContext(hRC);
}
//
//
//BEGIN_MESSAGE_MAP(COpenGL, CWnd)
//	//{{AFX_MSG_MAP(COpenGL)
//	ON_WM_CREATE()
//	ON_WM_PAINT()
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// COpenGL message handlers

int COpenGL::OnCreate() 
{
	//if (CWnd::OnCreate(lpCreateStruct) == -1)
	//	return -1;
	
	// TODO: Add your specialized creation code here
	HDC b = ::GetDC(m_hWnd);
//	MySetPixelFormat(b);
	::ReleaseDC(m_hWnd, b);
	return 0;
}

void COpenGL::OnPaint() 
{
	//PaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	HDC hdc = ::GetDC(m_hWnd);
	HGLRC hglrc;

	// TODO: Add your message handler code here
	glClearColor(0,0,0,0);
	glColor3f(1, 1, 1);

	if (hglrc = wglCreateContext(hdc))
	{ 
        // try to make it the thread's current rendering context 
        if(wglMakeCurrent(hdc, hglrc))
		{
 			//render here
			glClear(GL_COLOR_BUFFER_BIT);
			glBegin(GL_TRIANGLES);
			glColor3f(1, 0, 0);
			glVertex3f(.5, 0, 0);
			glColor3f(0, 1, 0);
			glVertex3f(0, .5, 0);
			glColor3f(0, 0, 1);
			glVertex3f(-.5, 0, 0);
			glEnd();
			SwapBuffers(hdc);
        } 
    }  

	wglMakeCurrent(NULL, NULL) ; 
	::ReleaseDC (m_hWnd, hdc) ; 
	wglDeleteContext(hglrc); 

	// Do not call CWnd::OnPaint() for painting messages
}


void COpenGL::DrawPixelTexture()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glDrawPixels(iwidth, iheight, GL_RGBA, GL_UNSIGNED_BYTE, rawImage);	
}

void COpenGL::DrawTriangle()
{

	/* rotate a triangle around */
	
	glRotatef(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glIndexi(1);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i(0, 1);
	glIndexi(2);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2i(-1, -1);
	glIndexi(3);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2i(1, -1);
	glEnd();
	
}

void COpenGL::Resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(0, 0);
	glPixelZoom((float)width / iwidth, (float)height / iheight);
}

void COpenGL::UpdateDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//hPalette = CreatePalette(rawPalette);
	//if (hPalette) {
	//	SelectPalette(hDC, hPalette, FALSE);
	//	RealizePalette(hDC);
	//}
	DrawPixelTexture();
	//DrawTriangle();
	glFlush();
	SwapBuffers(hDC);
}

HWND COpenGL::CreateOpenGLWindow(HWND hWnd, int x, int y, int width, int height, BYTE type, DWORD flags)
{
	int         n, pf;

	LOGPALETTE* lpPal;
	PIXELFORMATDESCRIPTOR pfd;
	
	if (!hWnd)
	{
		return hWnd;
	}

	hDC = GetDC(hWnd);

	/* there is no guarantee that the contents of the stack that become
	the pfd are zeroed, therefore _make sure_ to clear these bits. */
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
	pfd.iPixelType = type;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 32;

	pf = ChoosePixelFormat(hDC, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
			"Cannot find a suitable pixel format.", "Error", MB_OK);
		return 0;
	}

	if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat() failed:  "
			"Cannot set format specified.", "Error", MB_OK);
		return 0;
	}

	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if (pfd.dwFlags & PFD_NEED_PALETTE) {
		int redMask, greenMask, blueMask, i;

		n = 1 << pfd.cColorBits;
		if (n > 512) n = 512;

		lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
			sizeof(PALETTEENTRY) * n);
		memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = n;

		GetSystemPaletteEntries(hDC, 0, n, &lpPal->palPalEntry[0]);

		/* assume an RGBA pixel type. */
		redMask = (1 << pfd.cRedBits) - 1;
		greenMask = (1 << pfd.cGreenBits) - 1;
		blueMask = (1 << pfd.cBlueBits) - 1;

		/* fill in the entries with an RGB color ramp. */
		for (i = 0; i < n; ++i) {
			lpPal->palPalEntry[i].peRed =
				(((i >> pfd.cRedShift)   & redMask) * 255) / redMask;
			lpPal->palPalEntry[i].peGreen =
				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
			lpPal->palPalEntry[i].peBlue =
				(((i >> pfd.cBlueShift)  & blueMask) * 255) / blueMask;
			lpPal->palPalEntry[i].peFlags = 0;
		}

		hPalette = CreatePalette(lpPal);
		if (hPalette) {
			SelectPalette(hDC, hPalette, FALSE);
			RealizePalette(hDC);
		}

		free(lpPal);
	}

	ReleaseDC(hWnd, hDC);

	return hWnd;
}
