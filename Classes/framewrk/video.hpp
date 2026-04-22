/* The video classes */

#ifndef _VIDEO_HPP
#define _VIDEO_HPP

#include <cstdio>

typedef struct MGLDC MGLDC;

struct MGLDC {
    BYTE *surface;
};

#pragma warning (disable : 4099)
class Cblitbuf;

class Cvideo {

public:
  Cvideo(void);
  ~Cvideo(void);
  
  VG_BOOLEAN on(unsigned char *pixelbuffer, UINT16 width, UINT16 height, UINT16 numcols);
  void off(void);
  
  void DrawSettings(void);
  void DrawLoading(void);


  Cvideo &palette(BYTE *palette); // from is index in VGA palette
  Cvideo &palette(BYTE *palette, UINT16 factor); // from is index in VGA palette... factor is fadevalue (0-255)
  Cvideo &palette(BYTE *palette, UINT16 factor, UINT16 number);
                                  // number is till which color the factor
                                  // has any effect
  Cvideo &palette_index(BYTE *palette, UINT16 startcol, UINT16 numcol);
  Cvideo &returnpal(BYTE *destpal);
  Cvideo &clear(UINT16 color_index);
  void swap(void);
  void set_fade_swap_from_snapshot(bool enabled);
  Cvideo &scansync(void);
  
  Cvideo &copy_blitbuf(Cblitbuf &blitbuf, UINT16 dx, UINT16 dy, 
           UINT16 w, UINT16 h, UINT16 sx, UINT16 sy);
  Cvideo &para_blitbuf(Cblitbuf &blitbuf, UINT16 dx, UINT16 dy, 
           UINT16 w, UINT16 h, UINT16 sx, UINT16 sy);
  
  void dump(FILE *fd);

  UINT32 queryfreevram(void) { return freevram; }       // return the nr of free videoram bytes

  UINT16 get_width(void) { return width; }
  UINT16 get_height(void) { return height; }
  UINT16 get_fpitch(void) {return fpitch;} // when not locked=>assertion
  UINT16 get_bpitch(void) {return bpitch;} // when not locked=>assertion
  BYTE  *lock_fbuffer(void);
  void   unlock_fbuffer(void); 
  BYTE  *lock_bbuffer(void);
  void   unlock_bbuffer(void); 
   
  void   ShowFrameRate(void);
  void	 ShowMouseCursor(void);

  void   DisplayChars(unsigned char *Cyfers, int x, int y);
  void   DisplayChars2(unsigned char *Cyfers, int x, int y);
  void   Line( int x1, int y1, int x2, int y2, char color );

//  LPDIRECTDRAW get_ddraw(void) {return glpDirectDraw;}   //DON'T USE EXTERNALLY!!!!    #######
//  LPDIRECTDRAWSURFACE get_fddraw(void) { return glpFrontBuffer; }  // DON'T USE EXTERNALLY!!!! #######
//  LPDIRECTDRAWSURFACE get_bddraw(void) { return glpBackBuffer; }   // DON'T USE EXTERNALLY!!!! #######
private:
	void	ConvertPalToDib(void);

    unsigned char *pixelBuffer;     //hardware pixelbuffer
  VG_BOOLEAN   in_graphics;
  VG_BOOLEAN   useLinear;
  UINT16 width;
  UINT16 height;
  UINT16 colors;
  INT32  fpitch;                  // only valid when blitbuf is locked
  INT32  bpitch;                  // only valid when blitbuf is locked
  UINT32 freevram;    // nr of bytes free in Videoram available...

  char *m_OffscreenBuf;		// gewoon een char array
  char *m_FadeSnapshotBuffer{nullptr};
  bool m_FadeUsesSnapshot{false};
  UBYTE m_Palette[768];	// real palette		(r g b byte)
  short m_DibPalette[256];	// fast palette  (word met rgb)
  short m_DibPaletteIP[256];	// fast palette  (word met rgb) voor interpolatie alvast gehalved
    unsigned int m_DibPalette32[256]; // palette in RGBA 32 bits
    unsigned int m_DibPaletteIP32[256];	// correct interpol palette  (word met rgb) voor interpolatie alvast gehalved
  short m_RPalette[256];
  short m_GPalette[256];
  short m_BPalette[256];
};

class Cblitbuf {
  friend Cvideo &Cvideo::copy_blitbuf(Cblitbuf &blitbuf, UINT16 dx, UINT16 dy, 
              UINT16 w, UINT16 h, UINT16 sx, UINT16 sy);
  
public:
  Cblitbuf(Cvideo *video);      // give a blitbuffer description of the videobackbuffer
  Cblitbuf(UINT16 width, UINT16 height, UINT16 top_margin, UINT16 bottom_margin);
  Cblitbuf(UINT16 width, UINT16 height, UINT16 top_margin, UINT16 bottom_margin, UINT16 forcesystemmem);
  Cblitbuf(char *pcx_filename, UINT16 top_margin, UINT16 bottom_margin, UINT16 forcesystemmem = 0); // create with dimensions of PCX file
  ~Cblitbuf(void);
  void save_mem(UINT16 x, UINT16 y, UINT16 w, UINT16 h, BYTE *buf);
  void load_mem(UINT16 x, UINT16 y, UINT16 w, UINT16 h, BYTE *buf);
  VG_BOOLEAN save_pcx(char *filename, BYTE *palette);
  void compiled_sprite(UINT16 x, UINT16 y, COMP_SPRITE compspr);

  void dump(FILE *fd);

  UINT16 get_width(void) { return width; }
  UINT16 get_height(void) { return height; }
  UINT16 get_clipx(void) { return clipx; }
  UINT16 get_clipy(void) { return clipy; }
  UINT16 get_clipwidth(void) { return clipwidth; }
  UINT16 get_clipheight(void) { return clipheight; }
  void   set_clipping(UINT16 cx, UINT16 cy, UINT16 cw, UINT16 ch);
  UINT16 get_pitch(void) {return pitch;} // when not locked=>assertion
  UINT16 draw(Cblitbuf &blitbuf, INT16 x, INT16 y); // return TRUE if blitted in screen
  UINT16 draw(Cblitbuf &blitbuf, INT16 x, INT16 y,  // returns false if outside screen
          UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2);
  UINT16 draw_nokey(Cblitbuf &blitbuf, INT16 x, INT16 y);
  UINT16 draw_nokey(Cblitbuf &blitbuf, INT16 x, INT16 y,
          UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2);
  Cblitbuf &clear(UINT16 color_index);
  BYTE  *lock_buffer(void);
  void   unlock_buffer(void); 
//  LPDIRECTDRAWSURFACE get_ddraw(void) { return ddsurface; }   // DON'T USE EXTERNALLY
  void   debug(void);                                         // print debug stats
private:
  VG_BOOLEAN videoblitbuf;       // TRUE if it is a blitbuf derived from the videoclass
  UINT16 width;
  UINT16 height;
  UINT16 clipx;
  UINT16 clipy;
  UINT16 clipwidth;
  UINT16 clipheight;
  INT32  pitch;                  // only valid when blitbuf is locked
//  LPDIRECTDRAWSURFACE ddsurface;

    char *m_Buffer;		// gewoon een char array
};



#endif /* _VIDEO_HPP */




