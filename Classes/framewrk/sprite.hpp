#ifndef _SPRITE_HPP
#define _SPRITE_HPP

/* #include <compile.hpp> */

struct Cblitbuf;

#define NOKEY  (0)
#define USEKEY (1)

UINT16 frmwrk_fastblit(Cblitbuf *src, Cblitbuf &dest, int x1, int y1, int x2, int y2, int w, int h, int key);


typedef void (*COMP_SPRITE)(void);


extern void sprite_internal_blit(BYTE **linestart, COMP_SPRITE compspr, 
         UINT16 x);

#if 0
#pragma aux sprite_internal_blit = \
  "call eax" \
  parm [ebx] [eax] [ecx];
#endif

class Cspr_frame 
{
public:
/* Constructors */
  Cspr_frame(COMP_SPRITE *dc, UINT16 w, UINT16 h, INT16 hx, INT16 hy);
  Cspr_frame(Cblitbuf &blitbuf, INT16 hx, INT16 hy, UINT16 compileflg);
  Cspr_frame(char *pcx_filename, INT16 hx, INT16 hy, UINT16 compileflg);
  ~Cspr_frame(void);

/* Interface */
/* Normal draw routine blits with colorkey! */
/* returns TRUE if blitted, FALSE if outside screen */
  UINT16 draw(Cblitbuf &blitbuf, INT16 x, INT16 y);
/* draw routine which blits only the source rectangle x1,y1 - x2,y2 colorkeyed*/
  UINT16 draw(Cblitbuf &blitbuf, INT16 x, INT16 y,
      UINT16 x1, UINT16 y1,
      UINT16 x2, UINT16 y2);
/* Normal draw routine blits without colorkey! */
  UINT16 draw_nokey(Cblitbuf &blitbuf, INT16 x, INT16 y);
/* draw routine which blits only the source rectangle x1,y1 - x2,y2 not colorkeyed*/
  UINT16 draw_nokey(Cblitbuf &blitbuf, INT16 x, INT16 y,
      UINT16 x1, UINT16 y1,
      UINT16 x2, UINT16 y2);

  COMP_SPRITE get_draw_code() { return draw_code; }

  UINT16 get_width(void) { return width; }
  UINT16 get_height(void) { return height; }
  UINT16 get_pitch(void) {return pitch;} // when not locked=>assertion
  BYTE *lock_buffer(void);
  void unlock_buffer(void);

private:
  Cblitbuf *sprblbuf;
  VG_BOOLEAN bb_user_allocated;    // TRUE if dirdrawbuffer allocated by the user, don't free!
  VG_BOOLEAN dc_user_allocated;    // TRUE if drawcode allocated by the user, don't free!
  COMP_SPRITE draw_code;
  UINT16  width;
  UINT16  height;
  INT16   hotx;
  INT16   hoty;
  INT32  pitch;                  // only valid when blitbuf is locked
};

#if 0
class Cspr_sequence
{
public:
private:
  UINT16 num_frames;
  Cspr_frame **frames;
};

class Cspr_bg_save
{
public:
private:
  Cspr_bg_save *next; // linked list of areas to be restored in this order
  UINT16 x;
  UINT16 y;
  UINT16 width;
  UINT16 height;
  BYTE *save;     // buffer large enough to hold save data
};

class Cspr_blitbuf
{
public:

private:
  Cspr_bg_save *bg_save_list; // list of all save backgrounds, restore in order
  Cblitbuf *blitbuf;          // blitbuf sprites are drawn to
};
#endif

#endif /* _SPRITE_HPP */




