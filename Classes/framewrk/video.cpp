#include "frm_int.hpp"

MC_PALETTEENTRY        dumpe[1024];        // last entered palette  (for requests)
MC_PALETTEENTRY        pe[1024];        // last entered palette  (for requests)

short g_FrameDropper;
char *g_OffScreenBuf;

short g_RenderMode = 0;

#define NUMRENDERMODES (4)

Cvideo::Cvideo(void)
{
  in_graphics = VG_FALSE;
  g_FrameDropper = 0;
}

Cvideo::~Cvideo(void)
{
  if (in_graphics)
    {
       off();
    }
}

          
VG_BOOLEAN Cvideo::on(unsigned char *pixelbuffer, UINT16 w, UINT16 h, UINT16 numcols)
{
//  vgassert(!in_graphics);
//  vgassert(numcols == 256);

    this->pixelBuffer = pixelbuffer;
    
  width = w;
  height = h;

  m_OffscreenBuf = (char *)malloc(w*h);
    memset(m_OffscreenBuf, 0, w*h);
  m_FadeSnapshotBuffer = (char *)malloc(w*h);
  memset(m_FadeSnapshotBuffer, 0, static_cast<size_t>(w) * h);
  g_OffScreenBuf = m_OffscreenBuf;			// easy way to check for blitroutien if it is to backbuffer or not so we can skip it

  in_graphics = VG_TRUE;
  return VG_TRUE;
}


void Cvideo::off(void)
{
//  vgassert(in_graphics);

  free(m_OffscreenBuf);
  free(m_FadeSnapshotBuffer);
  m_FadeSnapshotBuffer = nullptr;

  in_graphics = VG_FALSE;
}


unsigned char *Cvideo::lock_fbuffer(void)
{
    fpitch = width;

    // Copy bitmap bits from system memory to video memory
    return (unsigned char *)m_OffscreenBuf;
}


void Cvideo::unlock_fbuffer(void)
{
    fpitch = 0;
}


unsigned char *Cvideo::lock_bbuffer(void)
{
    bpitch = width;

    // Copy bitmap bits from system memory to video memory
    return (unsigned char *)m_OffscreenBuf;
}


void Cvideo::unlock_bbuffer(void)
{
    bpitch = 0;
}


//extern GXDisplayProperties g_gxdp;		// GX struct

extern unsigned short *LoadingPic;
extern unsigned short *SettingsPic;
extern unsigned short *ButPic[6];
extern unsigned short *SwitchPic[6];




void Cvideo::DrawSettings(void)
{
    /*
	Surface *HalSurface;

	if(g_Display->BeginScene())
	{
		unsigned short *SrcPtr;
		unsigned short *PixelBuf;
		unsigned int Pitch;
		HalSurface = g_Display->GetBackBuffer();
		PixelBuf = (unsigned short *)HalSurface->GetPixels();
		Pitch = HalSurface->GetPitch();

		//achtergrond wissen
		SrcPtr = (unsigned short *)SettingsPic;
		for (unsigned int y = 0; y < 240; y++) {
			unsigned short * pusDest = PixelBuf;
			for (unsigned int x = 0; x < 320; x++) {
				*pusDest++ = *SrcPtr++;
			}
			PixelBuf += (Pitch>>1);
		}
		g_Display->Swap(SWAP_FULL_UPDATE);
	}
     */
}


void Cvideo::DrawLoading(void)
{
    /*
	Surface *HalSurface;

	if(g_Display->BeginScene())
	{
		unsigned short *PixelBuf;
		unsigned int Pitch;
		HalSurface = g_Display->GetBackBuffer();
		PixelBuf = (unsigned short *)HalSurface->GetPixels();
		Pitch = HalSurface->GetPitch();

		//achtergrond wissen
		for (unsigned int y = 0; y < 240; y++) {
			unsigned short * pusDest = PixelBuf;
			for (unsigned int x = 0; x < 320; x++) {
				*pusDest++ = 0;
			}
			PixelBuf += (Pitch>>1);
		}
     }
     */
     
}

#define UNROLL *DestBuf++ = m_DibPalette32[*SrcPtr++];
#define UNROLL8 UNROLL UNROLL UNROLL UNROLL UNROLL UNROLL UNROLL UNROLL
#define UNROLL32 UNROLL8 UNROLL8 UNROLL8 UNROLL8

// To fix the fade out effect in-game
void Cvideo::set_fade_swap_from_snapshot(bool enabled)
{
	m_FadeUsesSnapshot = enabled;
}

void Cvideo::swap(void)
{
	static int interlace;

	
	if(g_RenderMode>=4)
	{
		// ziet er gek uit...moet in deze volgorde bestaan!
		if(g_FrameDropper!=0)
		{
			g_FrameDropper++;
			g_FrameDropper&=1;
			return;
		}
		g_FrameDropper++;
		g_FrameDropper&=1;
	}
	interlace++;
	interlace&=1;

//	ShowMouseCursor();

	
    unsigned char *SrcPtr;
    unsigned int *DestBuf;
    DestBuf = (unsigned int *)this->pixelBuffer;

	if (m_FadeSnapshotBuffer != nullptr)
	{
		if (m_FadeUsesSnapshot)
		{
			SrcPtr = (unsigned char *)m_FadeSnapshotBuffer;
		}
		else
		{
			memcpy(m_FadeSnapshotBuffer, m_OffscreenBuf, static_cast<size_t>(width) * height);
			SrcPtr = (unsigned char *)m_OffscreenBuf;
		}
	}
	else
	{
		SrcPtr = (unsigned char *)m_OffscreenBuf;
	}
    
    if((g_RenderMode%NUMRENDERMODES)==0) // nearest neigbour
    {
        for (unsigned int y = 0; y < 480; y++) {
            for (unsigned int x = 0; x < (640>>5); x++) {
//                *DestBuf++ = m_DibPalette32[*SrcPtr++];
                UNROLL32
            }
        }
    }
    
/*	
	if(g_Display->BeginScene())
	{
		unsigned char *SrcPtr;
		unsigned short *PixelBuf;
		unsigned int Pitch;
		HalSurface = g_Display->GetBackBuffer();
		PixelBuf = (unsigned short *)HalSurface->GetPixels();
		Pitch = HalSurface->GetPitch();

		if((g_RenderMode%NUMRENDERMODES)==0) // nearest neigbour
		{
			SrcPtr = (unsigned char *)m_OffscreenBuf;
			for (unsigned int y = 0; y < 240; y++) {
				unsigned short * pusDest = PixelBuf;
				for (unsigned int x = 0; x < 320; x++) {
					*pusDest++ = m_DibPalette[*SrcPtr];
					SrcPtr+=2;
				}
				PixelBuf += (Pitch>>1);
				SrcPtr+=640;
			}
		}

		if((g_RenderMode%NUMRENDERMODES)==1) // interlace
		{
			SrcPtr = (unsigned char *)m_OffscreenBuf;
			if(interlace)
			{
				PixelBuf += 320;
				SrcPtr+=1280;
			}
			for (unsigned int y = 0; y < 120; y++) {
				unsigned short * pusDest = PixelBuf;
				for (unsigned int x = 0; x < 320; x++) {
					*pusDest++ = m_DibPalette[*SrcPtr];
					SrcPtr+=2;
				}
				PixelBuf += Pitch;
				SrcPtr+=640*3; //
			}
		}

		if((g_RenderMode%NUMRENDERMODES)==2) // fast interpolation
		{
			register unsigned short col;
			SrcPtr = (unsigned char *)m_OffscreenBuf;
			for (unsigned int y = 0; y < 240; y++) {
				unsigned short * pusDest = PixelBuf;
				for (unsigned int x = 0; x < 320; x++) {
					col  = m_DibPaletteIP[*(SrcPtr)];
					col += m_DibPaletteIP[*(SrcPtr+1)];
					col += m_DibPaletteIP[*(SrcPtr+640)];
					col += m_DibPaletteIP[*(SrcPtr+641)];

					*pusDest++ = col;
					SrcPtr+=2;
				}
				PixelBuf += (Pitch>>1);
				SrcPtr+=640;
			}
		}

		if((g_RenderMode%NUMRENDERMODES)==3) // interlace+interpolation
		{
			register unsigned short col;
			SrcPtr = (unsigned char *)m_OffscreenBuf;
			if(interlace)
			{
				PixelBuf += 320;
				SrcPtr+=1280;
			}
			for (unsigned int y = 0; y < 120; y++) {
				unsigned short * pusDest = PixelBuf;
				for (unsigned int x = 0; x < 320; x++) {
					col  = m_DibPaletteIP[*(SrcPtr)];
					col += m_DibPaletteIP[*(SrcPtr+1)];
					col += m_DibPaletteIP[*(SrcPtr+640)];
					col += m_DibPaletteIP[*(SrcPtr+641)];

					*pusDest++ = col;
					SrcPtr+=2;
				}
				PixelBuf += Pitch;
				SrcPtr+=640*3; //
				pusDest+=320; //extra regel overslaan voor interlacing
			}
		}

		
		
		g_Display->Swap(SWAP_FULL_UPDATE);
	}
 */

	
}


unsigned char charset[36*10] = 
{
	16,16,16,16,16,0,
	16,0,0,0,16,0,
	16,0,0,0,16,0,
	16,0,0,0,16,0,
	16,16,16,16,16,0,
	0,0,0,0,0,0,

	0,16,16,0,0,0,
	0,0,16,0,0,0,
	0,0,16,0,0,0,
	0,0,16,0,0,0,
	16,16,16,16,16,0,
	0,0,0,0,0,0,

	16,16,16,16,16,0,
	0,0,0,0,16,0,
	16,16,16,16,16,0,
	16,0,0,0,0,0,
	16,16,16,16,16,0,
	0,0,0,0,0,0,

	16,16,16,16,16,0,
	0,0,0,0,16,0,
	0,0,16,16,16,0,
	0,0,0,0,16,0,
	16,16,16,16,16,0,
	0,0,0,0,0,0,

	16,0,0,16,0,0,
	16,0,0,16,0,0,
	16,16,16,16,16,0,
	0,0,0,16,0,0,
	0,0,0,16,0,0,
	0,0,0,0,0,0,

	16,16,16,16,16,0,
	16,0,0,0,0,0,
	16,16,16,16,16,0,
	0,0,0,0,16,0,
	16,16,16,16,16,0,
	0,0,0,0,0,0,

	16,16,16,16,16,0,
	16,0,0,0,0,0,
	16,16,16,16,16,0,
	16,0,0,0,16,0,
	16,16,16,16,16,0,
	0,0,0,0,0,0,

	16,16,16,16,0,0,
	0,0,0,16,0,0,
	0,0,16,16,16,0,
	0,0,0,16,0,0,
	0,0,0,16,0,0,
	0,0,0,0,0,0,

	16,16,16,16,16,0,
	16,0,0,0,16,0,
	16,16,16,16,16,0,
	16,0,0,0,16,0,
	16,16,16,16,16,0,
	0,0,0,0,0,0,

	16,16,16,16,16,0,
	16,0,0,0,16,0,
	16,16,16,16,16,0,
	0,0,0,0,16,0,
	16,16,16,16,16,0,
	0,0,0,0,0,0,

};


unsigned int Ticks[8];



extern int g_MouseFlg;
extern int g_MouseXCurrent;
extern int g_MouseYCurrent;

extern int frmwrk_CenterX;
extern int frmwrk_CenterY;

#define FPP (8)

extern unsigned short levelloadedflg;

void Cvideo::ShowMouseCursor(void)
{
	int x,y,x2,y2;
	unsigned char *Screen;

	if(levelloadedflg==0) return;		//alleen tijdens levels!

	Screen = lock_bbuffer();


	if(g_MouseFlg)
	{
		{
			int i;

			x = g_MouseXCurrent;
			y = g_MouseYCurrent;

	//		y = frmwrk_CenterY;
	//		x = frmwrk_CenterX;
			if(x>8 && x<632 && y>8 &&y<472)
			{
				for(i=0; i<8; i++)
				{
					*(Screen + y*640 + (x+i)) = 0x10;
					*(Screen + y*640 + (x-i)) = 0x10;
					*(Screen + (y+i)*640 + x) = 0x10;
					*(Screen + (y-i)*640 + x) = 0x10;
				}
			}
		}
		{
			int i;

			x2 = frmwrk_CenterX;
			y2 = frmwrk_CenterY;

	//		y = frmwrk_CenterY;
	//		x = frmwrk_CenterX;
			if(x2>8 && x2<632 && y2>8 &&y2<472)
			{
				for(i=0; i<8; i++)
				{
					*(Screen + y2*640 + (x2+i)) = 0x10;
					*(Screen + y2*640 + (x2-i)) = 0x10;
					*(Screen + (y2+i)*640 + x2) = 0x10;
					*(Screen + (y2-i)*640 + x2) = 0x10;
				}
			}
		}

		Line(x<<FPP, y<<FPP, x2<<FPP, y2<<FPP, 0x10);
	}
}


void Cvideo::Line( int x1, int y1, int x2, int y2, char color ) 
{ 
	if (y1>y2) 
	{ 
		int h=y1; 
		y1=y2, y2=h; 
		h=x1, x1=x2, x2=h; 
	} 
	int dx=0, dy=0, len = abs( x2-x1 ); 
	if (abs( y2 - y1 ) > len) len = abs( y2 - y1 ); 
	len = len >> FPP; 
	if (len != 0) 
	{ 
		dx = (x2 - x1) / len; 
		dy = (y2 - y1) / len; 
	} 
	if (y1 < 0) 
	{ 
		if (dy > 0) 
		{ 
			int i = -(y1 / dy) + 1; 
			x1 += dx * i; 
			y1 += dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x1 < 0) 
	{ 
		if (dx > 0) 
		{ 
			int i = -(x1 / dx) + 1; 
			x1 += dx * i; 
			y1 += dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (y2 > (480 << FPP)) 
	{ 
		if (dy > 0) 
		{ 
			int i = ((y2 - (480 << FPP)) / dy) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 
	if (len > 0) if (x2 > (640 << FPP)) 
	{ 
		if (dx > 0) 
		{ 
			int i = ((x2 - (640 << FPP)) / dx) + 1; 
			x2 -= dx * i; 
			y2 -= dy * i; 
			len -= i; 
		} 
		else len = 0; 
	} 

	unsigned char *Screen;

	Screen = lock_bbuffer();

	for ( int i = 0; i < len + 1; i++) 
	{ 
        unsigned int px = x1>>FPP;
        unsigned int py = y1>>FPP;
        if(px<640 && py<480)
        {
            Screen[(py * 640) + px] = color; 
        }
		x1 += dx; 
		y1 += dy; 
	}

	unlock_bbuffer();
}

void Cvideo::ShowFrameRate(void)
{
    /*
	unsigned char buffer[16];
	int i;
	int FrameRate;
	for(i=0;i<7;i++)
	{
		Ticks[i] = Ticks[i+1];
	}

	Ticks[7] = GetTickCount();

	FrameRate = (Ticks[7] - Ticks[0])/8;
	if(FrameRate!=0)
	{
		FrameRate = 1000/FrameRate;
	}

	buffer[2] = FrameRate/100;
	if(buffer[2]>9)
	{
		buffer[2] = 9;
		buffer[1] = 9;
		buffer[0] = 9;
	}
	else
	{
		buffer[1] = (FrameRate-(buffer[2]*100))/10;
		buffer[0] = FrameRate%10;
	}
	buffer[3] = 255;


	// display chars:

	DisplayChars(buffer, 0, 0);
     */
}


void Cvideo::DisplayChars(unsigned char *Cijfers, int x, int y)
{
	int Cijfer;
	int Index;
	unsigned char *Screen;

	Screen = lock_bbuffer();

	Index = 2;

	while(Index>=0)
	{
		int x,y;
		unsigned char *CharsetPtr;
		unsigned char *ScreenPtr;
		Cijfer = Cijfers[Index];

		ScreenPtr = Screen;
		CharsetPtr = charset + (Cijfer*36);

		for(y=0;y<6;y++)
		{
			for(x=0;x<6;x++)
			{
				*ScreenPtr++ = *CharsetPtr;
				*ScreenPtr++ = *CharsetPtr++;
			}
			ScreenPtr += (640-12);
			CharsetPtr-=6;
			for(x=0;x<6;x++)
			{
				*ScreenPtr++ = *CharsetPtr;
				*ScreenPtr++ = *CharsetPtr++;
			}
			ScreenPtr += (640-12);
		}
		Screen+= 14;
		Index--;
	}

	unlock_bbuffer();
}

void Cvideo::DisplayChars2(unsigned char *Cijfers, int x, int y)
{
	int Cijfer;
	int Index;
	unsigned short *Screen;

	Screen = SettingsPic;

	Index = 2;

	while(Index>=0)
	{
		int x,y;
		unsigned char *CharsetPtr;
		unsigned short *ScreenPtr;
		Cijfer = Cijfers[Index];

		ScreenPtr = Screen;
		CharsetPtr = charset + (Cijfer*36);

		for(y=0;y<6;y++)
		{
			for(x=0;x<6;x++)
			{
				unsigned short Pix;
				if(*CharsetPtr++==0)
				{
					Pix =0;
				}
				else
				{
					Pix=0xffff;
				}
				*ScreenPtr++ = Pix;
				*ScreenPtr++ = Pix;
			}
			ScreenPtr += (240-12);
			CharsetPtr-=6;
			for(x=0;x<6;x++)
			{
				unsigned short Pix;
				if(*CharsetPtr++==0)
				{
					Pix =0;
				}
				else
				{
					Pix=0xffff;
				}
				*ScreenPtr++ = Pix;
				*ScreenPtr++ = Pix;
			}
			ScreenPtr += (240-12);
		}
		Screen+= 14;
		Index--;
	}
}


Cvideo &Cvideo::clear(UINT16 color_index)
{
	memset(m_OffscreenBuf, color_index, width*height);

  return *this;
}



Cvideo &Cvideo::palette(BYTE *palette)
{
  int                 i;
  UBYTE *PalPtr;

//  vgassert(in_graphics);
//  vgassert(palette);

  PalPtr = m_Palette;
  for( i=0; i<256; i++ )
    {
    *PalPtr++   = *(palette++);
    *PalPtr++ = *(palette++);
    *PalPtr++  = *(palette++);
    }
    
  
	ConvertPalToDib();

	return *this;
}


Cvideo &Cvideo::palette_index(BYTE *palette, UINT16 startcol, UINT16 numcol)
{
  int                 i;
  UBYTE *PalPtr;
//  vgassert(in_graphics);
//  vgassert(palette);

  PalPtr = m_Palette + (startcol*3);
  for( i=startcol; i<(startcol+numcol); i++ )
    {
    *PalPtr++   = *(palette++);
    *PalPtr++ = *(palette++);
    *PalPtr++  = *(palette++);
    }

  // convert naar dib palette
	ConvertPalToDib();

  return *this;
}


void Cvideo::ConvertPalToDib(void)
{
	unsigned short r,g,b;
	int i;
	
	for(i=0;i<256; i++)
	{
		unsigned int t;
		r = m_Palette[i*3 + 0]>>3;
		g = m_Palette[i*3 + 1]>>2;
		b = m_Palette[i*3 + 2]>>3;
		// This was originally BGR, but I'm changing it to RGB here to make porting easier -Mors
        m_DibPalette32[i] = (m_Palette[i*3 + 0]) | (m_Palette[i*3 + 1]<<8) | (m_Palette[i*3 + 2]<<16) | (255<<24);
   
        
		m_DibPalette[i] = (b) + (g<<5) + (r<<11);
		m_DibPaletteIP[i] = m_DibPalette[i];
		m_DibPaletteIP[i] >>= 2;
		m_DibPaletteIP[i] &= 0x39e7;	// 16 bits fast interpolation

		t = m_DibPalette[i];
		t &= 0x1863;
		t <<= 16;
		t |= m_DibPaletteIP[i];
		m_DibPaletteIP32[i] = t;	// 32 bits correct interpolation
	}

	for(i=0; i<256; i++)
	{
		m_RPalette[i] = (m_Palette[i*3 + 0]>>2);
		m_GPalette[i] = (m_Palette[i*3 + 1]>>2);
		m_BPalette[i] = (m_Palette[i*3 + 2]>>2);
	}

}

Cvideo &Cvideo::palette(BYTE *palette, UINT16 factor)
{
	int                 i;
	UBYTE *PalPtr;

//	vgassert(in_graphics);
//	vgassert(palette);

	PalPtr = m_Palette;
	for( i=0; i<256; i++ )
	{
		*PalPtr++   = ((*(palette++))*factor)>>8;
		*PalPtr++ = ((*(palette++))*factor)>>8;
		*PalPtr++  = ((*(palette++))*factor)>>8;
	}

	ConvertPalToDib();
    
	return *this;
}


Cvideo &Cvideo::palette(BYTE *palette, UINT16 factor, UINT16 number)
{
  int                 i;
  UBYTE *PalPtr;

//  vgassert(in_graphics);
//  vgassert(palette);

  PalPtr = m_Palette;
  for( i=0; i<256; i++ )
    {
    if (i<number)
      {
        *PalPtr++   = ((*(palette++))*factor)>>8;
        *PalPtr++ = ((*(palette++))*factor)>>8;
        *PalPtr++  = ((*(palette++))*factor)>>8;
      }
    else
      {
        *PalPtr++   = *(palette++);
        *PalPtr++ = *(palette++);
        *PalPtr++  = *(palette++);
      }
    }
    
  ConvertPalToDib();

  return *this;
}

Cvideo &Cvideo::returnpal(BYTE *destpal)
{
	
	destpal = m_Palette;

	return *this;
}

Cvideo &Cvideo::scansync(void)
{

	// euhm   (for i lus?!?!)


  return *this;
}


// Copy a rectangle from the blitbuffer <blitbuf> to the video screen. 
// Copy from upperleft corner (<x>,<y>) in the blitbuffer to (0,0) at the
// video screen. Only copy up to the bottom-right corner of the blitbuffer.
// Use the linestarts table of the blitbuf.

Cvideo &Cvideo::copy_blitbuf(Cblitbuf &blitbuf, UINT16 dx, UINT16 dy, 
           UINT16 w, UINT16 h, UINT16 sx, UINT16 sy)
{
	//notimpl

  return *this;
}

Cvideo &Cvideo::para_blitbuf(Cblitbuf &blitbuf, UINT16 dx, UINT16 dy, 
           UINT16 w, UINT16 h, UINT16 sx, UINT16 sy)
{
	//notimpl


  return *this;
}

void Cvideo::dump(FILE *fd)
{
  fprintf(fd, "Information video mode:\n");
  if (!in_graphics)
    {
      fprintf(fd, "Currently NOT in graphics mode!\n");
    }
  else
    {
      fprintf(fd, "Horizontal resolution: %d\n", width);
      fprintf(fd, "Vertical resolution  : %d\n", height);
      fprintf(fd, "Number of colors     : %d\n", colors);
    }

}








