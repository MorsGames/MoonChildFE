#ifndef __GAPI_H
#define __GAPI_H

#ifdef GXDLL_EXPORTS
#define GXDLL_API __declspec(dllexport)
#else
#define GXDLL_API __declspec(dllimport)
#endif
struct GXDisplayProperties 
{
	DWORD cxWidth;
	DWORD cyHeight;
	long cbxPitch;
	long cbyPitch;
	long cBPP;
	DWORD ffFormat;
};
struct GXKeyList 
{
	short vkUp;
	MC_POINT ptUp;
	short vkDown;
	MC_POINT ptDown;
	short vkLeft;
	MC_POINT ptLeft;
	short vkRight;
	MC_POINT ptRight;
	short vkA;
	MC_POINT ptA;
	short vkB;
	MC_POINT ptB;
	short vkC;
	MC_POINT ptC;
	short vkStart;
	MC_POINT ptStart;
};
struct GXScreenRect 
{
	DWORD dwTop;
	DWORD dwLeft;
	DWORD dwWidth;
	DWORD dwHeight;
};
GXDLL_API int GXOpenDisplay(HWND hWnd, DWORD dwFlags);
GXDLL_API int GXCloseDisplay();
GXDLL_API void * GXBeginDraw();
GXDLL_API int GXEndDraw();
GXDLL_API int GXOpenInput();
GXDLL_API int GXCloseInput();
GXDLL_API GXDisplayProperties GXGetDisplayProperties();
GXDLL_API GXKeyList GXGetDefaultKeys(int iOptions);
GXDLL_API int GXSuspend();
GXDLL_API int GXResume();
GXDLL_API int GXSetViewport( DWORD dwTop, DWORD dwHeight, DWORD dwReserved1, DWORD dwReserved2 );
GXDLL_API BOOL GXIsDisplayDRAMBuffer();
#define GX_FULLSCREEN	0x01
#define GX_NORMALKEYS	0x02
#define GX_LANDSCAPEKEYS	0x03
#ifndef kfLandscape
	#define kfLandscape	0x8			// Screen is rotated 270 degrees
	#define kfPalette	0x10		// Pixel values are indexes into a palette
	#define kfDirect	0x20		// Pixel values contain actual level information
	#define kfDirect555	0x40		// 5 bits each for red, green and blue values in a pixel.
	#define kfDirect565	0x80		// 5 red bits, 6 green bits and 5 blue bits per pixel
	#define kfDirect888	0x100		// 8 bits each for red, green and blue values in a pixel.
	#define kfDirect444	0x200		// 4 red, 4 green, 4 blue
	#define kfDirectInverted 0x400
#endif

#endif